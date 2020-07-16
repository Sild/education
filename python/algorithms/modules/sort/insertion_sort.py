import time


def sort(data_array):
    startTime = time.time()
    for i in range(1, len(data_array)):
        for j in range(i -1, -1, -1):
            if ( data_array[j] < data_array[j - 1] ):
                data_array[j], data_array[j - 1] = data_array[j - 1], data_array[j]
            else:
                break
    print(__name__, "execution time:", time.time() - startTime)
    return data_array
