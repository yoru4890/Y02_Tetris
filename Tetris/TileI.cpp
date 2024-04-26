#include "pch.h"
#include "TileI.h"

using namespace DX;
using namespace GameConstants;

TileI::TileI() : m_offset{ { 
	{ 0,0 }, { 0,1 }, { 0,2 }, { 0,3 }
	} }
{
}

TileI::~TileI()
{
}

void DX::TileI::InitTile()
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
	m_shape = ShapeTile::I_SHAPE;
}

void TileI::Rotate(DirectX::Keyboard::State const& kb, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	switch (m_state)
	{
		case State::UP:
			{
				if (kb.Q)
				{
					if (!m_isCCWRotate)
					{
						int nextX{};
						int nextY{ m_tilePos[1].second };
						int moveCount{};

						for (int i{}; i < 4; i++)
						{
							bool isRotate{ true };
							if (i < 3)
							{
								nextX = m_tilePos[1].first - 2 + i;
								if (nextX < 0)
								{
									moveCount++;
									continue;
								}
								for (int j{}; j < 4; j++)
								{
									if (m_board[nextY * BOARD_COL + nextX + j] != ShapeTile::BLANK)
									{
										isRotate = false;
										break;
									}
								}

								if (isRotate)
								{
									for (int j{}; j < 4; j++)
									{
										m_tilePos[j].first = nextX + j;
										m_tilePos[j].second = nextY;
									}

									m_isCCWRotate = true;
									m_rotation -= PI / 2.0f;
									m_rotation = fmodf(m_rotation, 2.0f * PI);

									m_position.x -= TILE_SIZE / 2;
									m_position.x += moveCount * TILE_SIZE;
									m_position.y -= TILE_SIZE / 2;

									m_state++;
									m_state %= STATE_MAX_SIZE;
									break;
								}

								moveCount++;
							}
							else
							{
								nextX = m_tilePos[1].first - 3;
								for (int j{}; j < 4; j++)
								{
									if (m_board[nextY * BOARD_COL + nextX + j] != ShapeTile::BLANK)
									{
										isRotate = false;
										break;
									}
								}

								if (isRotate)
								{
									for (int j{}; j < 4; j++)
									{
										m_tilePos[j].first = nextX + j;
										m_tilePos[j].second = nextY;
									}

									m_isCCWRotate = true;
									m_rotation -= PI / 2.0f;
									m_rotation = fmodf(m_rotation, 2.0f * PI);

									m_position.x -= TILE_SIZE / 2;
									m_position.x -= TILE_SIZE;
									m_position.y -= TILE_SIZE / 2;

									m_state++;
									m_state %= STATE_MAX_SIZE;
									break;
								}
							}

						}
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
						int nextX{};
						int nextY{ m_tilePos[1].second };
						int moveCount{};

						for (int i{}; i < 4; i++)
						{
							bool isRotate{ true };
							if (i < 3)
							{
								nextX = m_tilePos[1].first - 2 + i;
								if (nextX < 0)
								{
									moveCount++;
									continue;
								}
								for (int j{}; j < 4; j++)
								{
									if (m_board[nextY * BOARD_COL + nextX + j] != ShapeTile::BLANK)
									{
										isRotate = false;
										break;
									}
								}

								if (isRotate)
								{
									for (int j{}; j < 4; j++)
									{
										m_tilePos[j].first = nextX + j;
										m_tilePos[j].second = nextY;
									}

									m_isCWRotate = true;
									m_rotation -= PI / 2.0f;
									m_rotation = fmodf(m_rotation, 2.0f * PI);

									m_position.x -= TILE_SIZE / 2;
									m_position.x += moveCount * TILE_SIZE;
									m_position.y -= TILE_SIZE / 2;

									m_state++;
									m_state %= STATE_MAX_SIZE;
									break;
								}

								moveCount++;
							}
							else
							{
								nextX = m_tilePos[1].first - 3;
								for (int j{}; j < 4; j++)
								{
									if (m_board[nextY * BOARD_COL + nextX + j] != ShapeTile::BLANK)
									{
										isRotate = false;
										break;
									}
								}

								if (isRotate)
								{
									for (int j{}; j < 4; j++)
									{
										m_tilePos[j].first = nextX + j;
										m_tilePos[j].second = nextY;
									}

									m_isCWRotate = true;
									m_rotation -= PI / 2.0f;
									m_rotation = fmodf(m_rotation, 2.0f * PI);

									m_position.x -= TILE_SIZE / 2;
									m_position.x -= TILE_SIZE;
									m_position.y -= TILE_SIZE / 2;

									m_state++;
									m_state %= STATE_MAX_SIZE;
									break;
								}
							}

						}
					}
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
					if (!m_isCCWRotate)
					{
						int nextX{ m_tilePos[2].first };
						int nextY{};
						int moveCount{};

						for (int i{}; i < 4; i++)
						{
							nextY = m_tilePos[2].second + 2;

							if (nextY >= BOARD_ROW)
							{
								moveCount++;
								break;
							}

							bool isRotate{ true };
							if (i < 3)
							{
								for (int j{}; j < 4; j++)
								{
									if (m_board[(nextY-j) * BOARD_COL + nextX] != ShapeTile::BLANK)
									{
										isRotate = false;
										break;
									}
								}

								if (isRotate)
								{
									for (int j{ 3 }; j >= 0; j--)
									{
										m_tilePos[j].first = nextX - moveCount;
										m_tilePos[j].second = nextY--;
									}

									m_isCCWRotate = true;
									m_rotation -= PI / 2.0f;
									m_rotation = fmodf(m_rotation, 2.0f * PI);

									m_position.x += TILE_SIZE / 2;
									m_position.x -= moveCount * TILE_SIZE;
									m_position.y += TILE_SIZE / 2;

									m_state++;
									m_state %= STATE_MAX_SIZE;
									break;
								}

								moveCount++;
							}
							else
							{
								for (int j{}; j < 4; j++)
								{
									if (m_board[(nextY-j) * BOARD_COL + nextX] != ShapeTile::BLANK)
									{
										isRotate = false;
										break;
									}
								}

								if (isRotate)
								{
									for (int j{ 3 }; j >= 0; j--)
									{
										m_tilePos[j].first = nextX + 1;
										m_tilePos[j].second = nextY--;
									}

									m_isCCWRotate = true;
									m_rotation -= PI / 2.0f;
									m_rotation = fmodf(m_rotation, 2.0f * PI);

									m_position.x += TILE_SIZE / 2;
									m_position.x += TILE_SIZE;
									m_position.y += TILE_SIZE / 2;

									m_state++;
									m_state %= STATE_MAX_SIZE;
									break;
								}
							}

						}
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
						int nextX{ m_tilePos[2].first };
						int nextY{};
						int moveCount{};

						for (int i{}; i < 4; i++)
						{
							nextY = m_tilePos[2].second + 2;

							if (nextY >= BOARD_ROW)
							{
								moveCount++;
								break;
							}

							bool isRotate{ true };
							if (i < 3)
							{
								for (int j{}; j < 4; j++)
								{
									if (m_board[(nextY - j) * BOARD_COL + nextX] != ShapeTile::BLANK)
									{
										isRotate = false;
										break;
									}
								}

								if (isRotate)
								{
									for (int j{ 3 }; j >= 0; j--)
									{
										m_tilePos[j].first = nextX - moveCount;
										m_tilePos[j].second = nextY--;
									}

									m_isCWRotate = true;
									m_rotation -= PI / 2.0f;
									m_rotation = fmodf(m_rotation, 2.0f * PI);

									m_position.x += TILE_SIZE / 2;
									m_position.x -= moveCount * TILE_SIZE;
									m_position.y += TILE_SIZE / 2;

									m_state++;
									m_state %= STATE_MAX_SIZE;
									break;
								}

								moveCount++;
							}
							else
							{
								for (int j{}; j < 4; j++)
								{
									if (m_board[(nextY - j) * BOARD_COL + nextX] != ShapeTile::BLANK)
									{
										isRotate = false;
										break;
									}
								}

								if (isRotate)
								{
									for (int j{ 3 }; j >= 0; j--)
									{
										m_tilePos[j].first = nextX + 1;
										m_tilePos[j].second = nextY--;
									}

									m_isCWRotate = true;
									m_rotation -= PI / 2.0f;
									m_rotation = fmodf(m_rotation, 2.0f * PI);

									m_position.x += TILE_SIZE / 2;
									m_position.x += TILE_SIZE;
									m_position.y += TILE_SIZE / 2;

									m_state++;
									m_state %= STATE_MAX_SIZE;
									break;
								}
							}

						}
					}
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

void TileI::SpaceBar(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	m_isStuckBySpaceBar = true;

	switch (m_state)
	{
		case State::UP:
			{
				int nextX{ m_tilePos[3].first };
				int nextY{ m_tilePos[3].second };

				while (m_board[(nextY + 1) * BOARD_COL + nextX] == ShapeTile::BLANK)
				{
					m_position.y += TILE_SIZE;
					nextY++;
				}

				nextY -= 3;

				for (auto& e : m_tilePos)
				{
					e.second = nextY++;
				}

			}
			break;
		case State::RIGHT:
			{
				int nextX{ m_tilePos[0].first };
				int nextY{ m_tilePos[0].second};

				bool isCanMove{ true };

				while (isCanMove)
				{
					for (int i{}; i < 4; i++)
					{
						if (m_board[(nextY + 1) * BOARD_COL + nextX + i] != ShapeTile::BLANK)
						{
							isCanMove = false;
							break;
						}
					}

					if (!isCanMove) break;
					
					m_position.y += TILE_SIZE;
					nextY++;

				}

				for (auto& e : m_tilePos)
				{
					e.second = nextY;
				}
			}
			break;
		default:
			break;
	}

	
}

void DX::TileI::SetGhost(Actor* const& m_ghost, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	m_ghost->SetRotation(m_rotation);
	int moveCount{};

	switch (m_state)
	{
		case State::UP:
			{
				int nextX{ m_tilePos[3].first };
				int nextY{ m_tilePos[3].second };

				while (m_board[(nextY + moveCount +1) * BOARD_COL + nextX] == ShapeTile::BLANK)
				{
					++moveCount;
				}

				m_ghost->SetPosition(m_position.x, m_position.y + TILE_SIZE * moveCount);

			}
			break;
		case State::RIGHT:
			{
				int nextX{ m_tilePos[0].first };
				int nextY{ m_tilePos[0].second };

				bool isCanMove{ true };

				while (isCanMove)
				{
					for (int i{}; i < 4; i++)
					{
						if (m_board[(nextY + moveCount +1) * BOARD_COL + nextX + i] != ShapeTile::BLANK)
						{
							isCanMove = false;
							break;
						}
					}

					if (!isCanMove) break;

					++moveCount;

				}

				m_ghost->SetPosition(m_position.x, m_position.y + TILE_SIZE * moveCount);

			}
			break;
		default:
			break;
	}
}
