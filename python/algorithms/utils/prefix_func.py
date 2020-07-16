def calculate(source):
    prefix_arr = [0] * len(source)
    for i in range(1, len(source)):
        j = prefix_arr[i - 1]
        while j > 0 and source[i] != source[j]:
            j = prefix_arr[j - 1]
        if source[i] == source[j]:
            j += 1
        prefix_arr[i] = j
    return prefix_arr
