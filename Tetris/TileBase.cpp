#include "pch.h"

DX::TileBase::TileBase()
{
}

DX::TileBase::~TileBase()
{
}

bool DX::TileBase::IsStuck(DX::StepTimer const& timer, std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	return false;
}

void DX::TileBase::InitTile()
{
}


void DX::TileBase::SetStuckBySpaceBar(bool isStuck)
{
	m_isStuckBySpaceBar = isStuck;
}
