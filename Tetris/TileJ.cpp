#include "pch.h"
#include "TileJ.h"

using namespace DX;
using namespace GameConstants;

DX::TileJ::TileJ()
{
}

DX::TileJ::~TileJ()
{
}

void DX::TileJ::InitTile(GameConstants::ShapeTile shapeTile)
{
	TileBase::InitTile(shapeTile);
}

void DX::TileJ::Rotate(DirectX::Keyboard::State const& kb, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	if (kb.Q)
	{
		switch (m_state)
		{
			case UP:
				CCWRotate(m_board);
				break;
			case RIGHT:
				CCWRotate(m_board, -1);
				break;
			case DOWN:
				CCWRotate(m_board);
				break;
			case LEFT:
				CCWRotate(m_board, 1);
				break;
			case SIZE:
				break;
			default:
				break;
		}
	}
	else
	{
		m_isCCWRotate = false;
	}

	if (kb.E || kb.Up)
	{
		switch (m_state)
		{
			case UP:
				CWRotate(m_board);
				break;
			case RIGHT:
				CWRotate(m_board, 1);
				break;
			case DOWN:
				CWRotate(m_board);
				break;
			case LEFT:
				CWRotate(m_board, -1);
				break;
			case SIZE:
				break;
			default:
				break;
		}
	}
	else
	{
		m_isCWRotate = false;
	}
}

void DX::TileJ::CCWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int xDir, int yDir)
{
	if (!m_isCCWRotate)
	{
		for (int i{}; i < m_tilePos.size(); i++)
		{
			m_nextTilePos[i].first = (m_tilePos[i].second - basePos.second - ROTATION_OFFSET) + basePos.first + ROTATION_OFFSET;
			m_nextTilePos[i].second = (m_tilePos[i].first - basePos.first - ROTATION_OFFSET) * -1 + basePos.second + ROTATION_OFFSET;
		}

		for (int i{}; i < 2; i++)
		{
			bool isCanRotate{ true };

			for (const auto& e : m_nextTilePos)
			{
				int nextX = e.first + i * xDir;
				int nextY = e.second;

				if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

				if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
				{
					isCanRotate = false;
					break;
				}
			}

			if (isCanRotate)
			{
				for (int j{}; j < m_tilePos.size(); j++)
				{
					m_tilePos[j].first = m_nextTilePos[j].first + i * xDir;
					m_tilePos[j].second = m_nextTilePos[j].second;
				}

				m_isCCWRotate = true;
				m_rotation -= PI / 2.0f;
				m_rotation = fmodf(m_rotation, 2.0f * PI);

				m_position.x += TILE_SIZE * i * xDir;
				m_position.y += TILE_SIZE * i * yDir;
				m_state = (m_state + State::SIZE - 1) % State::SIZE;
				basePos.first += i * xDir;
				basePos.second += i * yDir;
				break;
			}
		}
	}
}

void DX::TileJ::CWRotate(const const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int xDir, int yDir)
{
	if (!m_isCWRotate)
	{
		for (int i{}; i < m_tilePos.size(); i++)
		{
			m_nextTilePos[i].first = (m_tilePos[i].second - basePos.second - ROTATION_OFFSET) * -1 + basePos.first + ROTATION_OFFSET;
			m_nextTilePos[i].second = (m_tilePos[i].first - basePos.first - ROTATION_OFFSET) + basePos.second + ROTATION_OFFSET;
		}

		for (int i{}; i < 2; i++)
		{
			bool isCanRotate{ true };


			for (const auto& e : m_nextTilePos)
			{
				int nextX = e.first + i * xDir;
				int nextY = e.second;

				if (nextX < 0 || nextX >= BOARD_COL || nextY < 0 || nextY >= BOARD_ROW) continue;

				if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
				{
					isCanRotate = false;
					break;
				}
			}

			if (isCanRotate)
			{
				for (int j{}; j < m_tilePos.size(); j++)
				{
					m_tilePos[j].first = m_nextTilePos[j].first + i * xDir;
					m_tilePos[j].second = m_nextTilePos[j].second;
				}

				m_isCWRotate = true;
				m_rotation += PI / 2.0f;
				m_rotation = fmodf(m_rotation, 2.0f * PI);

				m_position.x += TILE_SIZE * i * xDir;
				m_position.y += TILE_SIZE * i * yDir;
				m_state = (m_state + 1) % State::SIZE;
				basePos.first += i * xDir;
				basePos.second += i * yDir;
				break;
			}
		}
	}
}
