import time


def sort(data_array):
    startTime = time.time()

    for i in range(0, len(data_array) - 1):
        continue_required = False
        for j in range(0, len(data_array) - i - 1):
            if (data_array[j] > data_array[j + 1]):
                data_array[j], data_array[j + 1] = data_array[j + 1], data_array[j]
                continue_required = True
        if( continue_required == False ):
            break
    print(__name__, "execution time:", time.time() - startTime)
    return data_array
