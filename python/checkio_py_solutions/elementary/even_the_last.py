def checkio(array):
    if len(array) < 1:
        return 0
    evenArray = [v for k, v in enumerate(array) if k % 2 == 0]
    return sum(evenArray)*array[-1]


#These "asserts" using only for self-checking and not necessary for auto-testing
if __name__ == '__main__':
    assert checkio([0, 1, 2, 3, 4, 5]) == 30, "(0+2+4)*5=30"
    assert checkio([1, 3, 5]) == 30, "(1+5)*5=30"
    assert checkio([6]) == 36, "(6)*6=36"
    assert checkio([]) == 0, "An empty array = 0"