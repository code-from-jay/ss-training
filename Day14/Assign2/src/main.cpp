#include "shapes.h"

#include <cassert>

int main(int argc, char** argv)
{
	Triangle triangle{{0, 0}, {5, 5}, {10, 0}};
	Square square({0, 5}, {5, 5}, {5, 0}, {0, 0});
	Ellipse ellipse(5, 10);
	
	Shape2D* shape = &triangle;

	shape->area();
	assert(Shape2D::calls.str() == "{Triangle::area}");

	Polygon* polygon = &triangle;
	
	polygon->perimeter();
	assert(Shape2D::calls.str() == "{Triangle::area}{Polygon::perimeter}");

	polygon = &square;
	polygon->area();
	polygon->perimeter();
	assert(Shape2D::calls.str() == "{Triangle::area}{Polygon::perimeter}{Square::area}{Square::perimeter}");

	shape = &ellipse;
	shape->area();
	assert(Shape2D::calls.str() == "{Triangle::area}{Polygon::perimeter}{Square::area}{Square::perimeter}{Ellipse::area}");

	return 0;
}
