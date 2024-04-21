#pragma once
#include "TileBase.h"

namespace DX
{
	class TileI : public TileBase
	{
	private:
		const int STATE_MAX_SIZE{ 2 };

		int m_state;
		int m_bottom;
		int m_right;
		int m_left;
		
	public:
		TileI();
		virtual ~TileI();

		virtual void Move(DX::StepTimer const& timer, DirectX::Keyboard::State const& kb, double& accumulatedTime, double& keyPressedTime) override;
		virtual void Rotate(DirectX::Keyboard::State const& kb) override;
		virtual bool IsStuck(DX::StepTimer const& timer) override;
		virtual void SpaceBar() override;
	};
}

