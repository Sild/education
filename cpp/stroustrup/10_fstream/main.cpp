#include <vector>
#include "Point.h"
const char* PROCESS_FILE = "fstream_output.txt";

int main() {
    const std::vector<Point> original_points = Point::get_points();
    for(auto p: original_points) {
        std::cout << p << std::endl;
    }
    Point::write_points(original_points, PROCESS_FILE);

    const std::vector<Point> proceeded_points = Point::read_points(PROCESS_FILE);

    if ( original_points == proceeded_points ) {
        std::cout << "vectors are same" << std::endl;
    } else {
        std::cout << "vectors are not same" << std::endl;
    }
}