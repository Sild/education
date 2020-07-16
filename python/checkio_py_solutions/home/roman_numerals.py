def checkio(data):
    # print(data)
    data = str(data)[::-1]
    transform_map = [(1, "I"), (5, "V"), (1, "X"), (5, "L"), (1, "C"), (5, "D"), (1, "M")]
    result = []
    map_index_shift = 0
    for index in range(len(data)):
        transform_set = transform_map[index + map_index_shift]
        map_index = index + map_index_shift
        count = int(data[index]) // transform_set[0]
        # print(data[index], count, transform_set, map_index, map_index_shift)
        
        partition = []
        if count == 4:
            partition.append(transform_set[1])
            partition.append(transform_map[map_index + 1][1])
        elif count == 9:
            partition.append(transform_set[1])
            partition.append(transform_map[map_index + 2][1])
        elif count > 3:
            partition += [transform_map[map_index + 1][1]] + [transform_set[1]] * (count - 5)
        elif count == 0:
            pass
        else:
            partition += [transform_set[1]] * count

        map_index_shift += 1
        result = partition + result
        print(result)
    return "".join(result)

if __name__ == '__main__':
    #These "asserts" using only for self-checking and not necessary for auto-testing
    assert checkio(6) == 'VI', '6'
    assert checkio(10) == 'X', '10'
    assert checkio(76) == 'LXXVI', '76'
    assert checkio(499) == 'CDXCIX', '499'
    assert checkio(2387) == 'MMCCCLXXXVII', '2387'
    assert checkio(3888) == 'MMMDCCCLXXXVIII', '3888'