import time
from utils import prefix_func

def search(source, target):
    startTime = time.time()
    prefix_arr = prefix_func.calculate(target + "$" + source)
    target_length = len(target)
    total_length = len(target) + 1 + len(source)
    prefix_val = 0
    i = 2*target_length
    while(i < total_length and prefix_val < target_length):
        prefix_val = prefix_arr[i]
        i += target_length - prefix_val

    print(__name__, "execution time:", time.time() - startTime)
    if(i >= total_length):
        return -1
    return (i - 2*target_length)
