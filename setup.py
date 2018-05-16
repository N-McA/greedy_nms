

from distutils.core import setup
from Cython.Build import cythonize
import numpy
from distutils.extension import Extension

setup(ext_modules=cythonize([Extension(
   name="greedy_nms",
   sources=["box_clipper/greedy_nms.pyx"],
   include_dirs=[numpy.get_include()],
   language="c++",
   extra_compile_args=["-std=c++11"],
   extra_link_args=["-std=c++11"],
)]))
