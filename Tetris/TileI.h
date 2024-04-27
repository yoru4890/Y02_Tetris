#pragma once
#include "TileBase.h"

namespace DX
{
	class TileI : public TileBase
	{
	public:
		TileI();
		virtual ~TileI();

		virtual void InitTile(GameConstants::ShapeTile shapeTile) override;
		virtual void Rotate(DirectX::Keyboard::State const& kb, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
		virtual void CCWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int xDir = 0, int yDir = 0) override;
		virtual void CWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int xDir = 0, int yDir = 0) override;
	};
}

