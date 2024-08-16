#include <iostream>
using namespace std;


struct Point
{
    double x;
    double y;
    double z;

    Point(): x(0), y(0), z(0) {}

    Point(double x, double y, double z): x(x), y(y), z(z) {}

    Point vector_by_two_points(const Point& other) const {
        return Point(x - other.x, y - other.y, z - other.z);
    }

    double length() const {
        return sqrt(x * x + y * y + z * z);
    }

    Point vector_multiply(const Point& other) const {
        return Point(y * other.z - z * other.y,
                     z * other.x - x * other.z,
                     x * other.y - y * other.x);
    }
};


struct Plane {
    double A, B, C, D;

    Plane(const Point& p1, const Point& p2, const Point& p3) {
        Point v1 = p2.vector_by_two_points(p1);
        Point v2 = p3.vector_by_two_points(p1);
        Point normal = v1.vector_multiply(v2);

        A = normal.x;
        B = normal.y;
        C = normal.z;
        D = -(A * p1.x + B * p1.y + C * p1.z);
    }

    double side_check(const Point& p) const {
        return A * p.x + B * p.y + C * p.z + D;
    }
};


struct Triangle {
    Point p1;
    Point p2;
    Point p3;

    double triangle_area() const {
        Point v1 = p2.vector_by_two_points(p1);
        Point v2 = p3.vector_by_two_points(p1);

        return v1.vector_multiply(v2).length()/2;
    }

    Triangle(const Point& p1, const Point& p2, const Point& p3): p1(p1), p2(p2), p3(p3) {}
};

int main() {
    Point point1(0,0,4);
    Point point2(3,0,0);
    Point point3(3,4,0);

    Triangle triangle(point1, point2, point3);
    double area = triangle.triangle_area();
    cout << area;
    return 0;
}
