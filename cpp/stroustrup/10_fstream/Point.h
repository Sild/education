#ifndef CPP_TASKS_POINT_H
#define CPP_TASKS_POINT_H


#include <fstream>
#include <iostream>
#include <vector>

class Point {
public:
    static std::vector<Point> get_points();

    static void write_points(const std::vector<Point> &points, const char *file_path);

    static std::vector<Point> read_points(const char *file_path);

    Point() { x = y = 0; }

    Point(int in_x, int in_y) : x(in_x), y(in_y) {};

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
        os << "(" << p.x << "," << p.y << ")";
        return os;
    }

    friend std::ofstream &operator<<(std::ofstream &os, const Point &p) {
        os << p.x << " " << p.y;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Point &p);

    class ConstructorFailException : std::exception {};

private:
    int x, y;

    static size_t get_points_count();

    static void skip_line(std::istream &is);
};


#endif //CPP_TASKS_POINT_H
