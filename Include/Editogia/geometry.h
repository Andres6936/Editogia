#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <vector>
#include <string>

#ifndef MIN
#define MIN(x, y) ( (x) < (y) ? x : y)
#endif

#ifndef MOD
#define MOD(a, n) ( (a) < 0 ? ((a) % (n) + (n)) : ((a) % (n)))
#endif

#ifndef PI
#define PI 3.142
#endif

enum Direction
{
	DIR_NULL = 0,
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST
};

enum Direction_full
{
	DIRFULL_NULL = 0,
	DIRFULL_NORTH,
	DIRFULL_NORTHEAST,
	DIRFULL_EAST,
	DIRFULL_SOUTHEAST,
	DIRFULL_SOUTH,
	DIRFULL_SOUTHWEST,
	DIRFULL_WEST,
	DIRFULL_NORTHWEST
};

struct Point
{
	int x;
	int y;

	Point(int X = 0, int Y = 0) : x(X), y(Y)
	{
	}

	Point(const Point& p) : x(p.x), y(p.y)
	{
	}

	~Point()
	{
	}

	std::string str() const;

	bool operator==(const Point& other) const
	{
		return (x == other.x && y == other.y);
	}

	bool operator!=(const Point& other) const
	{
		return !(*this == other);
	}

	Point& operator+=(const Point& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Point& operator-=(const Point& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
};

inline Point operator+(Point lhs, const Point& rhs)
{
	lhs += rhs;
	return lhs;
}

struct Pointcomp
{
	bool operator()(const Point& lhs, const Point& rhs) const
	{
		if (lhs.x < rhs.x) return true;
		if (lhs.x > rhs.x) return false;
		if (lhs.y < rhs.y) return true;
		if (lhs.y > rhs.y) return false;
		return false;
	}
};

std::vector<Point> line_to(int x0, int y0, int x1, int y1);

std::vector<Point> line_to(Point origin, Point target);

int rl_dist(int x0, int y0, int x1, int y1);

int rl_dist(Point origin, Point target);

// sqrt(dX^2 + dY^2)
int trig_dist(int x0, int y0, int x1, int y1);

int trig_dist(Point origin, Point target);

// dX + dY
int manhattan_dist(int x0, int y0, int x1, int y1);

int manhattan_dist(Point origin, Point target);

// points_at_exact_distance() returns a list of all points exactly dist tiles
// (using rl_dist() calculation) from origin.  Basically, a circle.
std::vector<Point> points_at_exact_distance(Point origin, int dist);

#endif
