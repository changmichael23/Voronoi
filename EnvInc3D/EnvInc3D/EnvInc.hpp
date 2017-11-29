#pragma once
#include "Graph.hpp"
class EnvInc
{
	private:
		Graph *graph;
		std::vector<Point> pointsLeft;
	public:
		EnvInc() 
		{
			graph = nullptr;
		}
		EnvInc(Graph *g, std::vector<Point> p)
		{
			graph = g;
			pointsLeft = p;
		}
		Graph *getGraph()
		{
			return graph;
		}
		void setGraph(Graph * g)
		{
			graph = g;
		}
		
		void initializeGraph()
		{
			for (int i = 0; i < 4; i++)
			{
				graph->getSummitList()->push_back(*new Summit(pointsLeft.back()));
				pointsLeft.pop_back();
			}
			for (int i = 0; i < 3; i++)
			{
				for(int j = i+1; j < 4 ; j++)
				{ 
					std::vector<Point> tmp;
					tmp.push_back(graph->getSummitList()->at(i).getPoint());
					tmp.push_back(graph->getSummitList()->at(j).getPoint());


					Edge tmpEdge = *new Edge(tmp);
					graph->getSummitList()->at(i).getEdgesConnected()->push_back(tmpEdge);
					graph->getSummitList()->at(j).getEdgesConnected()->push_back(tmpEdge);
					graph->getEdgeList()->push_back(tmpEdge);

				}
			}

			std::vector<Point> tmp;
			tmp.push_back(graph->getSummitList()->at(0).getPoint());
			tmp.push_back(graph->getSummitList()->at(1).getPoint());
			tmp.push_back(graph->getSummitList()->at(2).getPoint());

			Face tmpFace = *new Face(tmp);
			

			graph->getEdgeList()->at(0).getFacesConnected()->push_back(tmpFace);
			graph->getEdgeList()->at(1).getFacesConnected()->push_back(tmpFace);
			graph->getEdgeList()->at(3).getFacesConnected()->push_back(tmpFace);
			graph->getFaceList()->push_back(tmpFace);
			tmp.clear();

			tmp.push_back(graph->getSummitList()->at(0).getPoint());
			tmp.push_back(graph->getSummitList()->at(1).getPoint());
			tmp.push_back(graph->getSummitList()->at(3).getPoint());

			tmpFace = *new Face(tmp);
			

			graph->getEdgeList()->at(0).getFacesConnected()->push_back(tmpFace);
			graph->getEdgeList()->at(2).getFacesConnected()->push_back(tmpFace);
			graph->getEdgeList()->at(4).getFacesConnected()->push_back(tmpFace);
			graph->getFaceList()->push_back(tmpFace);
			tmp.clear();

			tmp.push_back(graph->getSummitList()->at(0).getPoint());
			tmp.push_back(graph->getSummitList()->at(2).getPoint());
			tmp.push_back(graph->getSummitList()->at(3).getPoint());

			tmpFace = *new Face(tmp);

			graph->getEdgeList()->at(1).getFacesConnected()->push_back(tmpFace);
			graph->getEdgeList()->at(2).getFacesConnected()->push_back(tmpFace);
			graph->getEdgeList()->at(5).getFacesConnected()->push_back(tmpFace);
			graph->getFaceList()->push_back(tmpFace);
			tmp.clear();

			


			tmp.push_back(graph->getSummitList()->at(1).getPoint());
			tmp.push_back(graph->getSummitList()->at(2).getPoint());
			tmp.push_back(graph->getSummitList()->at(3).getPoint());

			tmpFace = *new Face(tmp);
			

			graph->getEdgeList()->at(3).getFacesConnected()->push_back(tmpFace);
			graph->getEdgeList()->at(4).getFacesConnected()->push_back(tmpFace);
			graph->getEdgeList()->at(5).getFacesConnected()->push_back(tmpFace);

			graph->getFaceList()->push_back(tmpFace);


		}
};