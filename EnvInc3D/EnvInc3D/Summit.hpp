#include "Point.hpp"
#include "Edge.hpp"

#include <vector>

class Summit {
	// each point connected to edges
	private:
	Point p;
	std::vector<Edge> edgesConnected;
	Colore color;

	public:
		Summit() {}
};