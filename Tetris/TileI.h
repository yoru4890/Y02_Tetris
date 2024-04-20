#pragma once
#include "TileBase.h"

namespace DX
{
	class TileI : public TileBase
	{
	private:
		const int STATE_MAX_SIZE{ 3 };

		int m_state;
		
	public:
		TileI();
		virtual ~TileI();

		virtual bool Move(DX::StepTimer const& timer, DirectX::Keyboard::State const& kb, double& accumulatedTime, double& keyPressedTime) override;
		virtual void Rotate() override;
		virtual bool IsStuck() override;
		virtual void SpaceBar() override;
	};
}

