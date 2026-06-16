#include "Point.hpp"

bool	bsp(Point const a, Point const b, Point const c, Point const point) {
	Point ab = b - a;
	Point bc = c - b;
	Point ca = a - c;
	Point ap = point - a;
	Point bp = point - b;
	Point cp = point - c;

	Fixed cross1 = ab * ap;
	Fixed cross2 = bc * bp;
	Fixed cross3 = ca * cp;
	
	float		aa = cross1.toFloat();
	float		bb = cross2.toFloat();
	float		cc = cross3.toFloat();
	
	const float	epsilon = 1e-7f;

	std::cout << "cross1 = " << aa << std::endl;
	std::cout << "cross2 = " << bb << std::endl;
	std::cout << "cross3 = " << cc << std::endl;

	if (std::abs(aa) < epsilon || std::abs(bb) < epsilon || std::abs(cc) < epsilon)
	{
		std::cout << "P est a l'exterieur" << std::endl;
		return false;
	}

	if ((aa > 0 && cc > 0 && bb > 0) || (aa < 0 && bb < 0 && cc < 0))
		{
			std::cout << "P est a l'interieur" << std::endl;
			return true;
		}
	std::cout << "P est a l'exterieur" << std::endl;
	return false;
}
