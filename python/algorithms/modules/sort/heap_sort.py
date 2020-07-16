import time


def sort(data_array):
    startTime = time.time()
    size = len(data_array)
    for i in range(int(size / 2) - 1, -1, -1):
        downHeap(data_array, i, size)

    for i in range(0, size):
        data_array[0], data_array[size - 1 - i] = data_array[size - 1 - i], data_array[0]
        downHeap(data_array, 0, size - 1 - i)
    print(__name__, "execution time:", time.time() - startTime)
    return data_array


def downHeap(data_array, index, size):
    new_item = data_array[index]
    while (index <= int(size / 2) - 1):
        child_index = 2 * index + 1
        if (child_index < (size - 2) and data_array[child_index] < data_array[child_index + 1]):
            child_index += 1
        if (new_item > data_array[child_index]):
            break
        else:
            data_array[index] = data_array[child_index]
            index = child_index
    data_array[index] = new_item
