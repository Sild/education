import time


def sort_recursive(data_array):
    startTime = time.time()
    data_array = mergeSort(data_array)
    print(__name__, "recursive execution time:", time.time() - startTime)
    return data_array

def sort_stack(data_array):
    startTime = time.time()
    #TODO undone
    # data_array = mergeSort(data_array)
    # print(__name__, "stack execution time:", time.time() - startTime)
    return data_array

def mergeSort(data_array):
    if (len(data_array) > 1):
        a = data_array[:int(len(data_array)/ 2)]
        b = data_array[int(len(data_array)/2):]
        return merge(mergeSort(a), mergeSort(b))
    return data_array


def merge(arr1, arr2):
    # print("to:", len(arr1) + len(arr2))
    i = j = 0
    merged = []
    while (i < len(arr1) and j < len(arr2)):
        if (arr1[i] <= arr2[j]):
            merged.append(arr1[i])
            i += 1
        else:
            merged.append((arr2[j]))
            j += 1
    while i < len(arr1):
        merged.append(arr1[i])
        i += 1
    while j < len(arr2):
        merged.append(arr2[j])
        j += 1
    # print("out:", len(merged))
    return merged
