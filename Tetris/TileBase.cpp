#include "pch.h"

DX::TileBase::TileBase()
{
}

DX::TileBase::~TileBase()
{
}

void DX::TileBase::InitTile()
{
}


void DX::TileBase::SetStuckBySpaceBar(bool isStuck)
{
	m_isStuckBySpaceBar = isStuck;
}

void DX::TileBase::StackBoard(std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	for (const auto& e : m_tilePos)
	{
		m_board[e.second * GameConstants::BOARD_COL + e.first] = m_shape;
	}
}
