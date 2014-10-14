#!/usr/bin/env/ python
# -*- coding: utf-8 -*-

__version__ = "0.9.2"
__description__ = "Wildfire - The Open Source KNX/EIB-Protocol Stack."

__copyright__ = """
    Wildfire - The Open Source KNX/EIB-Protocol Stack.

   (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
                                        cpu12.gems@googlemail.com>

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
"""

from os.path import join
from distutils.core import setup, Extension

def sourceFile(name):
  return join('..', 'src', '%s.c' % name)

def portFile(name):
  return join('..', 'port', 'win32', '%s.c' % name)

def makeExtension(name):
        #extname = name
        name = '_%s' % name
        nt = name[ 1 : ].title()
        swig_opts = [
                #'-c++',
                #'-features autodoc',
                #'-MD',
                #'-O',
                '-I ..\\inc',
                '-modern',
                '-modernargs',
                '-verbose',
                #'-debug-tmsearch',
                '-debug-tmused',
                #'-keyword'
        ]
        #rel = join(*rel.split())
        sources = [
                '%s.i' % nt,
                join('..', 'src', 'link-layer', 'uart_bif.c'),
                sourceFile('knx_utl'),
                sourceFile('knx_debug'),
                sourceFile('knx_et'),
                sourceFile('knx_timer'),
                sourceFile('Port'),
                sourceFile('knx_disp'),
                sourceFile('knx_messaging'),
                sourceFile('knx_tlg'),
                sourceFile('knx_tlc'),
                sourceFile('StateMachineKNX'),
                sourceFile('knx_nl'),
                sourceFile('knx_alg'),
                sourceFile('knx_alm'),
                sourceFile('knx_address'),
                sourceFile('Appl'),
                sourceFile('knx_ios'),
                sourceFile('knx_ail'),
                sourceFile('knx_lsm'),
                sourceFile('knx_conversion'),
                sourceFile('knx_sys_objs'),
                sourceFile('knx_sched'),
                'dummy.c',
                portFile('Win_Timer'),
                portFile('Win_Utils'),
                portFile("Port_Serial"),
                portFile('Win_SerialProxy'),
        ]
        zmq_include = join("C:\\","Program Files (x86)", "ZeroMQ 4.0.4", "include")
        print zmq_include

        include_dirs = ['.', join('..', 'inc'), join('..', 'config'), zmq_include]
        define_macros = [('SWIG', '1')]
        # define_macros : [(name : string, value : string|None)]
        # library_dirs : [string]
        libraries = ["libzmq-v120-mt-gd-4_0_4"]
        library_dirs = [r"C:\Program Files (x86)\ZeroMQ 4.0.4\lib"]
        # extra_compile_args : [string]
        # extra_link_args : [string

        return Extension(name = name, sources = sources, swig_opts = swig_opts,
            include_dirs = include_dirs, define_macros = define_macros,
            library_dirs = library_dirs, libraries = libraries
        )

setup(
  name = "pyKStack",
  version = "0.1",
  author = "Christoph Schueler",
  description = "Wildfire - The Open Source KNX/EIB-Protocol Stack.",
  ext_modules = [
      makeExtension('kstackf'),
  ],
)

