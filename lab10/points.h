#ifndef POINTS_H
#define POINTS_H

#include "math.h"

class Point_3d
{
public:
    Point_3d() {}
    Point_3d(double x, double y, double z);
    ~Point_3d() {}

    void set_x(const double x);
    void set_y(const double y);
    void set_z(const double z);

    double get_x() const;
    double get_y() const;
    double get_z() const;

    void offset(const double dx, const double dy, const double dz);
    void scale(const double k, const Point_3d &centre = Point_3d(0, 0, 0));
    void rotate_x(const double angle, const Point_3d &centre = Point_3d(0, 0, 0));
    void rotate_y(const double angle, const Point_3d &centre = Point_3d(0, 0, 0));
    void rotate_z(const double angle, const Point_3d &centre = Point_3d(0, 0, 0));

private:
    double x;
    double y;
    double z;
};

class Point_2d
{
public:
    Point_2d() {}
    Point_2d(Point_3d point);
    ~Point_2d() {}

    void set_x(const double x);
    void set_y(const double y);

    double get_x() const;
    double get_y() const;

private:
    double x;
    double y;
};

#endif // POINTS_H
