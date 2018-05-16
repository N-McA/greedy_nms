
# distutils: language = c++
# distutils: sources = ./box_clipper/greedy_nms_impl.cpp

import cython

import numpy as np
cimport numpy as np


cdef extern void hello()
cdef extern void cpp_greedy_nms(float *coords, int *output, int length, float thresh)


def hello_cpp():
    '''
    Prints hello from C++, hopefully.
    '''
    print("Hello")
    hello()


@cython.boundscheck(False)
@cython.wraparound(False)
def greedy_nms_worker(
        np.ndarray[float, ndim=1, mode="c"] input not None, 
        np.ndarray[int, ndim=1, mode="c"] output not None,
        float threshold
    ):

    length = len(input)
    cpp_greedy_nms(&(input[0]), &(output[0]), length, threshold)


def greedy_nms(input_boxes, threshold):
    assert len(input_boxes.shape) == 3
    assert input_boxes.shape[1:] == (4, 2)
    
    flat_boxes = input_boxes.reshape([-1])

    result = np.zeros(len(input_boxes), dtype=np.int32)

    assert len(result) * 8 == len(flat_boxes)

    greedy_nms_worker(flat_boxes, result, threshold)
    return result



