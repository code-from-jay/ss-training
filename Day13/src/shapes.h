#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <initializer_list>

struct Point
{
	double x, y;

	Point(int x, int y) : x(x), y(y) {}
};


class Shape2D
{
public:
	virtual double area() = 0;
};

class Polygon : Shape2D
{
protected:
	std::vector<Point> _vertices;

public:
	Polygon(std::initializer_list<Point> points) : _vertices(points)
	{}

	Polygon(const Polygon& other);
	Polygon(Polygon&& other);

	Polygon& operator=(const Polygon& other);
	Polygon& operator=(Polygon&& other);

	virtual double perimeter();
};

class Triangle : public Polygon
{
	Point a, b, c;
public:
	Triangle(Point a, Point b, Point c) :
		Polygon({a, b, c}), a(a), b(b), c(c)
	{}

	Triangle(const Triangle& other);
	Triangle(Triangle&& other);

	Triangle& operator=(const Triangle& other);
	Triangle& operator=(Triangle&& other);

	virtual double area();
};

class Quadrilateral : public Polygon
{
protected:
	Point a, b, c, d;
public:
	Quadrilateral(Point a, Point b, Point c, Point d) :
		Polygon({a, b, c, d}), a(a), b(b), c(c), d(d)
	{}

	Quadrilateral(const Quadrilateral& other);
	Quadrilateral(Quadrilateral&& other);

	Quadrilateral& operator=(const Quadrilateral& other);
	Quadrilateral& operator=(Quadrilateral&& other);

	virtual double area();
};

class Trapezoid : public Quadrilateral
{
public:
	Trapezoid(Point a, Point b, Point c, Point d) :
		Quadrilateral(a, b, c, d)
	{}

	virtual double area();
};

class Parallelogram : public Quadrilateral
{
public:
	Parallelogram(Point a, Point b, Point c, Point d) :
		Quadrilateral(a, b, c, d)
	{}

	virtual double area();
};

class Rhombus : public Parallelogram
{
public:
	Rhombus(Point a, Point b, Point c, Point d) :
		Parallelogram(a, b, c, d)
	{}

	virtual double area();
};

class Rectangle : public Parallelogram
{
public:
	Rectangle(Point a, Point b, Point c, Point d) :
		Parallelogram(a, b, c, d)
	{}

	virtual double area();
};

class Square : public Rhombus
{
public:
	Square(Point a, Point b, Point c, Point d) :
		Rhombus(a, b, c, d)
	{}

	virtual double area();
	virtual double perimeter();
};

class Ellipse : public Shape2D
{
	double majorAxis, minorAxis;
public:
	Ellipse(double a, double b) :
		majorAxis(a > b ? a : b),
		minorAxis(a < b ? a : b)
	{}

	Ellipse(const Ellipse& other);
	Ellipse(Ellipse&& other);

	Ellipse& operator=(const Ellipse& other);
	Ellipse& operator=(Ellipse&& other);

	virtual double area();
	virtual double circumference();
	inline double majorDiameter() { return majorAxis; }
	inline double minorDiameter() { return minorAxis; }
};

class Circle : public Ellipse
{
	double x;
public:
	Circle(double x) : Ellipse(x, x), x(x)
	{}

	Circle(const Circle& other);
	Circle(Circle&& other);

	Circle& operator=(const Circle& other);
	Circle& operator=(Circle&& other);

	virtual double area();
	virtual double circumference();
	inline double diameter() { return x; }
};

#endif
