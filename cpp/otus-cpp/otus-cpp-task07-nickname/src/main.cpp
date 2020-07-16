#include <iostream>
#include <string>
#include "radix_tree.h"
void run() {
    NCustom::TRadixTree tree;
    for (std::string line; std::getline(std::cin, line);) {
        tree.Emplace(line);
    }
    tree.PrintAsTree();
    tree.PrintItems();

}
int main(int argc, char* argv[]) {
    run();
    return 0;
}
