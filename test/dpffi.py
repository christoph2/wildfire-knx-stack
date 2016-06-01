#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import ctypes
import logging
#import random
import unittest
from ctypes import Structure, Union, c_uint8, c_int32, POINTER, byref, CFUNCTYPE, pointer, ARRAY, create_string_buffer
import enum

STATUS_T = ctypes.c_uint16

"""
dpffi (Dynamic Python FFF)
-----

dpffi ist inspired by LuaJITs FFI module.....
"""

Operators = namedtuple('Operators', 'a u')

OPS = (
    (">=",              u"\u2265"), # ≥
    ("<=",              u"\u2264"), # ≤
    (">",               u"\u003E"), # >
    ("<",               u"\u003C"), # <
    ("!=",              u"\u2262"), # ≢
    ("==",              u"\u2261"), # ≡
    ("==>",             u"\u21D2"), # ⇒
    ("<==>",            u"\u21D4"), # ⇔
    ("&&",              u"\u2227"), # ∧
    ("^^",              u"\u22BB"), # ⊻
    ("!",               u"\u00AC"), # ¬
    ("- (unary minus)", u"\u2212"), # −
    ("\\forall",        u"\u2200"), # ∀
    ("\\exists",        u"\u2203"), # ∃
    ("integer",         u"\u2124"), # ℤ
    ("real",            u"\u211d"),  # ℝ
    ("boolean",         u"\u1d539"), # ᵓ9
)

for a, u in OPS:
    #print("{0} ==> {1}".format(a, u))
    print(u)