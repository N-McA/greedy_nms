
import numpy as np


def test_1():
    import greedy_nms
    greedy_nms.hello_cpp()
    print("Test 1 Passed")


def test_2():
    import greedy_nms
    b = np.array([
        [0., 0.],
        [0., 1.],
        [1., 1.],
        [1., 0.],
    ])

    boxes = []
    for _ in range(10):
        s = np.random.uniform(0, 10, size=2)
        t = np.random.uniform(0, 10, size=2)
        boxes.append(b*s + t)

    boxes = np.array(boxes).astype(np.float32)
    print(greedy_nms.greedy_nms(boxes, 1.0))
    print("Test 2 Passed")


if __name__ == '__main__':
    test_1()
    test_2()
