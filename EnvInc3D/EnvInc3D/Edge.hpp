#pragma once
#include "Face.hpp"
#include <vector>
class Edge
{
	private:
		
		std::vector<Face> facesConnected;
		Colore color;

	public:
		Edge() {}
		Edge(Colore c)
		{
			color = c;
		}
		std::vector<Face> getFacesConnected()
		{
			return facesConnected;
		}
		
};