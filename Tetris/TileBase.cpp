#include "pch.h"

using namespace DX;
using namespace GameConstants;

DX::TileBase::TileBase() : 
	START_POS
	{ {
		{},{},{},{},
		{},{},{},{},
		{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 1.0f, 3.0f }, // I_SHAPE
		{ 1.0f, 1.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f }, { 3.0f, 2.0f },	// J_SHAPE
		{ 2.0f, 1.0f }, { 2.0f, 2.0f }, { 1.0f, 2.0f }, { 0.0f, 2.0f }, // L_SHAPE
		{ 1.0f, 1.0f }, { 2.0f, 1.0f }, { 2.0f, 2.0f }, { 1.0f, 2.0f }, // O_SHAPE
		{ 2.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 0.0f, 2.0f }, // S_SHAPE
		{ 1.0f, 1.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f }, // T_SHAPE
		{ 1.0f, 1.0f }, { 2.0f, 1.0f }, { 2.0f, 2.0f }, { 3.0f, 2.0f }	// Z_SHAPE
	} }
{
}

DX::TileBase::~TileBase()
{
}

void DX::TileBase::InitTile(GameConstants::ShapeTile shapeTile)
{
	basePos = { -2.0f + START_X,-2.0f + START_Y };
	m_state = UP;
	m_isStuck = false;
	m_rotation = 0.f;
	m_stuckTime = 0.0;
	m_isStuckBySpaceBar = false;
	m_shape = shapeTile;

	for (int i{}; i < m_tilePos.size(); i++)
	{
		m_tilePos[i] = START_POS[m_tilePos.size() * static_cast<int>(m_shape) + i];
		m_tilePos[i].first += START_X - 2;
		m_tilePos[i].second += START_Y - 2;
	}
 }

void DX::TileBase::Move(
	const double& elapsedSeconds,
	DirectX::Keyboard::State const& kb,
	double& accumulatedTime, double& keyPressedTime,
	const std::array<GameConstants::ShapeTile,
	GameConstants::BOARD_SIZE>& m_board)
{
	if (m_isStuckBySpaceBar)
	{
		m_stuckTime += elapsedSeconds;

		if (m_stuckTime >= TILE_DOWN_TIME / 4)
		{
			m_isStuck = true;
		}
		return;
	}

	accumulatedTime += elapsedSeconds;


	if (accumulatedTime >= TILE_DOWN_TIME)
	{
		accumulatedTime = 0.0;

		if (!MoveDown(m_board))
		{
			m_isStuck = true;
			return;
		}
	}

	if (kb.D || kb.Right)
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

		keyPressedTime += elapsedSeconds;
	}
	else if (kb.A || kb.Left)
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

		keyPressedTime += elapsedSeconds;

	}
	else if (kb.S || kb.Down)
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

		keyPressedTime += elapsedSeconds;
	}
	else
	{
		keyPressedTime = 0.0;
	}
}

bool DX::TileBase::MoveDown(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	bool isCanMove{ true };

	for (int i{}; i < 4; i++)
	{
		int nextX = m_tilePos[i].first;
		int nextY = m_tilePos[i].second + 1;

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
			e.second++;
		}

		basePos.second++;

		return true;
	}
	else
	{
		return false;
	}
}

void DX::TileBase::MoveRight(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	bool isCanMove{ true };

	for (int i{}; i < 4; i++)
	{
		int nextX = m_tilePos[i].first + 1;
		int nextY = m_tilePos[i].second;

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
			e.first++;
		}

		basePos.first++;
	}
}

void DX::TileBase::MoveLeft(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	bool isCanMove{ true };

	for (int i{}; i < 4; i++)
	{
		int nextX = m_tilePos[i].first - 1;
		int nextY = m_tilePos[i].second;

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
			e.first--;
		}

		basePos.first--;
	}
}

void DX::TileBase::SpaceBar(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	m_isStuckBySpaceBar = true;
	bool isCanMove{ true };

	while (isCanMove)
	{
		for (const auto& pos : m_tilePos)
		{
			int nextX = pos.first;
			int nextY = pos.second + 1;

			if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

			if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
			{
				isCanMove = false;
				break;
			}
		}

		if (isCanMove)
		{
			for (auto& pos : m_tilePos)
			{
				pos.second += 1;
			}

			m_position.y += TILE_SIZE;
		}
	}
}

void DX::TileBase::SetGhost(Actor* const& m_ghost, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	m_ghost->SetRotation(m_rotation);

	int moveCount{0};

	bool isCanMove{ true };

	while (isCanMove)
	{
		moveCount++;

		for (int i{}; i < 4; i++)
		{
			int nextX = m_tilePos[i].first;
			int nextY = m_tilePos[i].second + moveCount;

			if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

			if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
			{
				isCanMove = false;
				break;
			}
		}
	}

	m_ghost->SetPosition(m_position.x, m_position.y + TILE_SIZE * (moveCount-1));
}

void DX::TileBase::StackBoard(std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	for (const auto& e : m_tilePos)
	{
		m_board[e.second * GameConstants::BOARD_COL + e.first] = m_shape;
	}
}

bool DX::TileBase::IsDie(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	for (const auto& tile : m_tilePos)
	{
		if (m_board[tile.second * BOARD_COL + tile.first] != ShapeTile::BLANK)
		{
			return true;
		}
	}
	return false;
}
