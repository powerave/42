#include "Point.hpp"

Point::Point() : x(0), y(0) {}

Point::Point(const float x, const float y) : x(x), y(y) {}

Point::Point(const Point& other) : x(other.x), y(other.y) {}

Point&	Point::operator=(const Point& other) {
	if (this != &other) {
		x = other.x;
		y = other.y;
	}
	return *this;
}

Point	Point::operator-(const Point& other) const{
	return (Point((x - other.x).toFloat(), (y - other.y).toFloat()));
}

Fixed	Point::operator*(const Point& other) const {
	return ((x * other.y) - (y * other.x));
}

Point::~Point() {}