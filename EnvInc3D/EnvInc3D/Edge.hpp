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
		Edge(Colore c, std::vector<Face> f)
		{
			color = c;
			facesConnected = f;
		}
		void setColor(const Colore & c)
		{
			color = c;
		}
		const Colore & getColor() const
		{
			return color;
		}
		void setFacesConnected(const std::vector<Face> & f)
		{
			facesConnected = f;
		}
		const std::vector<Face> & getFacesConnected() const
		{
			return facesConnected;
		}
		
};