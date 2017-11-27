#pragma once
#include "Summit.hpp"

class Graph {
	private:
	std::vector<Summit> summitList;
	
	
	public:
		Graph() {}
		std::vector<Summit> getSummitList()
		{
			return summitList;
		}




};