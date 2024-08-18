#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>

using namespace std;

struct Point { // çтруктура точки, для дальнейшего удобства
    // перегрузил операторы сравнения, написал методы вычисления длины вектора, вектора по точкам, векторного умножения
    double x, y, z;

    Point(): x(0), y(0), z(0) {}

    Point(double x, double y, double z): x(x), y(y), z(z) {}

    Point vector_by_two_points(const Point& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    double length() const {
        return sqrt(x * x + y * y + z * z);
    }

    Point vector_multiply(const Point& other) const {
        return {y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x};
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

struct Plane { // плоскость
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
    // метод чекает с какой стороны проверяет с какой стороны от плоскости находится точка, обычная подстановка в уравнение плоскости
    double side_check(const Point& p) const {
        return A * p.x + B * p.y + C * p.z + D;
    }


    //метод находит пересечения вектора нормали плоскости с вектором, который задается двумя точками, возвращает точку пересечения
    vector<Point> triangle_side_intersection(const Point& p1, const Point& p2) const {
        vector<Point> intersections;
        Point direction = p2.vector_by_two_points(p1);
        double denom = A * direction.x + B * direction.y + C * direction.z;

        if (fabs(denom) < 1e-9) {
            return intersections;
        }
        double t = -(A * p1.x + B * p1.y + C * p1.z + D) / denom;
        if (t < 0 || t > 1) {
            return intersections;
        }
        Point intersection(p1.x + t * direction.x, p1.y + t * direction.y, p1.z + t * direction.z);
        intersections.push_back(intersection);
        return intersections;
    }
};

struct Triangle { // треугольник
    Point p1, p2, p3;

    Triangle(const Point& p1, const Point& p2, const Point& p3): p1(p1), p2(p2), p3(p3) {}

    // площадь треугольника, через векторное произведение
    double triangle_area() const {
        Point v1 = p2.vector_by_two_points(p1);
        Point v2 = p3.vector_by_two_points(p1);

        return v1.vector_multiply(v2).length() / 2;
    }


    // метод позволяет понять, с какой стороны от плоскости находится точка, возвращает 3 вектора со знаками + - и точками в плоскости
    tuple<vector<Point>, vector<Point>, vector<Point>> areas_type(const Plane& plane) const {
        double check1 = plane.side_check(p1);
        double check2 = plane.side_check(p2);
        double check3 = plane.side_check(p3);
        vector<Point> points = {p1, p2, p3};
        vector<double> checks = {check1, check2, check3};
        vector<Point> minus, plus, nulls;

        for (int i = 0; i < 3; i++) {
            if (checks[i] > 0) {
                plus.push_back(points[i]);
            } else if (checks[i] < 0) {
                minus.push_back(points[i]);
            } else {
                nulls.push_back(points[i]);
            }
        }
        return make_tuple(minus, plus, nulls);
    }
};


int main() {
    Point t1, t2, t3;
    Point p1, p2, p3;

    cin >> t1.x >> t1.y >> t1.z;
    cin >> t2.x >> t2.y >> t2.z;
    cin >> t3.x >> t3.y >> t3.z;

    Triangle triangle1(t1, t2, t3);

    cin >> p1.x >> p1.y >> p1.z;
    cin >> p2.x >> p2.y >> p2.z;
    cin >> p3.x >> p3.y >> p3.z;

    Plane plane1(p1, p2, p3);

    tuple<vector<Point>, vector<Point>, vector<Point>> result = triangle1.areas_type(plane1);
    vector<Point> minus = get<0>(result);
    vector<Point> plus = get<1>(result);
    vector<Point> nulls = get<2>(result);

    vector<Point> intersection_points;
    vector<double> areas;

    if (nulls.size() > 1 or plus.size() == 3 or minus.size() == 3) { // все случаи когда нет разреза
        return 0;
    } else if (nulls.size() == 1) { // случаи, когда одна вершина лежит в плоскости и после разреза получается 2 треугольника
        if (nulls[0] != triangle1.p1 && nulls[0] != triangle1.p2) {
            vector<Point> side1 = plane1.triangle_side_intersection(triangle1.p1, triangle1.p2);
            if (!side1.empty()) {
                intersection_points.push_back(side1[0]);
            }
        } else if (nulls[0] != triangle1.p2 && nulls[0] != triangle1.p3) {
            vector<Point> side2 = plane1.triangle_side_intersection(triangle1.p2, triangle1.p3);
            if (!side2.empty()) {
                intersection_points.push_back(side2[0]);
            }
        } else if (nulls[0] != triangle1.p1 && nulls[0] != triangle1.p3) {
            vector<Point> side3 = plane1.triangle_side_intersection(triangle1.p1, triangle1.p3);
            if (!side3.empty()) {
                intersection_points.push_back(side3[0]);
            }
        }
        if (intersection_points.size() == 1) {
            Triangle triangle2(intersection_points[0], minus[0], nulls[0]);
            areas.push_back(triangle2.triangle_area());
            areas.push_back(triangle1.triangle_area() - triangle2.triangle_area());
        } else {
            return 0; // случай, когда плоскость перпендикулярна треугольнику, и задевает вершину, но не режет
        }
    } else { // случай, когда при разрезе образуется трапеция
        vector<Point> side1 = plane1.triangle_side_intersection(triangle1.p1, triangle1.p2);
        vector<Point> side2 = plane1.triangle_side_intersection(triangle1.p2, triangle1.p3);
        vector<Point> side3 = plane1.triangle_side_intersection(triangle1.p3, triangle1.p1);

        if (!side1.empty()) {
            intersection_points.push_back(side1[0]);
        }
        if (!side2.empty()) {
            intersection_points.push_back(side2[0]);
        }
        if (!side3.empty()) {
            intersection_points.push_back(side3[0]);
        }
        if( plus.size() > minus.size()){
            Triangle triangle2(intersection_points[0], intersection_points[1], minus[0]);
            areas.push_back(triangle2.triangle_area());
            areas.push_back(triangle1.triangle_area() - triangle2.triangle_area());
        }else{
            Triangle triangle2(intersection_points[0], intersection_points[1], plus[0]);
            areas.push_back(triangle2.triangle_area());
            areas.push_back(triangle1.triangle_area() - triangle2.triangle_area());
        }
    }
    for (double area : areas) {
        cout << area << endl;
    }
    return 0;
}
