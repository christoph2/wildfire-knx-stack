#!/usr/bin/env python
# -*- coding: utf-8 -*-

#from collections import namedtuple
#import ctypes
#import logging
#from ctypes import Structure, Union, c_uint8, c_int32, POINTER, byref, CFUNCTYPE, pointer, ARRAY
import enum

#STATUS_T = ctypes.c_uint16

class ModuleIDs(enum.IntEnum):
    MSG      = 1
    AIL      = 2
    ALG      = 3
    ALM      = 4
    TLC      = 5
    TLG      = 6
    TLSM     = 7
    NL       = 8
    DL       = 9
    UART_BIF = 48


def defaultChecker(result, func, args):
    #print("Checker - func: {0} args: {1} result: {2} ".format(func.__name__, args, result))
#    if not result:
#        errorCode = win32api.GetLastError()
#        errorMsg = win32api.FormatMessage(errorCode)
#        raise ctypes.WinError(errorCode, "{0}: {1}".format(func.__name__, errorMsg))
    return args

class FFI(object):

    def __init__(self, dll = None, parent = None):
        if dll is None and parent is None:
            raise TypeError("dll and parent are None.")
        if dll is None:
            dll = parent.dll
        #print("FFI::__init__ [{0!r}]".format(dll))
        self.dll = dll
        self.loadFunctions()
##
##    def returnValue(self, value):
##        if value == -1:
##            errno = self.parent.base.errorNumber()
##            errstr = self.parent.base.errorString(errno)
##            raise MSPError("{0} -- {1}.".format(errno, errstr), errno = errno)
##        elif value != 0:
##            self.warn("Unexpected return value: {0}".format(value))
##        return StatusCode(value)
##

    def functionFactory(self, library, functionName, resultType, argTypes, errorChecker = None):
        if not errorChecker:
            errorChecker = defaultChecker
        func = getattr(library, functionName)
        # These functions do not return 'STATUS_T'.
        func.errcheck = errorChecker
        func.restype = resultType #self.returnValue
        func.argtypes = argTypes
        return func

    def loadFunctions(self):
        for fun in self._functions_:
            addFunction = True
            if len(fun) == 3:
                functionName, resultType, argTypes = fun
                if argTypes == []:
                    argTypes = None
                try:
                    function = self.functionFactory(self.dll, functionName, resultType, argTypes)
                except AttributeError as e:
                    print("Error: {0}.".format(str(e)))
                    addFunction = False
            elif len(fun) == 4:
                functionName, resultType, argTypes, checker = fun
                if argTypes == []:
                    argTypes = None
                try:
                    function = self.functionFactory(self.dll, functionName, resultType, argTypes, checker)
                except AttributeError:
                    print("Error: {0}.".format(str(e)))
                    addFunction = False
            else:
                raise AttributeError("wrong length of function definition '{0}'.".format(fun))
            if addFunction:
                setattr(self, functionName, function)

def nullPointerChecker(result, func, args):
    if result is None:
        errorCode = win32api.GetLastError()
        if errorCode:
            errorMsg = win32api.FormatMessage(errorCode)
            raise ctypes.WinError(errorCode, "{0}: {1}".format(func.__name__, errorMsg))
    return args

