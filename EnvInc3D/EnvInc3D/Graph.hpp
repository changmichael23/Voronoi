#pragma once
#include "Summit.hpp"

class Graph {
private:
	std::vector<Summit> *summitList;
	std::vector<Edge> *edgeList;
	std::vector<Face> *faceList;


public:
	Graph() {
		summitList = new std::vector<Summit>;
		edgeList = new std::vector<Edge>;
		faceList = new std::vector<Face>;
	}
	Graph(std::vector<Summit> *s, std::vector<Face>* f, std::vector<Edge>* e)
	{
		summitList = s;
		faceList = f;
		edgeList = e;
	}
	void setSummitList(std::vector<Summit>* s)
	{
		summitList = s;
	}
	std::vector<Summit> * getSummitList()
	{
		return summitList;
	}
	void setFaceList(std::vector<Face> * f)
	{
		faceList = f;
	}
	std::vector<Face> * getFaceList()
	{
		return faceList;
	}
	void setEdgeList(std::vector<Edge>* e)
	{
		edgeList = e;
	}
	std::vector<Edge> * getEdgeList()
	{
		return edgeList;
	}

	Point getRandomInsidePoint(Face f)
	{
		for (int i = 0; i < summitList->size(); i++)
		{
			bool isSame = false;
			for (int j = 0; j < 3; j++)
			{
				if (summitList->at(i).getPoint().x == f.getPoints()[j].x && summitList->at(i).getPoint().y == f.getPoints()[j].y
					&& summitList->at(i).getPoint().z == f.getPoints()[j].z)
				{
					isSame = true;
				}
			}
			if (!isSame)
			{
				return summitList->at(i).getPoint();
			}
		}
	}

	bool isVisible(Face f, Point p)
	{
		Point tmp = getRandomInsidePoint(f);
		std::vector<float> v1;       // Vecteur (f0,pointInt)
		std::vector<float> v2;		 // Vecteur (f0,nouveauPoint)

		v1.push_back(tmp.x - f.getPoints()[0].x);
		v1.push_back(tmp.y - f.getPoints()[0].y);
		v1.push_back(tmp.z - f.getPoints()[0].z);

		v2.push_back(p.x - f.getPoints()[0].x);
		v2.push_back(p.y - f.getPoints()[0].y);
		v2.push_back(p.z - f.getPoints()[0].z);

		std::vector<float> ab;       // Vecteur (f0,pointInt)
		std::vector<float> ac;		 // Vecteur (f0,nouveauPoint)

		ab.push_back(f.getPoints()[1].x - f.getPoints()[0].x);
		ab.push_back(f.getPoints()[1].y - f.getPoints()[0].y);
		ab.push_back(f.getPoints()[1].z - f.getPoints()[0].z);

		ac.push_back(f.getPoints()[2].x - f.getPoints()[0].x);
		ac.push_back(f.getPoints()[2].y - f.getPoints()[0].y);
		ac.push_back(f.getPoints()[2].z - f.getPoints()[0].z);


		std::vector<float> normalExt = vectorialProduct(ab, ac);
		bool sign = scalarProduct(normalExt, v1) >= 0.0f ? true : false;
		bool sign1 = scalarProduct(normalExt, v2) >= 0.0f ? true : false;
		if (sign == sign1)
		{
			return false;
		}
		return true;

	}

	std::vector<float> vectorialProduct(std::vector<float> f1, std::vector<float> f2)
	{
		if (f1.size() != 3 || f2.size() != 3)
		{
			std::cout << "Erreur taille produit vectoriel" << std::endl;
			return f1;
		}
		else
		{
			std::vector<float> tmp;
			tmp.push_back(f1[1] * f2[2] - f1[2] * f2[1]);
			tmp.push_back(f1[2] * f2[0] - f1[0] * f2[2]);
			tmp.push_back(f1[0] * f2[1] - f1[1] * f2[0]);
			return tmp;
		}
	}

	float scalarProduct(std::vector<float> f1, std::vector<float> f2)
	{
		float tmp;
		for (int i = 0; i < f1.size(); i++)
		{
			tmp += f1[i] + f2[i];
		}
		return tmp;
	}

	void visualiseGraph()
	{
		for (int i = 0; i < summitList->size(); i++)
		{
			std::cout << "Summit : " << std::endl;
			std::cout << summitList->at(i).getPoint().toString() << std::endl;
			
			for (int j = 0; j < summitList->at(i).getEdgesConnected()->size(); j++)
			{
				std::cout << "Edges : " << std::endl;
				std::cout << summitList->at(i).getEdgesConnected()->at(j).getPoints()[0].toString() << std::endl;
				std::cout << summitList->at(i).getEdgesConnected()->at(j).getPoints()[1].toString() << std::endl;
				std::cout << "Faces : " << std::endl;
				for (int k = 0; k < 3; k++)
				{
					std::cout << summitList->at(i).getEdgesConnected()->at(j).getFacesConnected()->at(0).getPoints()[k].toString() << " ";
				}
				std::cout << "" << std::endl;
				std::cout << "Faces 1: " << std::endl;
				for (int k = 0; k < 3; k++)
				{
					std::cout << summitList->at(i).getEdgesConnected()->at(j).getFacesConnected()->at(1).getPoints()[k].toString() << " ";
				}
				std::cout << "" << std::endl;

			}
		}
	}
};