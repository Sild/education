#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cstring>
#include <charconv>

void __attribute__((noinline)) fillVector(std::vector<int>& vector, size_t elementsCount) {
	for (int i = 0; i < elementsCount; ++i)
	{
		vector.emplace_back(i);
	}
};

void __attribute__((noinline)) fillVectorFast(std::vector<int>& vector, size_t elementsCount) {
	vector.reserve(elementsCount);
	for (int i = 0; i < elementsCount; ++i)
	{
		vector.emplace_back(i);
	}
};

size_t __attribute__((noinline)) doubleToIntRound(double val) {
	return std::lround(val*100);
};

size_t __attribute__((noinline)) doubleToIntRoundFast(double val) {
	return val*100 + 0.5;
};

void dump(std::vector<int>& v) {
	(void)v;
	std::cout << "dumping ..." << std::endl;
}

int main(int argc, char** argv) {

	if (argc < 2) {
		std::cout << "error: specify iteration count" << std::endl;
		return 1;
	}
	int iterationCount = 0;
 	std::from_chars(argv[1], argv[1] + strlen(argv[1]), iterationCount);
	std::cout << "iterations count: " << iterationCount << std::endl;
	std::vector<int> a;
	std::vector<int> b;
	fillVector(a, iterationCount * 5);
	fillVectorFast(b, iterationCount / 2);
	dump(a);
	dump(b);

	size_t res;
	size_t resRound;
	size_t resRoundFast;
	for (auto i = 0; i < iterationCount; ++i) {
		resRound = doubleToIntRound(static_cast<double>(i) + 0.67);
		res += resRound;
	}
	for (auto i = 0; i < iterationCount; ++i) {
		resRoundFast = doubleToIntRoundFast(static_cast<double>(i) + 0.67);
		res += resRoundFast;
	}
	std::cout << "total res: " << res << std::endl;
}
