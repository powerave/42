#include "Fixed.hpp"

class	Point {
	private :
		Fixed	x;
		Fixed	y;
	
	public :
		Point();
		Point(const float x, const float y);
		Point(const Point& other);
		Point&	operator=(const Point& other);
		Point	operator-(const Point& other) const;
		Fixed	operator*(const Point& other) const;
		~Point();
};

bool	bsp(Point const a, Point const b, Point const c, Point const point);