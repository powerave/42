#include "Point.hpp"

int main()
{
	/*Point	a(1.0f, 0.0000002f);
	Point	b(1.11f, 0.0000002f);
	Point	c(0.4f, 0.0000025f);
	Point	p(1.0f, 0.0000001f);*/

	/*Point	a(1.0f, 0.02f);
	Point	b(1.11f, 0.02f);
	Point	c(0.4f, 0.25f);
	Point	p(1.0f, 0.01f);*/

	Point	a(0.0f, 0.0f);
	Point	b(4.4f, 0.0f);
	Point	c(0.5f, 8.1f);
	Point	p(1.5f, 4.2f);

	bsp(a, b, c, p);
	return (0);
}
