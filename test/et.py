
#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import ctypes
from ctypes import Structure, Union, c_uint8, c_int32, POINTER, byref, CFUNCTYPE, pointer, ARRAY
import enum

from common import ModuleIDs, FFI

ErrorType = namedtuple("ErrorType", "moduleId apiId errorCode")

EtCallback = CFUNCTYPE(None, c_uint8, c_uint8, c_uint8)

class KnxEt_ErrorConditionType(Structure):

    _fields_ = [
        ("ModuleId", c_uint8),
        ("ApiId", c_uint8),
        ("ErrorCode", c_uint8),
    ]
    _pack_ = 1


class Et(FFI):

    _functions_ = (
        ("KnxEt_SetCallback", None, [EtCallback]),
        ("KnxEt_GetErrorCondition", None, [POINTER(KnxEt_ErrorConditionType)]),
    )

    def getErrorCondition(self):
        cond = KnxEt_ErrorConditionType()
        self.KnxEt_GetErrorCondition(byref(cond))
        return ErrorType(cond.ModuleId, cond.ApiId, cond.ErrorCode)

    def setCallback(self, callback):
        ptr = EtCallback(callback)
        print("setCallback: {0}".format(ptr))
        self.KnxEt_SetCallback(ptr)
        #self.KnxEt_SetCallback(CFUNCTYPE(callback))

