#include "pch.h"
#include "TileJ.h"

using namespace DX;
using namespace GameConstants;

DX::TileJ::TileJ() : 
	m_offset
	{{
		{0,0},{1,0},{1,1},{1,2},
		{0,2},{0,1},{1,1},{1,2},
		{2,2},{1,2},{1,1},{1,0},
		{2,0},{2,1},{1,1},{0,1}
	}}
{
}

DX::TileJ::~TileJ()
{
}

void DX::TileJ::InitTile()
{
	for (int i{}; i < 4; i++)
	{
		m_tilePos[i].first = START_X + m_offset[i].first;
		m_tilePos[i].second = START_Y + m_offset[i].second;
	}

	m_state = UP;
	m_isStuck = false;
	m_rotation = 0.f;
	m_stuckTime = 0.0;
	m_shape = ShapeTile::J_SHAPE;
}

void DX::TileJ::Rotate(DirectX::Keyboard::State const& kb, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	switch (m_state)
	{
		case State::UP:
			{
				if (kb.Q)
				{
					CCWRotate(m_board, 1, -1, 1);
				}
				else
				{
					m_isCCWRotate = false;
				}

				if (kb.E)
				{
					CWRotate(m_board, -1, 1, 1);
				}
				else
				{
					m_isCWRotate = false;
				}
			}
			break;
		case State::RIGHT:
			{
				if (kb.Q)
				{
					CCWRotate(m_board, 1, -1, -1);
				}
				else
				{
					m_isCCWRotate = false;
				}

				if (kb.E)
				{
					CWRotate(m_board, -1, -1, 1);
				}
				else
				{
					m_isCWRotate = false;
				}
			}
			break;
		case State::DOWN:
			{
				if (kb.Q)
				{
					CCWRotate(m_board, -1, 1, -1);
				}
				else
				{
					m_isCCWRotate = false;
				}

				if (kb.E)
				{
					CWRotate(m_board, 1, -1, -1);
				}
				else
				{
					m_isCWRotate = false;
				}
			}
			break;
		case State::LEFT:
			{
				if (kb.Q)
				{
					CCWRotate(m_board, -1, 1, 1);
				}
				else
				{
					m_isCCWRotate = false;
				}

				if (kb.E)
				{
					CWRotate(m_board, 1, 1, -1);
				}
				else
				{
					m_isCWRotate = false;
				}
			}
			break;
		default:
			break;
	}
}

void DX::TileJ::CCWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int moveDir, int xDir, int yDir)
{
	if (!m_isCCWRotate)
	{
		int nextState{ m_state };
		int moveCount{};

		nextState = (nextState + STATE_MAX_SIZE - 1) % STATE_MAX_SIZE;

		for (int i{}; i < 4; i++)
		{
			bool isCanRotate{ true };

			if (i < 3)
			{
				for (int j{}; j < 4; j++)
				{
					int nextX{ m_tilePos[0].first + m_offset[nextState * State::SIZE + j].first + i * moveDir };
					int nextY{ m_tilePos[0].second + m_offset[nextState * State::SIZE + j].second };

					if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

					if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
					{
						isCanRotate = false;
						break;
					}
				}

				if (isCanRotate)
				{
					for (int j{}; j < 4; j++)
					{
						m_tilePos[j].first = m_tilePos[0].first + m_offset[nextState * State::SIZE + j].first + i * moveDir;
						m_tilePos[j].second = m_tilePos[0].second + m_offset[nextState * State::SIZE + j].second;
					}

					m_isCCWRotate = true;
					m_rotation -= PI / 2.0f;
					m_rotation = fmodf(m_rotation, 2.0f * PI);

					m_position.x += xDir * TILE_SIZE / 2;
					m_position.x += moveDir * moveCount * TILE_SIZE;
					m_position.y += yDir * TILE_SIZE / 2;

					m_state = nextState;
					break;
				}

				moveCount++;
			}
			else
			{
				for (int j{}; j < 4; j++)
				{
					int nextX{ m_tilePos[0].first + m_offset[nextState * State::SIZE + j].first - moveDir };
					int nextY{ m_tilePos[0].second + m_offset[nextState * State::SIZE + j].second };

					if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

					if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
					{
						isCanRotate = false;
						break;
					}
				}

				if (isCanRotate)
				{
					for (int j{}; j < 4; j++)
					{
						m_tilePos[j].first = m_tilePos[0].first + m_offset[nextState * State::SIZE + j].first - moveDir;
						m_tilePos[j].second = m_tilePos[0].second + m_offset[nextState * State::SIZE + j].second;
					}

					m_isCCWRotate = true;
					m_rotation -= PI / 2.0f;
					m_rotation = fmodf(m_rotation, 2.0f * PI);

					m_position.x += xDir * TILE_SIZE / 2;
					m_position.x -= moveDir * TILE_SIZE;
					m_position.y += yDir * TILE_SIZE / 2;

					m_state = nextState;
					break;
				}
			}
		}
	}
}

void DX::TileJ::CWRotate(const const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int moveDir, int xDir, int yDir)
{
	if (!m_isCWRotate)
	{
		int nextState{ m_state };
		int moveCount{};

		nextState = (nextState + 1) % STATE_MAX_SIZE;

		for (int i{}; i < 4; i++)
		{
			bool isCanRotate{ true };

			if (i < 3)
			{
				for (int j{}; j < 4; j++)
				{
					int nextX{ m_tilePos[0].first + m_offset[nextState * State::SIZE + j].first + i * moveDir };
					int nextY{ m_tilePos[0].second + m_offset[nextState * State::SIZE + j].second };

					if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

					if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
					{
						isCanRotate = false;
						break;
					}
				}

				if (isCanRotate)
				{
					for (int j{}; j < 4; j++)
					{
						m_tilePos[j].first = m_tilePos[0].first + m_offset[nextState * State::SIZE + j].first + i * moveDir;
						m_tilePos[j].second = m_tilePos[0].second + m_offset[nextState * State::SIZE + j].second;
					}

					m_isCWRotate = true;
					m_rotation += PI / 2.0f;
					m_rotation = fmodf(m_rotation, 2.0f * PI);

					m_position.x += xDir * TILE_SIZE / 2;
					m_position.x += moveDir * moveCount * TILE_SIZE;
					m_position.y += yDir * TILE_SIZE / 2;

					m_state = nextState;
					break;
				}

				moveCount++;
			}
			else
			{
				for (int j{}; j < 4; j++)
				{
					int nextX{ m_tilePos[0].first + m_offset[nextState * State::SIZE + j].first - moveDir };
					int nextY{ m_tilePos[0].second + m_offset[nextState * State::SIZE + j].second };

					if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

					if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
					{
						isCanRotate = false;
						break;
					}
				}

				if (isCanRotate)
				{
					for (int j{}; j < 4; j++)
					{
						m_tilePos[j].first = m_tilePos[0].first + m_offset[nextState * State::SIZE + j].first - moveDir;
						m_tilePos[j].second = m_tilePos[0].second + m_offset[nextState * State::SIZE + j].second;
					}

					m_isCWRotate = true;
					m_rotation += PI / 2.0f;
					m_rotation = fmodf(m_rotation, 2.0f * PI);

					m_position.x += xDir * TILE_SIZE / 2;
					m_position.x -= moveDir * TILE_SIZE;
					m_position.y += yDir * TILE_SIZE / 2;

					m_state = nextState;
					break;
				}
			}

		}
	}
}

void DX::TileJ::SpaceBar(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
}

void DX::TileJ::SetGhost(Actor* const& m_ghost, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
}
