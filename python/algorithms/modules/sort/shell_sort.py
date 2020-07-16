import time

def sort(data_array):
    startTime = time.time()
    size = len(data_array)
    seq = sedjvic_seq(size)
    # print(seq)
    for diff in reversed(seq):
        for i in range(0, len(data_array) - diff):
            continue_required = False
            for j in range(0, len(data_array) - i - diff):
                if (data_array[j] > data_array[j + diff]):
                    data_array[j], data_array[j + diff] = data_array[j + diff], data_array[j]
                    if ( diff == 1 ):
                        continue_required = True
            if( continue_required == False):
                break
    print(__name__, "execution time:", time.time() - startTime)
    return data_array


def sedjvic_seq(array_size):
    i = 0
    seq = []
    while True:
        element = int(get_sedjvic_element(i))
        i += 1
        if( 3 * element > array_size ):
            break
        seq.append(element)
    return seq


def get_sedjvic_element(num):
    if(num % 2 == 0):
        return 9*pow(2, num) - 9*pow(2, num/2) + 1
    else:
        return 8*pow(2, num) - 6*pow(2, (num + 1)/2) + 1
