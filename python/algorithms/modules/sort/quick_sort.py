import time
from . import insertion_sort

RECURSIVE_LIMIT = 5


def recursive(data_array):
    startTime = time.time()
    __recursive_phase__(data_array, 0, len(data_array) - 1)
    print(__name__, "recursive execution time:", time.time() - startTime)
    return data_array


def recursive_improved(data_array):
    startTime = time.time()
    __recursive_phase__(data_array, 0, len(data_array) - 1)
    insertion_sort.sort(data_array)
    print(__name__, "recursive improved execution time:", time.time() - startTime)
    return data_array


def stack_emulation(data_array):
    startTime = time.time()
    l_stack = [0]
    r_stack = [len(data_array) - 1]
    while (len(l_stack) > 0):
        L = start = l_stack.pop()
        R = end = r_stack.pop()
        guard = data_array[int((start + end) / 2)]
        while True:
            while data_array[start] < guard:
                start += 1
            while data_array[end] > guard:
                end -= 1
            if (start <= end):
                data_array[start], data_array[end] = data_array[end], data_array[start]
                start += 1
                end -= 1
            if (start > end):
                break
        if(start < R):
            l_stack.append(start)
            r_stack.append(R)
        if(end > L):
            l_stack.append(L)
            r_stack.append(end)
    print(__name__, "stack emulation execution time for recursive:", time.time() - startTime)
    return data_array


def __recursive_phase__(data_array, start, end):
    L = start
    R = end
    guard = data_array[int((end + start) / 2)]
    while True:
        while data_array[start] < guard:
            start += 1
        while guard < data_array[end]:
            end -= 1
        if (start <= end):
            data_array[start], data_array[end] = data_array[end], data_array[start]
            start += 1
            end -= 1
        if (start > end):
            break
    if (start < R):
        __recursive_phase__(data_array, start, R)
    if (end > L):
        __recursive_phase__(data_array, L, end)


def __recursive_phase_improved__(data_array, start, end):
    L = start
    R = end
    guard = data_array[int((end + start) / 2)]
    while True:
        while data_array[start] < guard:
            start += 1
        while guard < data_array[end]:
            end -= 1
        if (start <= end):
            data_array[start], data_array[end] = data_array[end], data_array[start]
            start += 1
            end -= 1
        if (start > end):
            break
    if ((start + RECURSIVE_LIMIT) < R):
        __recursive_phase__(data_array, start, R)
    if ((end - RECURSIVE_LIMIT) > L):
        __recursive_phase__(data_array, L, end)
