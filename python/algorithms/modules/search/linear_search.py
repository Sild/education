import time
def search(source, target):
    startTime = time.time()
    for i in range(0, len(source)):
        j = 0
        while (j < len(target)):
            if(source[i+j] != target[j]):
                break
            j+=1
        if(j == len(target)):
            print(__name__, "execution time:", time.time() - startTime)
            return i
    return -1