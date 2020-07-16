from modules import *
from utils import fs

short = fs.read_file_as_int_array("../../resources/sort/unsorted_short.txt")
long = fs.read_file_as_int_array("../../resources/sort/unsorted_long.txt")

# print("result:", bubble_sort.sort(short[:]), "\n")
# print("result:", shaker_sort.sort(short[:]), "\n")
# print("result:", selection_sort.sort(short[:]), "\n")
# print("result:", insertion_sort.sort(short[:]), "\n")
# print("result:", shell_sort.sort(short[:]), "\n")
# print("result:", heap_sort.sort(short[:]), "\n")
# print("result:", quick_sort.recursive(short[:]), "\n")
# print("result:", quick_sort.recursive_improved(short[:]), "\n")
print("result:", quick_sort.stack_emulation(short[:]), "\n")
print("result:", merge_sort.sort_recursive(short[:]), "\n")


# bubble_sort.sort(long[:])
# shaker_sort.sort(long[:])
# selection_sort.sort(long[:])
# shell_sort.sort(long[:])
# heap_sort.sort(long[:])
# quick_sort.recursive(long[:])
# quick_sort.recursive_improved(long[:])
quick_sort.stack_emulation(long[:])
merge_sort.sort_recursive(long[:])