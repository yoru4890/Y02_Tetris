#pragma once
#include "Actor.h"

namespace DX
{
	class Tile : public Actor // TODO
	{
	public:
		Tile();
		virtual ~Tile();

		void Move();
		void Rotate();
		bool IsStuck();
		void SpaceBar();
	};
}



