#include <utility>
#include <vector>
#include <cmath>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

using std::chrono::high_resolution_clock;
using std::chrono::duration;

constexpr double G = 6.673e-11;
constexpr double timestep = 0.001;
constexpr double initial_mass = 1.0;
constexpr double k = 100;

struct Vec2
{
	double x;
	double y;

	Vec2(double x, double y) : x(x), y(y) {}
	Vec2() : Vec2(0,0) {}

	Vec2(const Vec2& other) : x(other.x), y(other.y) {}
	Vec2(Vec2&& other) : x(std::exchange(other.x, 0)), y(std::exchange(other.y, 0)) {}

	Vec2& operator=(const Vec2& other)
	{
		if (this == &other)
			return *this;

		x = other.x;
		y = other.y;

		return *this;
	}

	Vec2& operator=(Vec2&& other)
	{
		if (this == &other)
			return *this;

		x = std::exchange(other.x, 0);
		y = std::exchange(other.y, 0);

		return *this;
	}

	Vec2& operator+=(const Vec2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	friend Vec2 operator+(const Vec2& lhs, const Vec2& rhs) { return Vec2{lhs.x + rhs.x, lhs.y + rhs.y}; }
	friend Vec2 operator-(const Vec2& lhs, const Vec2& rhs) { return Vec2{lhs.x - rhs.x, lhs.y - rhs.y}; }
	friend Vec2 operator*(const Vec2& lhs, const Vec2& rhs) { return Vec2{lhs.x * rhs.x, lhs.y * rhs.y}; }
	friend Vec2 operator*(const Vec2& lhs, double rhs) { return Vec2{lhs.x * rhs, lhs.y * rhs}; }
	friend Vec2 operator/(const Vec2& lhs, const Vec2& rhs) { return Vec2{lhs.x / rhs.x, lhs.y / rhs.y}; }
	friend Vec2 operator/(const Vec2& lhs, double rhs) { return Vec2{lhs.x / rhs, lhs.y / rhs}; }
};

struct Body
{
	int ind;
	double mass;
	Vec2 position;
	Vec2 velocity;
	std::vector<Vec2> forceVector;
	Vec2 totalForce;
	Vec2 acceleration;

	Body(int index, int totalBodies, Vec2 position) :
		ind(index), mass(initial_mass), position(position),
		velocity(Vec2{}), forceVector(totalBodies),
		totalForce(Vec2{}), acceleration(Vec2{})
	{}
};

int main()
{
	std::stringstream output;
	output << "{\n  \"records\": [";
	int numBodies[] = { 10, 20, 50, 100, 200, 500, 1000, 2000 };

	auto update_force = [](double m1, double m2, Vec2 r1, Vec2 r2)
	{
		if (r1.x == r2.x && r1.y == r2.y)
			return Vec2{};

		auto calculate_distance = [r1, r2]
		{
			return sqrt(pow(r1.x - r2.x, 2) + pow(r1.y - r2.y, 2));
		};
		double distance = calculate_distance();

		auto calculate_force = [m1, m2, distance]
		{
			return -1 * G * m1 * m2 / pow(distance, 2);
		};
		double force = calculate_force();

		double r = atan(std::abs(r1.y - r2.y) / std::abs(r1.x - r2.x));
		Vec2 f = Vec2{ cos(r) * force, sin(r) * force };

		if (r1.x < r2.x) f.x = std::abs(f.x);
		else if (r1.y < r2.y) f.y = std::abs(f.y);

		return f;
	};
	
	bool first = true;
	for (auto bodyCount : numBodies)
	{
		std::vector<Body> bodies = std::vector<Body>();
		bodies.reserve(bodyCount);

		for (int i = 0; i < bodyCount; ++i)
			bodies.emplace_back(i, bodyCount, Vec2{(double)(bodyCount * i), (double)(bodyCount * i * 2)});

		auto start = high_resolution_clock::now();
		for (int step = 0; step < k; ++step)
		{
			for (auto& body : bodies)
			{
				for (auto& otherBody : bodies)
				{
					body.forceVector[otherBody.ind] = update_force(body.mass, otherBody.mass, body.position, otherBody.position);
				}

				[&body] { for (auto& f : body.forceVector) body.totalForce += f; }();
				[&body] { body.acceleration = body.totalForce / body.mass; }();
				[&body] { body.velocity += body.acceleration * timestep; }();
				[&body] { body.position += body.velocity * timestep; }();
			}
		}
		auto end = high_resolution_clock::now();
		duration<double> result = end - start;

		auto ips = [bodyCount, result] { return k * bodyCount * bodyCount / result.count(); }();
		if (first) first = !first;
		else output << ",";
		output << "\n    {\n      \"N\": " << bodyCount << ",\n      \"IpS\": " << ips << "\n    }";
	}
	output << "\n  ]\n}";

	std::ofstream fs{"output.json", std::ios::out};
	fs << output.str() << std::endl;
	fs.close();
	
	return 0;
}
