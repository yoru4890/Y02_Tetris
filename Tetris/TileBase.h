#pragma once
#include "Actor.h"
namespace DX
{
	class TileBase : public Actor
	{
	public:
		TileBase() {};
		virtual ~TileBase() {};

		virtual bool Move(DX::StepTimer const& timer, DirectX::Keyboard::State const& kb, double& accumulatedTime, double& keyPressedTime) { return false; };
		virtual void Rotate() {};
		virtual bool IsStuck() { return false; };
		virtual void SpaceBar() {};
	};

}

