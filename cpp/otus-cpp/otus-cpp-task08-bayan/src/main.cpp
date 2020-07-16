#include <iostream>
#include <string>
#include <argparse.h>
#include <config.h>
#include "fs_walker.h"

int main(int argc, char* argv[]) {
    auto sConfig = parseConfig(argc, argv);
    auto sWalker = FsWalker(sConfig);
    sWalker.Run();
    sWalker.PrintDuplicates();

}