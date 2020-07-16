#include "Point.h"

std::istream &operator>>(std::istream &is, Point &p) {
    is >> p.x >> p.y;
    if (!is) {
        throw Point::ConstructorFailException();
    }
    return is;
}

std::vector<Point> Point::get_points() {
    std::cin.exceptions(std::cin.exceptions() | std::ios_base::badbit);
    size_t count = get_points_count();
    std::vector<Point> points;
    for (int i = 0; i < count && std::cin; i++) {
        std::cout << (i + 1) << ": ";
        Point p;
        try {
            std::cin >> p;
            points.push_back(p);
        } catch (Point::ConstructorFailException e) {
            std::cout << std::endl << "Некорректные входные данные" << std::endl;
            skip_line(std::cin);
            i--;
        }
    }
    return points;
}

void Point::write_points(const std::vector<Point> &points, const char *file_path) {
    std::cout.exceptions(std::cout.exceptions() | std::ios_base::badbit);
    std::ofstream ofs(file_path);
    if (!ofs) {
        throw std::ofstream::failure("Fail to open target file");
    }
    for (const auto &p: points) {
        ofs << p << std::endl;
    }
}

std::vector<Point> Point::read_points(const char *file_path) {
    std::cin.exceptions(std::cin.exceptions() | std::ios_base::badbit);
    std::vector<Point> points;
    std::ifstream ifs(file_path);
    if (!ifs) {
        throw std::ifstream::failure("Fail to open source file");
    }
    while (!ifs.eof()) {
        Point point;
        try {
            ifs >> point;
            points.push_back(point);
        } catch (Point::ConstructorFailException e) {
            std::cout << "unknown line, ignoring ..." << std::endl;
            if(ifs.eof()) continue;
            skip_line(ifs);
            if(!ifs) {
                throw std::ifstream::failure("Unknown error while reading the file");
            }
        }

    }
    return points;
}

size_t Point::get_points_count() {
    size_t points_count;
    std::cin.exceptions(std::cin.exceptions() | std::ios_base::badbit);
    std::cout << "input points count: ";
    std::cin >> points_count;
    while (!std::cin) {
        skip_line(std::cin);
        std::cout << "Wrong count. Any size_t required. Try again: ";
        std::cin >> points_count;
    }
    return points_count;
}

void Point::skip_line(std::istream &is) {
    is.clear();
    while (is.get() != '\n') {
        continue;
    }
}
