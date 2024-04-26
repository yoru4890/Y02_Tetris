#pragma once
#include "TileBase.h"
namespace DX
{
	class TileJ : public TileBase
	{
	private:

		const int STATE_MAX_SIZE{ 4 };
		std::array<std::pair<int, int>, 16> m_offset;

	public:
		TileJ();
		virtual ~TileJ();

		virtual void InitTile() override;
		virtual void Rotate(DirectX::Keyboard::State const& kb, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
		virtual void CCWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int moveDir, int xDir, int yDir);
		virtual void CWRotate(const const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int moveDir, int xDir, int yDir);
		virtual void SpaceBar(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
		virtual void SetGhost(Actor* const& m_ghost, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) override;
	};
}

