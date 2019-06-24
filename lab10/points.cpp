#include "points.h"

Point_3d::Point_3d(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point_3d::set_x(const double x) {
    this->x = x;
}

void Point_3d::set_y(const double y) {
    this->y = y;
}

void Point_3d::set_z(const double z) {
    this->z = z;
}

double Point_3d::get_x() const {
    return x;
}

double Point_3d::get_y() const {
    return y;
}

double Point_3d::get_z() const {
    return z;
}

void Point_3d::offset(const double dx, const double dy, const double dz) {
    x += dx;
    y += dy;
    z += dz;
}

void Point_3d::scale(const double k, const Point_3d &centre) {
    x = centre.x + k * (x - centre.x);
    y = centre.y + k * (y - centre.y);
    z = centre.z + k * (z - centre.z);
}

void Point_3d::rotate_x(const double angle, const Point_3d &centre) {
    Point_3d tmp = Point_3d(x, y, z);
    double ang = angle * M_PI / 180;

    y = int(centre.y + (tmp.y - centre.y) * cos(ang) + (tmp.z - centre.z) * sin(ang));
    z = int(centre.z - (tmp.y - centre.y) * sin(ang) + (tmp.z - centre.z) * cos(ang));
}

void Point_3d::rotate_y(const double angle, const Point_3d &centre) {
    Point_3d tmp = Point_3d(x, y, z);
    double ang = angle * M_PI / 180;

    x = centre.x + (tmp.x - centre.x) * cos(ang) + (tmp.z - centre.z) * sin(ang);
    z = centre.z - (tmp.x - centre.x) * sin(ang) + (tmp.z - centre.z) * cos(ang);
}

void Point_3d::rotate_z(const double angle, const Point_3d &centre) {
    Point_3d tmp = Point_3d(x, y, z);
    double ang = angle * M_PI / 180;

    x = centre.x + (tmp.x - centre.x) * cos(ang) + (tmp.y - centre.y) * sin(ang);
    y = centre.y - (tmp.x - centre.x) * sin(ang) + (tmp.y - centre.y) * cos(ang);
}

Point_2d::Point_2d(Point_3d point) {
    set_x(point.get_x() /*+ 0.5 * point.get_z()*/);
    set_y(point.get_y() /*+ 0.5 * point.get_z()*/);
}

void Point_2d::set_x(const double x) {
    this->x = x;
}

void Point_2d::set_y(const double y) {
    this->y = y;
}

double Point_2d::get_x() const {
    return x;
}

double Point_2d::get_y() const {
    return y;
}
