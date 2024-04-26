#pragma once
#include "TileBase.h"

namespace DX
{
	class TileI : public TileBase
	{
	private:

		const int STATE_MAX_SIZE{ 2 };
		std::array<std::pair<int, int>, 4> m_offset;
		
	public:
		TileI();
		virtual ~TileI();

		virtual void InitTile() override;
		virtual void Rotate(DirectX::Keyboard::State const& kb, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
		virtual void SpaceBar(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
		virtual void SetGhost(Actor* const& m_ghost, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
	};
}

