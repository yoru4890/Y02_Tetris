#include "pch.h"
#include "TileI.h"

using namespace DX;
using namespace GameConstants;

TileI::TileI() : m_state(), m_bottom(4), m_right(1), m_left()
{
}

TileI::~TileI()
{
}

void TileI::Move(DX::StepTimer const& timer, DirectX::Keyboard::State const& kb, double& accumulatedTime, double& keyPressedTime)
{
	if (m_isStuckBySpaceBar) return;

	accumulatedTime += timer.GetElapsedSeconds();

	if (accumulatedTime >= TILE_DOWN_TIME)
	{
		accumulatedTime = 0.0;

		if (this->GetPosition().y < (BOARD_ROW - 1 - m_bottom) * TILE_SIZE)
		{
			this->SetPosition(this->GetPosition().x, this->GetPosition().y + TILE_SIZE);
		}
	}


	if (kb.D)
	{
		if (!keyPressedTime)
		{
			if (this->GetPosition().x < (BOARD_COL - 1 - m_right) * TILE_SIZE)
			{
				this->SetPosition(this->GetPosition().x + TILE_SIZE, this->GetPosition().y);
			}

		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				if (this->GetPosition().x < (BOARD_COL - 1 - m_right) * TILE_SIZE)
				{
					this->SetPosition(this->GetPosition().x + TILE_SIZE, this->GetPosition().y);
				}

				keyPressedTime = KEY_PRESSED_INTERVAL_TIME;
			}
		}

		keyPressedTime += timer.GetElapsedSeconds();
	}
	else if (kb.A)
	{
		if (!keyPressedTime)
		{
			if (this->GetPosition().x > TILE_SIZE)
			{
				this->SetPosition(this->GetPosition().x - TILE_SIZE, this->GetPosition().y);
			}			
		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				if (this->GetPosition().x > TILE_SIZE)
				{
					this->SetPosition(this->GetPosition().x - TILE_SIZE, this->GetPosition().y);
				}

				keyPressedTime = KEY_PRESSED_INTERVAL_TIME;
			}
		}

		keyPressedTime += timer.GetElapsedSeconds();

	}
	else if (kb.S)
	{
		if (!keyPressedTime)
		{
			if (this->GetPosition().y < (BOARD_ROW - 1 - m_bottom) * TILE_SIZE)
			{
				this->SetPosition(this->GetPosition().x, this->GetPosition().y + TILE_SIZE);
			}
		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				if (this->GetPosition().y < (BOARD_ROW - 1 - m_bottom) * TILE_SIZE)
				{
					this->SetPosition(this->GetPosition().x, this->GetPosition().y + TILE_SIZE);
				}

				keyPressedTime = KEY_PRESSED_INTERVAL_TIME;
			}
		}

		keyPressedTime += timer.GetElapsedSeconds();
	}
	else
	{
		keyPressedTime = 0.0;
	}
}

void TileI::Rotate(DirectX::Keyboard::State const& kb)
{
	if (kb.Q)
	{
		if (!m_isCCWRotate)
		{
			m_isCCWRotate = true;
			m_rotation -= PI / 2.0f;
			m_rotation = fmodf(m_rotation, 2.0f * PI);
		}
	}
	else
	{
		m_isCCWRotate = false;
	}

	if (kb.E)
	{
		if (!m_isCWRotate)
		{
			m_isCWRotate = true;
			m_rotation += PI / 2.0f;
			m_rotation = fmodf(m_rotation, 2.0f * PI);
		}
	}
	else
	{
		m_isCWRotate = false;
	}
}

bool TileI::IsStuck(DX::StepTimer const& timer)
{
	if (this->GetPosition().y >= (BOARD_ROW - 1 - m_bottom) * TILE_SIZE)
	{
		m_stuckTime += timer.GetElapsedSeconds();
	}

	if (m_stuckTime >= CAN_MOVE_TIME)
	{
		m_stuckTime = 0.0;
		return true;
	}

	return false;
}

void TileI::SpaceBar()
{
	m_isStuckBySpaceBar = true;

	while (this->GetPosition().y < (BOARD_ROW - 1 - m_bottom) * TILE_SIZE)
	{
		this->SetPosition(this->GetPosition().x, this->GetPosition().y + TILE_SIZE);
	}
}
