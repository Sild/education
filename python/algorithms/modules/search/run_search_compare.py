from modules import *
from utils import fs

source = fs.read_file_as_string("../resources/search/search_source.txt")
target = fs.read_file_as_string("../resources/search/search_target.txt")



print("result:", linear_search.search(source, target))
print("result:", kmp_search.search(source, target))
print("result:", bm_search.search(source, target))
