#include "Point.hpp"
#include "Edge.hpp"

#include <vector>

class Summit {
	// each point connected to edges
	private:
	Point point;
	std::vector<Edge> edgesConnected;
	Colore color;

public:
	Summit() {}
	Summit(Colore c, std::vector<Edge> e, Point p)
	{
		color = c;
		edgesConnected = e;
		point = p;
	}
	void setPoint(const Point & p)
	{
		point = p;
	}
	const Point & getPoint() const
	{
		return point;
	}
	void setColor(const Colore & c)
	{
		color = c;
	}
	const Colore & getColor() const
	{
		return color;
	}
	void setEdgesConnected(const std::vector<Edge> & f)
	{
		edgesConnected = f;
	}
	const std::vector<Edge> & getEdgesConnected() const
	{
		return edgesConnected;
	}
};