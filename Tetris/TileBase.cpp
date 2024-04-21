#include "pch.h"

DX::TileBase::TileBase()
{
}

DX::TileBase::~TileBase()
{
}

void DX::TileBase::Move(DX::StepTimer const& timer, DirectX::Keyboard::State const& kb, double& accumulatedTime, double& keyPressedTime)
{
}

void DX::TileBase::Rotate(DirectX::Keyboard::State const& kb)
{
}

bool DX::TileBase::IsStuck(DX::StepTimer const& timer)
{
	return false;
}

void DX::TileBase::SpaceBar()
{
}

void DX::TileBase::SetStuckBySpaceBar(bool isStuck)
{
	m_isStuckBySpaceBar = isStuck;
}
