import time
from utils import prefix_func


def search(source, target):
    startTime = time.time()
    stop_map = stop_char_map(target)
    suffix_arr = suffix_table(target)
    # print(stop_map)
    # print(suffix_arr)
    i = 0
    while i < (len(source) - len(target)):
        j = len(target) - 1
        while j >= 0 and target[j] == source[i + j]:
            j = j - 1
        if (j < 0):
            print(__name__, "execution time:", time.time() - startTime)
            return i
        shift_by_stop = j - stop_map.get(source[i + j], -1)
        shift_by_suffix = suffix_arr[j+1]
        # print("step: ", i, "|length: ", j, "|target: ", target[j], "|source: ", source[i+j], "|by stop: ", shift_by_stop, "|by suffix: ", shift_by_suffix)
        i += max(shift_by_stop, shift_by_suffix)
    print(len(source), len(target))
    return -1


def suffix_table(str):
    str_len = len(str)
    prefix = prefix_func.calculate(str)
    # print("prefix: ", prefix)
    prefix_reverse = prefix_func.calculate(str[::-1])
    # print("prefix_reverse: ", prefix_reverse)
    suffix_arr = [0] * (str_len + 1)
    for i in range(0, str_len + 1):
        suffix_arr[i] = str_len - prefix[str_len - 1]
    for i in range(1, str_len):
        index = str_len - prefix_reverse[i]
        suffix_arr[index] = min(suffix_arr[index], i - prefix_reverse[i - 1])
    return suffix_arr


def stop_char_map(str):
    map = {}
    for i in range(0, len(str) - 1):
        map[str[i]] = i
    return map
