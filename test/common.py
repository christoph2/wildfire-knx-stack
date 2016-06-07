#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import sys
import enum

from wildfire.ffi import loadLibrary

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
    SERIAL   = 49


class BaseTest(unittest.TestCase):

    CLASS = None
    DLL = ""

    def setUp(self):
        self.obj = self.CLASS(loadLibrary(self.DLL))

    def tearDown(self):
        self.obj.deinit()
        del self.obj

