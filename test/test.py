#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import ctypes
import logging
#import random
import unittest
from ctypes import Structure, Union, c_uint8, c_int32, POINTER, byref, CFUNCTYPE, pointer, ARRAY, create_string_buffer
import enum

from common import ModuleIDs, FFI
from et import Et
from utl import Utl

STATUS_T = ctypes.c_uint16

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

class Services(enum.IntEnum):
    INIT                      = 0x00
    ALLOCATE_BUFFER           = 0x01
    ALLOCATE_BUFFER_WRAPPER   = 0x02
    RELEASE_BUFFER            = 0x03
    CLEAR_BUFFER              = 0x04
    POST                      = 0x05
    GET                       = 0x06
    SET_LEN                   = 0x07
    GET_LEN                   = 0x08
    SET_ROUTING_COUNT         = 0x09
    GET_ROUTING_COUNT         = 0x0a
    GET_BUFFER_ADDRESS        = 0x0b
    GET_BUFFER_NUMBER         = 0x0c
    CLEAR_MESSAGE_BUFFER      = 0x0d


class Errors(enum.IntEnum):
    UNINIT                        = 0x01
    NULL_PTR                      = 0x02
    NO_BUFFER_AVAIL               = 0x03
    INVALID_BUFFER                = 0x04
    NOT_ALLOCATED                 = 0x05


class StatusCode(enum.IntEnum):
    STATUS_ERROR = -1
    STATUS_OK = 0

MSG_LEN = 22
MAX_APDU_LEN = 14
MAX_PROP_DATA_LEN = 10


class MyStructure(Structure):

    def __repr__ (self):
        fields = self._fields_
        result = []
        result.append("{0} {{".format(self.__class__.__name__))
        for name,_ in fields:
            result.append("    {0} = {1!r}".format(name, getattr(self, name)))
        result.append("}")
        return '\n'.join(result)

    __str__ = __repr__


class KnxMsg_StandardFrameType(Structure):
    _fields_ = [
        ("ctrl", c_uint8),
        ("source", c_uint8 * 2),
        ("dest", c_uint8 * 2),
        ("npci", c_uint8),
        ("tpci", c_uint8),
        ("apci", c_uint8),
        ("data", c_uint8 * MAX_APDU_LEN),
    ]
    _pack_ = 1


class KnxMsg_PropertyFrameType(Structure):
    _fields_ = [
        ("ctrl", c_uint8),
        ("source", c_uint8 * 2),
        ("dest", c_uint8 * 2),
        ("npci", c_uint8),
        ("tpci", c_uint8),
        ("apci", c_uint8),
        ("obj_id", c_uint8),
        ("prop_id", c_uint8),
        ("num_elems", c_uint8),
        ("start_index", c_uint8),
        ("data", c_uint8 * MAX_PROP_DATA_LEN),
    ]
    _pack_ = 1


class KnxMsg_PollingFrameType(Structure):
    _fields_ = [
        ("ctrl", c_uint8),
        ("source", c_uint8 * 2),
        ("poll_addr", c_uint8 * 2),
        ("num_slots", c_uint8),
        ("slot", c_uint8 * MAX_APDU_LEN),
    ]
    _pack_ = 1


class MessageUnion(Union):
    _fields_ = [
        ("raw", c_uint8 * MSG_LEN),
        ("standard", KnxMsg_StandardFrameType),
        ("property", KnxMsg_PropertyFrameType),
        ("polling", KnxMsg_PollingFrameType),
    ]
    _pack_ = 1


class KnxMsg_Buffer(Structure):
    _fields_ = [
        ("next", c_uint8),
        ("len", c_uint8),
        ("service", c_uint8),
        ("sap", c_uint8),
        ("status", c_uint8),
        ("msg", MessageUnion),
    ]
    _pack_ = 1

class KnxMsg_DebugBufferCounters(Structure):
    _fields_ = [
        ("used", c_uint8),
        ("free", c_uint8),
    ]
    _pack_ = 1


KnxMsg_Buffer_Ptr = POINTER(KnxMsg_Buffer)
CounterType = namedtuple("CounterType", "free used")

class Messaging(FFI):

    _functions_ = (
        ("KnxMsg_Init", None, []),
        ("KnxMsg_Deinit", None, []),
        ("KnxMsg_AllocateBuffer", STATUS_T, [POINTER(KnxMsg_Buffer_Ptr)]),
        ("KnxMsg_AllocateBufferWrapper", KnxMsg_Buffer_Ptr, []),
        ("KnxMsg_ReleaseBuffer", STATUS_T, [KnxMsg_Buffer_Ptr]),
        ("KnxMsg_ClearBuffer", STATUS_T, [KnxMsg_Buffer_Ptr]),
        ("KnxMsg_Post", STATUS_T, [KnxMsg_Buffer_Ptr]),
        ("KnxMsg_Get", KnxMsg_Buffer_Ptr, [c_uint8]),
        ("KnxMsg_SetLen", None, [KnxMsg_Buffer_Ptr, c_uint8]),
        ("KnxMsg_GetLen", c_uint8, [KnxMsg_Buffer_Ptr]),
        ("KnxMsg_GetRoutingCount", c_uint8, [KnxMsg_Buffer_Ptr]),
        ("KnxMsg_SetRoutingCount", None, [KnxMsg_Buffer_Ptr]),
        ("KnxMsg_SetRoutingCtrl", None, [KnxMsg_Buffer_Ptr, c_uint8]),
        ("KnxMsg_DebugGetBufferCounters", None, [POINTER(KnxMsg_DebugBufferCounters)]),
        ("KnxMsg_DebugGetBuffers", None, [POINTER(KnxMsg_Buffer * 4)]),

    )

    def __init__(self, dll = None, parent = None):
        super(Messaging, self).__init__(dll, parent)
        self.utl = Utl(parent = self)
        self.et = Et(parent = self)

    def init(self):
        self.KnxMsg_Init()

    def deinit(self):
        self.KnxMsg_Deinit()

    def getBufferCounters(self):
        counters = KnxMsg_DebugBufferCounters()
        self.KnxMsg_DebugGetBufferCounters(byref(counters))
        return CounterType(counters.free, counters.used)

    def getBuffers(self):
        ptr = (KnxMsg_Buffer * 4)()
        self.KnxMsg_DebugGetBuffers(pointer(ptr))
        return ptr

    def allocateBuffer(self):
        ptr = KnxMsg_Buffer_Ptr()
        result = self.KnxMsg_AllocateBuffer(byref(ptr))
        if ptr:
            return ptr.contents
        else:
            return None

    def allocateBufferWrapper(self):
        return self.KnxMsg_AllocateBufferWrapper()

    def releaseBuffer(self, buffer):
        return self.KnxMsg_ReleaseBuffer(buffer)

    def clearBuffer(self, buffer):
        return self.KnxMsg_ClearBuffer(buffer)

    def post(self, buffer):
        return self.KnxMsg_Post(buffer)

    def get(self, layer):
        buffer =  self.KnxMsg_Get(layer)
        print("Buffah {0} - Null? {1}".format(buffer, not bool(buffer)))
        if buffer:
            return buffer.contents
        else:
            return None
        return buffer


class BaseTest(unittest.TestCase):

    def setUp(self):
        self.msg = Messaging(ctypes.cdll.LoadLibrary("messaging"))

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


import os

#os.chdir(r"C:\projekte\csProjects\k-ps\test\messaging")

dll = None

for ext in ("so", "dll", "dylib"):
  try:
    dll = ctypes.CDLL("./messaging.{0}".format(ext))
  except Exception as e:
    pass
  else:
    print("{0} - [{1}]".format(dll, ext))

msg = Messaging(dll)

def main():
    unittest.main()
    """
    # A reasonable way to unit-test C-code with the help of Python.

    Always remember: unit-tests are an asset not a needless effort!
    """

if __name__ == '__main__':
    main()

