#!/bin/env/python

from distutils.core import setup, Extension
import os
from setuptools import find_packages
from glob import glob

def packagez(base):
    return  ["%s%s%s" % (base, os.path.sep, p) for p in find_packages(base)]

setup(
    name = 'K-Stack TPUART driver',
    version = '0.1.0',
    description = "Platform independent TPUART driver",
    author = 'Christoph Schueler',
    author_email = 'cpu12.gems@googlemail.com',
    url = 'https://www.github.com/Christoph2/K-Stack',
    packages = packagez('kstack'),
    install_requires = ['pyserial', 'pyzmq >= 14'],
    entry_points = {
	    'console_scripts': [
            'tpuartAdapter = kstack.busaccess.tpuart.tpuartAdapter:main'
        ],
    }
)

