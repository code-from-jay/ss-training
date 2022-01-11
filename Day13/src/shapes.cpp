#include <cmath>
#include <algorithm>

#include "shapes.h"

double distance(const Point& a, const Point& b)
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

Polygon::Polygon(const Polygon& other)
{
	std::copy(other._vertices.begin(), other._vertices.end(), _vertices.begin());
}

Polygon::Polygon(Polygon&& other)
{
	std::move(other._vertices.begin(), other._vertices.end(), _vertices.begin());
}

Polygon& Polygon::operator=(const Polygon& other)
{
	if (this == &other)
		return *this;

	std::copy(other._vertices.begin(), other._vertices.end(), _vertices.begin());
	return *this;
}

Polygon& Polygon::operator=(Polygon&& other)
{
	if (this == &other)
		return *this;

	std::move(other._vertices.begin(), other._vertices.end(), _vertices.begin());
	return *this;
}

double Polygon::perimeter()
{
	double sum = 0;
	for (int i = 1; i < _vertices.size(); ++i)
		sum += distance(_vertices[i-1], _vertices[i]);
	sum += distance(*(_vertices.end()-1), *_vertices.begin());
	return sum;
}

Triangle::Triangle(const Triangle& other) :
	a(other.a), b(other.b), c(other.c),
	Polygon(dynamic_cast<const Polygon&>(other))
{

}

Triangle::Triangle(Triangle&& other) :
	a(std::move(other.a)), b(std::move(other.b)), c(std::move(other.c)),
	Polygon(dynamic_cast<Polygon&&>(other))
{
}

Triangle& Triangle::operator=(const Triangle& other)
{
	if (this == &other)
		return *this;

	this->a = other.a;
	this->b = other.b;
	this->c = other.c;

	this->Polygon::operator=(dynamic_cast<const Polygon&>(other));
	return *this;
}

Triangle& Triangle::operator=(Triangle&& other)
{
	if (this == &other)
		return *this;

	this->a = std::move(other.a);
	this->b = std::move(other.b);
	this->c = std::move(other.c);

	this->Polygon::operator=(dynamic_cast<Polygon&&>(other));
	return *this;
}

double Triangle::area()
{
	return std::abs(
			(a.x * (b.y - c.y)) +
			(b.x * (c.y - a.y)) +
			(c.x * (a.y - b.y))) / 2;
}

Quadrilateral::Quadrilateral(const Quadrilateral& other) :
	a(other.a), b(other.b), c(other.c), d(other.d),
	Polygon(dynamic_cast<const Polygon&>(other))
{
}

Quadrilateral::Quadrilateral(Quadrilateral&& other) :
	a(std::move(other.a)), b(std::move(other.b)), c(std::move(other.c)), d(std::move(other.d)),
	Polygon(dynamic_cast<Polygon&&>(other))
{
}

Quadrilateral& Quadrilateral::operator=(const Quadrilateral& other)
{
	if (this == &other)
		return *this;

	this->a = other.a;
	this->b = other.b;
	this->c = other.c;
	this->d = other.d;

	this->Polygon::operator=(dynamic_cast<const Polygon&>(other));
	return *this;
}

Quadrilateral& Quadrilateral::operator=(Quadrilateral&& other)
{
	if (this == &other)
		return *this;

	this->a = std::move(other.a);
	this->b = std::move(other.b);
	this->c = std::move(other.c);
	this->d = std::move(other.d);

	this->Polygon::operator=(dynamic_cast<Polygon&&>(other));
	return *this;
}

double Quadrilateral::area()
{
	Triangle t1{a, b, c};
	Triangle t2{b, c, d};

	return t1.area() + t2.area();
}

double Trapezoid::area()
{
	double height = std::abs(a.y) + std::abs(d.y);
	double b1 = distance(a, b);
	double b2 = distance(c, d);

	return (height/2) * (b1 + b2);
}

double Parallelogram::area()
{
	double A = distance(a, b), B = distance(c, d);
	return (A*B)*sin(90);
}

double Rhombus::area()
{
	double d1 = distance(a, c), d2 = distance(b, d);
	return (d1/2)*d2;
}

double Rectangle::area()
{
	double L = distance(a, b), B = distance(b, c);
	return L * B;
}

double Square::area()
{
	return pow(distance(a, b), 2);
}

double Square::perimeter()
{
	return distance(a, b) * 4;
}

Ellipse::Ellipse(const Ellipse& other) :
	majorAxis(other.majorAxis),
	minorAxis(other.minorAxis)
{}

Ellipse::Ellipse(Ellipse&& other) :
	majorAxis(std::move(other.majorAxis)),
	minorAxis(std::move(other.minorAxis))
{}

Ellipse& Ellipse::operator=(const Ellipse& other)
{
	if (this == &other)
		return *this;

	this->majorAxis = other.majorAxis;
	this->minorAxis = other.minorAxis;

	return *this;
}

Ellipse& Ellipse::operator=(Ellipse&& other)
{
	if (this == &other)
		return *this;

	this->majorAxis = std::move(other.majorAxis);
	this->minorAxis = std::move(other.minorAxis);

	return *this;
}

double Ellipse::area()
{
	return M_PI * majorAxis * minorAxis;
}

double Ellipse::circumference()
{
	return 2 * M_PI * sqrt((pow(majorAxis, 2) + pow(minorAxis, 2))/2);
}

Circle::Circle(const Circle& other) :
	x(other.x),
	Ellipse(dynamic_cast<const Ellipse&>(other))
{}

Circle::Circle(Circle&& other) :
	x(std::move(other.x)),
	Ellipse(dynamic_cast<Ellipse&&>(other))
{}

Circle& Circle::operator=(const Circle& other)
{
	if (this == &other)
		return *this;

	this->x = other.x;

	this->Ellipse::operator=(dynamic_cast<const Ellipse&>(other));
	return *this;
}

Circle& Circle::operator=(Circle&& other)
{
	if (this == &other)
		return *this;

	this->x = std::move(other.x);

	this->Ellipse::operator=(dynamic_cast<Ellipse&&>(other));
	return *this;
}

double Circle::area()
{
	return M_PI * pow(x/2, 2);
}

double Circle::circumference()
{
	return M_PI * x;
}
