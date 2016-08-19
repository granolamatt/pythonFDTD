from distutils.core import setup, Extension

module1 = Extension('pythonFDTD',
                    sources = ['pythonFDTD.cc'])

setup (name = 'pythonFDTD',
       version = '1.0',
       description = 'This is a c++ workflow demo',
       ext_modules = [module1])
