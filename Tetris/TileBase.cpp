#include "pch.h"

using namespace DX;
using namespace GameConstants;

DX::TileBase::TileBase() : 
	START_POS
	{ {
		{},{},{},{},
		{},{},{},{},
		{ 1.0f, 0.0f }, { 2.0f, 0.0f }, { 3.0f, 0.0f }, { 4.0f, 0.0f },
		{ 1.0f, 1.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f }, { 3.0f, 2.0f },
		{ 2.0f, 1.0f }, { 2.0f, 2.0f }, { 1.0f, 2.0f }, { 0.0f, 2.0f },
		{ 1.0f, 1.0f }, { 2.0f, 1.0f }, { 2.0f, 2.0f }, { 1.0f, 2.0f },
		{ 2.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 0.0f, 2.0f },
		{ 1.0f, 1.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f },
		{ 1.0f, 1.0f }, { 2.0f, 1.0f }, { 2.0f, 2.0f }, { 3.0f, 2.0f }
	} }
{
}

DX::TileBase::~TileBase()
{
}

void DX::TileBase::InitTile()
{
	m_state = UP;
	m_isStuck = false;
	m_rotation = 0.f;
	m_stuckTime = 0.0;

	for (int i{}; i < State::SIZE; i++)
	{
		m_tilePos[i] = START_POS[State::SIZE * static_cast<int>(m_shape) + i];
	}
}

void DX::TileBase::Rotate(DirectX::Keyboard::State const& kb, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{

}

void DX::TileBase::CCWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int moveDir, int xDir, int yDir)
{
}

void DX::TileBase::CWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int moveDir, int xDir, int yDir)
{
}

void DX::TileBase::Move(
	DX::StepTimer const& timer,
	DirectX::Keyboard::State const& kb,
	double& accumulatedTime, double& keyPressedTime,
	const std::array<GameConstants::ShapeTile,
	GameConstants::BOARD_SIZE>& m_board)
{
	if (m_isStuckBySpaceBar)
	{
		m_stuckTime += timer.GetElapsedSeconds();

		if (m_stuckTime >= TILE_DOWN_TIME / 4)
		{
			m_isStuck = true;
		}
		return;
	}

	accumulatedTime += timer.GetElapsedSeconds();


	if (accumulatedTime >= TILE_DOWN_TIME)
	{
		accumulatedTime = 0.0;

		MoveDown(m_board);
	}

	if (kb.D)
	{
		if (!keyPressedTime)
		{
			MoveRight(m_board);
		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				MoveRight(m_board);

				keyPressedTime = KEY_PRESSED_INTERVAL_TIME;
			}
		}

		keyPressedTime += timer.GetElapsedSeconds();
	}
	else if (kb.A)
	{
		if (!keyPressedTime)
		{
			MoveLeft(m_board);
		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				MoveLeft(m_board);

				keyPressedTime = KEY_PRESSED_INTERVAL_TIME;
			}
		}

		keyPressedTime += timer.GetElapsedSeconds();

	}
	else if (kb.S)
	{
		if (!keyPressedTime)
		{
			MoveDown(m_board);
		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				MoveDown(m_board);

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

void DX::TileBase::MoveDown(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	bool isCanMove{ true };

	for (int i{}; i < 4; i++)
	{
		int nextX{ m_tilePos[i].first };
		int nextY{ m_tilePos[i].second + 1 };

		if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

		if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
		{
			isCanMove = false;
			break;
		}
	}

	if (isCanMove)
	{
		m_position.y += TILE_SIZE;
		for (auto& e : m_tilePos)
		{
			e.second += 1;
		}
	}
	else
	{
		m_isStuck = true;
	}
}

void DX::TileBase::MoveRight(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	bool isCanMove{ true };

	for (int i{}; i < 4; i++)
	{
		int nextX{ m_tilePos[i].first + 1 };
		int nextY{ m_tilePos[i].second };

		if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

		if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
		{
			isCanMove = false;
			break;
		}
	}

	if (isCanMove)
	{
		m_position.x += TILE_SIZE;
		for (auto& e : m_tilePos)
		{
			e.first += 1;
		}
	}
	else
	{
		m_isStuck = true;
	}
}

void DX::TileBase::MoveLeft(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	bool isCanMove{ true };

	for (int i{}; i < 4; i++)
	{
		int nextX{ m_tilePos[i].first - 1 };
		int nextY{ m_tilePos[i].second };

		if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

		if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
		{
			isCanMove = false;
			break;
		}
	}

	if (isCanMove)
	{
		m_position.x -= TILE_SIZE;
		for (auto& e : m_tilePos)
		{
			e.first -= 1;
		}
	}
	else
	{
		m_isStuck = true;
	}
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
