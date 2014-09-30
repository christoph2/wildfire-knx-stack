#!/usr/bin/env python
# -*- coding: utf-8 -*-

__copyright__ = """
    KONNEX/EIB-Protocol-Stack.

   (C) 2001-2014 by Christoph Schueler <cpu12.gems@googlemail.com>

   All Rights Reserved

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

   s. FLOSS-EXCEPTION.txt
"""
__author__  = 'Christoph Schueler'
__version__ = '0.1.0'

"""
    TPUART 1/2, NCN5120 Pass-Thru Server.
"""

import struct
import threading
import types
import sys
import time
from optparse import OptionParser
import Queue
import zmq


from kstack.busaccess.tpuart import serialport
from kstack.busaccess.tpuart import utils

from kstack.busaccess.bif import BIF

BYTES_TO_READ   = 32 # 4

def marshall(arr):
    length = len(arr)
    fmt = "!H%uB" % length
    return struct.pack(fmt, length, *arr)

def unmarshall(blob):
    length = struct.unpack("!H", blob[: 2])[0]
    fmt = "%uB" % length
    data = struct.unpack(fmt, blob[2 :])
    return data


class TPUARTSerial(BIF):

    def __init__(self, **kws):
        super(TPUARTSerial, self).__init__(**kws)
        self.port = serialport.Serial(self.port, baudrate = self.baudrate,
            timeout = self.timeout, parity = self.parity
        )

    def connect(self):
        try:
            self.port.connect()
        except:
            raise
        else:
            self.connected = True

    def disconnect(self):
        self.port.close()

    def read(self, length):
        data =  self.port.read(length)
        return data

    def write(self, request):
        self.port.write(request)

    @property
    def closed(self):
        return self.port.closed


class Thread(threading.Thread):

    registry = []

    def __init__(self):
        super(Thread, self).__init__()
        self.quitEvent = threading.Event()
        self.register(self)

    def register(self, thread):
        Thread.registry.append(thread)

    def quit(self):
        self.quitEvent.set()
        self.join()

    @classmethod
    def quitAll(self):
        for thread in Thread.registry:
            thread.quit()

    def getName(self):
        return "%s-%u" % (self.__class__.__name__, self.ident)

    def run(self):
        print "Starting {0} thread.".format(self.getName())
        while True:
            signal = self.quitEvent.wait(timeout = 0.01)
            if signal == True:
                break
            try:
                if not self.execute():
                    break
            except Exception as e:
                print e
                break
        print "Exiting {0} thread.".format(self.getName())

    def execute(self):
        raise NotImplementedError("'execute()' method needs to be overriden.")


class Receiver(Thread):

    def __init__(self, portAdapter, context):
        super(Receiver, self).__init__()
        self.portAdapter = portAdapter
        self.context = context
        self.socket = self.context.socket(zmq.PUSH)
        #self.socket.setsockopt(zmq.RCVTIMEO, 100)
        #self.socket.setsockopt(zmq.SNDTIMEO, 100)
        self.socket.setsockopt(zmq.IDENTITY, "TPUART_ADAPTER_SENDER")
        self.socket.setsockopt(zmq.LINGER, 0)
        self.socket.setsockopt(zmq.SNDHWM, 1)
        self.socket.connect("tcp://localhost:5557")

    def execute(self):
        result = True
        if not self.portAdapter.closed:
            try:
                data = self.portAdapter.read(BYTES_TO_READ)
            except serialport.SerialException:
                data = None
                result = False
            if data:
                #print "RCV: '%s'" % utils.hexDump(data)
                print "RCV"
                if not self.context.closed:
                    try:
                        self.socket.send(marshall(data))
                    except zmq.error.Again:
                        pass
                    except zmq.error.ZMQError as e:
                        print "ZMQ-Error:", e
                        result = False
                    except Exception as e:
                        print str(e)
        return result


BIF_TYPES = {
    0:  "TPUART1",
    1:  "TPUART2",
    2:  "NCN5120",
}

def bifClass(simulation, bifType):
    if simulation:
        moduleName = BIF_TYPES[bifType].lower()
        module = __import__("kstack.busaccess.tpuart.vbus", fromlist = [moduleName])
        return getattr(module,  moduleName).Interface
    else:
        return TPUARTSerial

def main():

    parser = OptionParser()
    parser.add_option("-p", "--port", dest = "serialPort", default = 0, help = "Serial port number (default: 0)")
    parser.add_option("-s", "--simulation", action = "store_true", dest = "simulation",
        default = False, help = "Use virtual bus connection instead of actual one"
    )
    parser.add_option("-t", "--type", dest = "bif_type", default = 0, type = "int",
        help = "0 - TPUART1; 1 - TPUART2; 2 - NCN5120"
    )
    options, args = parser.parse_args()

    serialPort = options.serialPort
    if isinstance(serialPort, types.StringType) and serialPort.isdigit():
        serialPort = int(serialPort)

    #print bifClass(options.simulation, options.bif_type)
    #sys.exit()

    ctx = zmq.Context()
    socket = ctx.socket(zmq.PULL)
    socket.bind("tcp://*:5556")
    socket.setsockopt(zmq.IDENTITY, "TPUART_ADAPTER_RESPONDER")
    #socket.setsockopt(zmq.RCVTIMEO, 100)
    #socket.setsockopt(zmq.SNDTIMEO, 100)
    #socket.setsockopt(zmq.LINGER, 0)
    print "Server now listening."

    try:
        BifClass =  bifClass(options.simulation, options.bif_type)
        portAdapter = BifClass(port = serialPort, baudrate = 19200, timeout = 0.01, parity = serialport.Serial.PARITY_EVEN)
        portAdapter.connect()
    except Exception as e:
        print "\n", str(e), "\nexiting."
        socket.close()
        ctx.term()
        sys.exit(1)

    receiver = Receiver(portAdapter, ctx)
    receiver.start()
    poller = zmq.Poller()
    poller.register(socket, zmq.POLLIN)

    while True:
        try:
            result = dict(poller.poll(50))
            if result.get(socket):
                request = unmarshall(socket.recv())
                #print "REQ: ", utils.hexDump(request)
                print "REQ"
                portAdapter.write(request)
        except KeyboardInterrupt:
            print "*** Keyboard Interrupt, exiting ***"
            break

    portAdapter.disconnect()
    print "Exiting server..."
    socket.close()
    ctx.term()
    Thread.quitAll()

if __name__ == '__main__':
    main()


"""
import zmq
import signal
interrupted = False

def signal_handler(signum, frame):
    global interrupted
    interrupted = True


context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5558")

# or you can use a custom handler
counter = 0
signal.signal(signal.SIGINT, signal_handler)
while True:
    try:
        message = socket.recv(zmq.DONTWAIT)
    except zmq.ZMQError:
        pass
    counter += 1
    if interrupted:
        print "W: interrupt received, killing server..."
        break
"""
