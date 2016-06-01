#!/usr/bin/env python
# -*- coding: utf-8 -*-

#import ctypes
#import unittest


from collections import namedtuple
import ctypes
import logging
#import random
import unittest
from ctypes import Structure, Union, c_uint8, c_int32, POINTER, byref, CFUNCTYPE, pointer, ARRAY
import enum

#from common import ModuleIDs, FFI
#from et import Et
#from utl import Utl

print("Trying to load dll...")
msg = ctypes.cdll.LoadLibrary("messaging")
print("dll: {0!r}".format(msg))

def main():
    unittest.main()

if __name__ == '__main__':
    main()

