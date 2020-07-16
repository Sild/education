import time


def sort(data_array):
    startTime = time.time()
    left = 0
    right = len(data_array) - 1
    while left <= right:
        continue_required = False
        for i in range(left, right):
            if (data_array[i] > data_array[i + 1]):
                data_array[i], data_array[i + 1] = data_array[i + 1], data_array[i]
                continue_required = True
        right -=1
        for i in range(right, left, -1):
            if (data_array[i] < data_array[i - 1]):
                data_array[i], data_array[i - 1] = data_array[i - 1], data_array[i]
                continue_required = True
        left +=1
        if (continue_required == False):
            break
    print(__name__, "execution time:", time.time() - startTime)
    return data_array
