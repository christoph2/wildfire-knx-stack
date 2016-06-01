#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import ctypes
import logging
import os
import sys
#import random
import unittest
from ctypes import create_string_buffer

from common import ModuleIDs
from messaging import Messaging, Services, Errors

"""
>=  0x2265
<=  0x2264
> > 0x003E
< < 0x003C
!= 6 0x2262
==  0x2261
==> =) 0x21D2
<==> () 0x21D4
&& ^ 0x2227
|| _ 0x2228
^^  0x22BB
! : 0x00AC
- (unary minus) 􀀀 0x2212
\forall 8 0x2200
\exists 9 0x2203
integer Z 0x2124
real R 0x211D
boolean B 0x1D539
"""

class BaseTest(unittest.TestCase):

    def setUp(self):
        self.msg = Messaging(loadLibrary("messaging"))

    def tearDown(self):
        self.msg.deinit()
        del self.msg


class TestInit(BaseTest):

    def testInitShallSucced(self):
        self.msg.init()
        self.assertEqual(self.msg.getBufferCounters(), (4, 0))


class TestAllocation(BaseTest):

    def randomAllocate(self):
        allocations = random.randint(1, 4)
        buffers = []
        for i in range(allocations):
            buf = self.msg.allocateBuffer()
            self.assertTrue(buf)
            buffers.append(buf)
        print(buffers)
        for i in range(allocations):    # Random Release.
            buf = random.choice(buffers)
            print(buf)
            result = self.msg.releaseBuffer(buf)
            self.assertEqual(result, 0)
            buffers.remove(buf)

    def testAllocateShallSucced(self):
        self.msg.init()
        buf = self.msg.allocateBuffer()
        self.assertTrue(buf)
        self.assertEqual(self.msg.getBufferCounters(), (3, 1))

    def testAllocateAndReleaseOne(self):
        self.msg.init()
        buf = self.msg.allocateBuffer()
        self.assertTrue(buf)
        self.msg.releaseBuffer(buf)
        self.assertEqual(self.msg.getBufferCounters(), (4, 0))

    def testAllocateAndReleaseTwo(self):
        self.msg.init()
        buf0 = self.msg.allocateBuffer()
        self.assertTrue(buf0)
        buf1 = self.msg.allocateBuffer()
        self.assertTrue(buf1)
        self.msg.releaseBuffer(buf1)
        self.msg.releaseBuffer(buf0)
        self.assertEqual(self.msg.getBufferCounters(), (4, 0))

    def testAllocateAndReleaseMany(self):
        self.msg.init()
        for _ in range(1000):
            #self.randomAllocate()
            self.assertEqual(self.msg.getBufferCounters(), (4, 0))


class TestInitialisationErrors(BaseTest):

    def testAllocateBuffer(self):
        buf = self.msg.allocateBuffer()
        self.assertFalse(buf)
        cond = self.msg.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.ALLOCATE_BUFFER, Errors.UNINIT))

    def testAllocateBufferWrapper(self):
        buf = self.msg.allocateBufferWrapper()
        self.assertFalse(buf)
        cond = self.msg.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.ALLOCATE_BUFFER, Errors.UNINIT))

    def testReleaseBuffer(self):
        buf = None
        self.msg.releaseBuffer(buf)
        cond = self.msg.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.RELEASE_BUFFER, Errors.UNINIT))

    def testClearBuffer(self):
        buf = None
        self.msg.clearBuffer(buf)
        cond = self.msg.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.CLEAR_BUFFER, Errors.UNINIT))

    def testPost(self):
        buf = None
        self.msg.post(buf)
        cond = self.msg.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.POST, Errors.UNINIT))



class TestUtlStringFunctions(BaseTest):

    def testStrCpyEmptyStringDoesNothing(self):
        dest = create_string_buffer(120)
        src = create_string_buffer(b"")
        self.msg.utl.strCpy(dest, src)
        self.assertEqual(dest.value, "")

    def testStrCpyReallyCopies(self):
        dest = create_string_buffer(120)
        src = create_string_buffer(b"Hello, world!!!")
        self.msg.utl.strCpy(dest, src)
        self.assertEqual(dest.value, "Hello, world!!!")

    def testStrRevEmptyStringDoesNothing(self):
        src = create_string_buffer(b"")
        self.msg.utl.strRev(src)
        self.assertEqual(src.value, "")

    def testStrRevReallyWorks(self):
        src = create_string_buffer(b"Hello, world!!!")
        self.msg.utl.strRev(src)
        self.assertEqual(src.value, "Hello, world!!!"[::-1])

    def testStrRevMinimalReversal(self):
        src = create_string_buffer(b"AB")
        self.msg.utl.strRev(src)
        self.assertEqual(src.value, "BA")

    def testItoa1(self):
        result = self.msg.utl.itoa(0, 2)
        self.assertEqual(result, "0")

    def testItoa2(self):
        result = self.msg.utl.itoa(0xDEADBEEF, 16)
        self.assertEqual(result, "DEADBEEF")

    def testItoa3(self):
        result = self.msg.utl.itoa(4711, 10)
        self.assertEqual(result, "4711")

    def testItoa4(self):
        result = self.msg.utl.itoa(0xAA55, 2)
        self.assertEqual(result, "1010101001010101")

    def testItoa5(self):
        result = self.msg.utl.itoa(0xABBA, 16)
        self.assertEqual(result, "ABBA")

    def testRandom(self):
        self.msg.utl.randomize(4711)
        result = [self.msg.utl.random() for _ in range(32)]
        self.assertEqual(result, [26701, 4643, 12511, 4081, 12962, 32246, 28920, 2322, 12805, 21482,
                                  27822, 4008, 16381, 9415, 30906, 32284, 17572, 27729, 11141, 409,
                                  27925, 9538, 10099, 25706, 23699, 8457, 15859, 8721, 17530, 8005,
                                  14151, 8486]
        )

def loadLibrary(name):
    pf = sys.platform
    if pf.startswith("win"):
        ext = "dll"
    elif pf.startswith("linux"):
        ext = "so"
    elif pf.startswith("darwin"):
        ext = "dylib"
    dll = ctypes.CDLL("./{0}.{1}".format(name, ext))
    return dll


msg = Messaging(loadLibrary("messaging"))

def main():
    unittest.main()

if __name__ == '__main__':
    main()

