#include <iostream>
#include "input_reader.h"
#include "data_handler.h"

static constexpr size_t DEFAULT_BLOCK_SIZE = 5;
size_t ReadBlockSize(int argc, char* argv[]) {
    if(argc == 2) {
        return static_cast<size_t>(std::stoi(argv[1]));
    }
    return DEFAULT_BLOCK_SIZE;
}

int main(int argc, char* argv[]) {
    TInputReader reader(ReadBlockSize(argc, argv));

    TConsoleHandler condoleHandler(reader);
    TFileHandler fileHandler(reader);

    reader.Start();


    return 0;
}
