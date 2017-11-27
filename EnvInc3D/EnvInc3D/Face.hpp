#pragma once
#include "Colore.hpp"


class Face{


	private:
		Colore color;


	public:
		Face() {}
		Face(Colore c)
		{
			color = c;
		}
		const Colore & getColor() const
		{
			return color;
		}
		void setColor(const Colore & c)
		{
			color = c;
		}
};