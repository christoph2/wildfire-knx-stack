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


import types

import serial


class SerialException(serial.SerialException): pass


class Serial(object):

    def __init__(self, num, baudrate, bytesize = serial.EIGHTBITS, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE,
            timeout = 0.1):
        self.port = None
        self.num = num
        self.baudrate = baudrate
        self.bytesize = bytesize
        self.parity = parity
        self.stopbits = stopbits
        self.timeout = timeout

    def connect(self):
        try:
            self.port = serial.Serial(port = self.num, baudrate = self.baudrate, bytesize = self.bytesize, parity = self.parity,
                stopbits = self.stopbits, timeout = self.timeout
            )
        except serial.SerialException, e:
            raise SerialException(e)

    def __del__(self):
        if self.port and self.port.isOpen() == True:
            self.port.close()

    def write(self, data):
        if not isinstance(data, (types.ListType, types.TupleType)):
            data = [data]
        self.port.write(bytearray(data))
        #self.port.flush()

    def read(self, length):
        data = self.port.read(length)
        return bytearray(data)

    def close(self):
        if self.port is not None:
            if not self.port.closed:
                self.port.close()

for k, v in ((a, getattr(serial, a)) for a in dir(serial) if a.isupper()):
    setattr(Serial, k, v)

