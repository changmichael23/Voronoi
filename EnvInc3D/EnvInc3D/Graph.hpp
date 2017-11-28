#pragma once
#include "Summit.hpp"

class Graph {
	private:
	std::vector<Summit> summitList;
	
	
	public:
		Graph() {}
		Graph(std::vector<Summit> s)
		{
			summitList = s;
		}
		void setSummitList(const std::vector<Summit> & s)
		{
			summitList = s;
		}
		const std::vector<Summit> & getSummitList() const
		{
			return summitList;
		}




};