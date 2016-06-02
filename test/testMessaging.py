#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest

from common import ModuleIDs, BaseTest
from msg import Messaging, Services, Errors

class TestInit(BaseTest):

    CLASS = Messaging
    DLL = "messaging"

    def testInitShallSucced(self):
        self.obj.init()
        self.assertEqual(self.obj.getBufferCounters(), (4, 0))


class TestAllocation(BaseTest):

    CLASS = Messaging
    DLL = "messaging"

    def randomAllocate(self):
        allocations = random.randint(1, 4)
        buffers = []
        for i in range(allocations):
            buf = self.obj.allocateBuffer()
            self.assertTrue(buf)
            buffers.append(buf)
        print(buffers)
        for i in range(allocations):    # Random Release.
            buf = random.choice(buffers)
            print(buf)
            result = self.obj.releaseBuffer(buf)
            self.assertEqual(result, 0)
            buffers.remove(buf)

    def testAllocateShallSucced(self):
        self.obj.init()
        buf = self.obj.allocateBuffer()
        self.assertTrue(buf)
        self.assertEqual(self.obj.getBufferCounters(), (3, 1))

    def testAllocateAndReleaseOne(self):
        self.obj.init()
        buf = self.obj.allocateBuffer()
        self.assertTrue(buf)
        self.obj.releaseBuffer(buf)
        self.assertEqual(self.obj.getBufferCounters(), (4, 0))

    def testAllocateAndReleaseTwo(self):
        self.obj.init()
        buf0 = self.obj.allocateBuffer()
        self.assertTrue(buf0)
        buf1 = self.obj.allocateBuffer()
        self.assertTrue(buf1)
        self.obj.releaseBuffer(buf1)
        self.obj.releaseBuffer(buf0)
        self.assertEqual(self.obj.getBufferCounters(), (4, 0))

    def testAllocateAndReleaseMany(self):
        self.obj.init()
        for _ in range(1000):
            #self.randomAllocate()
            self.assertEqual(self.obj.getBufferCounters(), (4, 0))


class TestInitialisationErrors(BaseTest):

    CLASS = Messaging
    DLL = "messaging"

    def testAllocateBuffer(self):
        buf = self.obj.allocateBuffer()
        self.assertFalse(buf)
        cond = self.obj.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.ALLOCATE_BUFFER, Errors.UNINIT))

    def testAllocateBufferWrapper(self):
        buf = self.obj.allocateBufferWrapper()
        self.assertFalse(buf)
        cond = self.obj.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.ALLOCATE_BUFFER, Errors.UNINIT))

    def testReleaseBuffer(self):
        buf = None
        self.obj.releaseBuffer(buf)
        cond = self.obj.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.RELEASE_BUFFER, Errors.UNINIT))

    def testClearBuffer(self):
        buf = None
        self.obj.clearBuffer(buf)
        cond = self.obj.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.CLEAR_BUFFER, Errors.UNINIT))

    def testPost(self):
        buf = None
        self.obj.post(buf)
        cond = self.obj.et.getErrorCondition()
        self.assertEqual(cond, (ModuleIDs.MSG, Services.POST, Errors.UNINIT))

def main():
    unittest.main()

if __name__ == '__main__':
    main()


