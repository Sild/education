#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
int main() {
    std::string input = "input.txt";
    std::string output = "output.txt";
    
    std::string line = "";
    std::ifstream file(input);
    if (file.is_open()) {
        std::getline(file, line);
        file.close();
    }
    auto space_pos = line.find(" ");
    auto end = line.data() + space_pos;
    int first = std::strtoll(line.data(), &(end), 10);
    end = line.data() + line.size();
    int second = std::strtoll(line.data() + space_pos + 1, &(end), 10);

    std::ofstream myfile;
    myfile.open(output);
    myfile << (first + second);
    myfile.close();
}