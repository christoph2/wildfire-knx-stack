
from setuptools import setup, find_packages
from codecs import open
from os import path

here = path.abspath(path.dirname(__file__))

#with open(path.join(here, 'README.rst'), encoding='utf-8') as f:
#    long_description = f.read()

setup(
    name = 'wildfire',
    version = '1.0',
    description = 'Python binding for Wildfire KNX protocol stack.',
#    long_description = long_description,
#    url = 'https://github.com/christoph2/ar4formula',
    author = 'Christoph Schueler',
    author_email = 'cpu12.gems@googlemail.com',
    license = 'GPL',
    classifiers = [
        'Development Status :: 4, // 3 - Alpha, 4 - Beta, 5- Production Stable',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
        'License :: OSI Approved :: GPL License',
        'Programming Language :: Python :: 2.7',
    ],

    keywords = 'microcontroller [development]',
#    packages = find_packages(exclude = ['contrib', 'docs', 'tests']),
    packages = ['wildfire'],
    install_requires = [u'enum34'],

    extras_require = {
        'dev': ['check-manifest'],
        'test': ['coverage'],
    },

#    entry_points={
#        'console_scripts': [
#            'sample=sample:main',
#        ],
#    },
)
