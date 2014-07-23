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


from knxReTk.busaccess.tpuart import serialport
from knxReTk.busaccess.tpuart import utils

ACK = 0xe5

def marshall(arr):
    length = len(arr)
    fmt = "!H%uB" % length
    return struct.pack(fmt, length, *arr)

def unmarshall(blob):
    length = struct.unpack("!H", blob[: 2])[0]
    fmt = "%uB" % length
    data = struct.unpack(fmt, blob[2 :])
    return data

def connectSerial(port):
    port = serialport.Serial(port, 19200, timeout = 0.01, parity = serialport.Serial.PARITY_EVEN)
    port.connect()
    return port


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
            self.execute()
        print "Exiting {0} thread.".format(self.getName())

    def execute(self):
        raise NotImplementedError("'execute()' method needs to be overriden.")


class Receiver(Thread):

    def __init__(self, port, context):
        super(Receiver, self).__init__()
        self.port = port
        self.context = context
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:5557")

    def execute(self):
        data = self.port.read(32)
        if data:
            print "R: '%s'" % utils.hexDump(data)
            if not self.context.closed:
                self.socket.send(marshall(data))
                resp = self.socket.recv()
                print "ACK: ", resp

def main():
    parser = OptionParser()
    parser.add_option("-p", "--port", dest="port", default=0,
                  help="Serial port number (default: 0)")
    options, args = parser.parse_args()

    port = options.port
    if isinstance(port, types.StringType) and port.isdigit():
        port = int(port)

    ctx = zmq.Context()
    socket = ctx.socket(zmq.REP)
    socket.bind("tcp://*:5556")
    print "Server now listening."

    port = connectSerial(port)
    receiver = Receiver(port, ctx)
    print "Starting receiver thread."
    receiver.start()
    while True:
        try:
            request = unmarshall(socket.recv())
            port.write(request)
            print "REQ: ", request
        except KeyboardInterrupt:
            print "*** Keyboard Interrupt, exiting ***"
            break
        time.sleep(.5)
        socket.send(marshall([ACK]))
    print "Exiting server..."
    socket.close()
    ctx.term()
    Thread.quitAll()
    port.close()

if __name__ == '__main__':
    main()

