#include <iostream>
#include <vector>
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

    Plane(double A, double B, double C, double D) : A(A), B(B), C(C), D(D) {}

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

    Triangle(const Point& p1, const Point& p2, const Point& p3): p1(p1), p2(p2), p3(p3) {}

    double triangle_area() const {
        Point v1 = p2.vector_by_two_points(p1);
        Point v2 = p3.vector_by_two_points(p1);

        return v1.vector_multiply(v2).length()/2;
    }

    pair<vector<Point>, vector<Point>> areas_type(const Plane& plane) const {
        double check1 = plane.side_check(p1);
        double check2 = plane.side_check(p2);
        double check3 = plane.side_check(p3);
        vector<Point> points = {p1, p2, p3};
        vector<double> checks = {check1, check2, check3};
        vector<Point> minus;
        vector<Point> plus;
        for(int i = 0; i < 3; i++ ){
            if(checks[i] >= 0) {
                plus.push_back(points[i]);
            }else{
                minus.push_back(points[i]);
            }
        }
        return make_pair(minus, plus);
    }
};

int main() {
    Point t1(0.0, 0.0, 4.0);
    Point t2(3.0, 0.0, 0.0);
    Point t3(3.0, 4.0, 0.0);
    Point p1(0.0, 0.0, 2.0);
    Point p2(1.5, 0.0, 2.0);
    Point p3(1.5, 2.0, 2.0);
    Plane plane1(p1, p2, p3);

//    Point t1(0.0, 0.0, 0.0);
//    Point t2(1.0, 0.0, 0.0);
//    Point t3(0.0, 1.0, 0.0);
//    Plane plane1(0.0, 0.0, 1.0, -1.0);

    Triangle triangle1(t1, t2, t3);

    double area = triangle1.triangle_area();
    pair<vector<Point>, vector<Point>> result = triangle1.areas_type(plane1);

    vector<Point> minus = result.first;
    vector<Point> plus = result.second;
    cout << "отрицательный знак"<< endl;
    for (int i = 0; i < minus.size(); i++) {
        cout << minus[i].x << " , " << minus[i].y << " , " << minus[i].z << endl;
    }
    cout << "положительный знак"<< endl;

    for (int i = 0; i < plus.size(); i++) {
        cout << plus[i].x << " , " << plus[i].y << " , " << plus[i].z << endl;
    }

    cout << area;
    return 0;
}
