#pragma once
#include "TileBase.h"

namespace DX
{
	class TileI : public TileBase
	{
	private:
		enum State
		{
			UP,
			RIGHT
		};


		const int STATE_MAX_SIZE{ 2 };

		int m_state;
		
	public:
		TileI();
		virtual ~TileI();

		virtual void Move(
			DX::StepTimer const& timer, 
			DirectX::Keyboard::State const& kb, 
			double& accumulatedTime, 
			double& keyPressedTime,
			std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board
		) override;
		virtual void Rotate(DirectX::Keyboard::State const& kb, std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
		virtual bool IsStuck(DX::StepTimer const& timer, std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
		virtual void InitTile() override; 
		virtual void SpaceBar(std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
	};
}

