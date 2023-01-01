#include <iostream>
#include <array>
#include <unordered_map>

template<typename T>
struct FlagConfigurator
{
};

struct DebInf: public FlagConfigurator<DebInf>
{
	bool location = false;
	bool snippet = false;
	bool astId = false;

	static auto const& componentMap()
	{
		static std::unordered_map<const char*, bool DebInf::*> const components = {
			{"location", &DebInf::location},
			{"snippet", &DebInf::snippet},
			{"ast-id", &DebInf::astId},
		};
		return components;
	}
};


int main() {
    std::cout << "hello" << std::endl;
    DebInf di;
}
