from multiprocessing import Process, Manager

resultAccumulator = Manager().list()

def doSmth(arg1, arg2, accumulator):
    print "smth-{}-{}".format(arg1, arg2)   
    accumulator.append("{}-{}".format(arg1, arg2))


l = [1, 2, 3, 4 ,5]
static = "static"
processPool = []
if __name__ == '__main__':
    for x in l:   
        p = Process(target=doSmth, args=(static, x, resultAccumulator))
        p.start()
        processPool.append(p)

for p in processPool:
    p.join()

print resultAccumulator
print "ended!"
