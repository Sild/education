import time


def sort(data_array):
    startTime = time.time()
    tail_size = len(data_array)
    while( tail_size > 0 ):
        replace_index = 0
        max_value = data_array[0]
        for i in range(1, tail_size):
            if ( data_array[i] > max_value ):
                max_value = data_array[i]
                replace_index = i
        data_array[replace_index], data_array[tail_size - 1] = data_array[tail_size - 1], data_array[replace_index]
        tail_size -= 1
    print(__name__, "execution time:", time.time() - startTime)
    return data_array
