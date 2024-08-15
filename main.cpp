#include <iostream>
using namespace std;
struct Point
{
    double x;
    double y;
    double z;

    Point(): x(0), y(0), z(0) {}

    Point(double x, double y, double z): x(x), y(y), z(z) {}

    Point vector_by_two_points(Point& other) const {
        return Point(x - other.x, y - other.y, z - other.z);
    }

    double length_of_vector() const {
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
    }
};


struct Triangle {
    Point p1;
    Point p2;
    Point p3;

    double triangle_area() {
        Point v1 = p2.vector_by_two_points(p1);
        Point v2 = p3.vector_by_two_points(p1);

        Point vect_mul(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);

        cout << vect_mul.length_of_vector()/2 << endl;

    }

    Triangle(const Point& p1, const Point& p2, const Point& p3): p1(p1), p2(p2), p3(p3) {}
};

int main() {
    Point point1(0,0,4);
    Point point2(3,0,0);
    Point point3(3,4,0);

    Triangle triangle(point1, point2, point3);
    triangle.triangle_area();
    return 0;
}
