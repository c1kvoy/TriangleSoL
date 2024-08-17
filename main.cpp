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

    vector<Point> triangle_side_intersection(const Point& p1, const Point& p2) const {
        vector<Point> intersections;
        Point direction = p2.vector_by_two_points(p1);
        double denom = A * direction.x + B * direction.y + C * direction.z;
        double t = -(A * p1.x + B * p1.y + C * p1.z + D) / denom;
        if (denom == 0) return intersections;
        if (t < 0 || t > 1) {
            return intersections;
        }
        Point intersection(p1.x + t * direction.x, p1.y + t * direction.y, p1.z + t * direction.z);
        intersections.push_back(intersection);
        return intersections;
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

    vector<double> triangle_division(const Plane& plane, const Point point) const {
        vector<Point> side1 = plane.triangle_side_intersection(p1, p2);
        vector<Point> side2 = plane.triangle_side_intersection(p2, p3);
        vector<Point> side3 = plane.triangle_side_intersection(p3, p1);

        vector<Point> intersection_points;

        if (!side1.empty()) {
            intersection_points.push_back(side1[0]);
        }
        if (!side2.empty()) {
            intersection_points.push_back(side2[0]);
        }
        if (!side3.empty()) {
            intersection_points.push_back(side3[0]);
        }

        vector<double> areas;
        if(intersection_points.size()==2){
            Triangle t1(intersection_points[0], intersection_points[1], point);
            areas.push_back(t1.triangle_area());
            areas.push_back(triangle_area() - t1.triangle_area());
            return areas;
        }
        return {};
    }
};

int main() {
    Point t1, t2, t3;
    Point p1, p2, p3;


    cin >> t1.x >> t1.y >> t1.z;
    cin >> t2.x >> t2.y >> t2.z;
    cin >> t3.x >> t3.y >> t3.z;

    Triangle triangle1(t1, t2, t3);
//    cout << triangle1.triangle_area() << endl;

    cin >> p1.x >> p1.y >> p1.z;
    cin >> p2.x >> p2.y >> p2.z;
    cin >> p3.x >> p3.y >> p3.z;

    Plane plane1(p1, p2, p3);

    pair<vector<Point>, vector<Point>> result = triangle1.areas_type(plane1);
    vector<Point> minus = result.first;
    vector<Point> plus = result.second;


    if(minus.size() == 3 or plus.size() == 3){
        cout << triangle1.triangle_area() << endl;
    }else{
        if(minus.size()==1){
            vector<double> areas_res = triangle1.triangle_division(plane1, minus[0]);
            for(double areas_re : areas_res){
                if(areas_re > 0){
                    cout << areas_re << endl;
                }
            }
        }else if(plus.size()==1){
            vector<double> areas_res = triangle1.triangle_division(plane1, plus[0]);
            for (double areas_re : areas_res){
                if(areas_re > 0){
                    cout << areas_re << endl;
                }
            }
        }
    }
    return 0;
}
