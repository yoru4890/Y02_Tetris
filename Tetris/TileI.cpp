#include "pch.h"
#include "TileI.h"

using namespace DX;
using namespace GameConstants;

TileI::TileI() : m_state()
{
}

TileI::~TileI()
{
}

void TileI::Move(
	DX::StepTimer const& timer, 
	DirectX::Keyboard::State const& kb, 
	double& accumulatedTime, 
	double& keyPressedTime,
	std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board
	)
{
	if (m_isStuckBySpaceBar) return;

	accumulatedTime += timer.GetElapsedSeconds();

	if (accumulatedTime >= TILE_DOWN_TIME)
	{
		accumulatedTime = 0.0;

		switch (m_state)
		{
			case UP:
				{
					int nextX{ m_tilePos[3].first };
					int nextY{ m_tilePos[3].second + 1 };
					if (m_board[nextY * BOARD_COL + nextX] == ShapeTile::BLANK)
					{
						m_position.y += TILE_SIZE;
						for (auto& e : m_tilePos)
						{
							e.second += 1;
						}
					}
				}
				break;
			case RIGHT:
				break;
			default:
				break;
		}
	}


	if (kb.D)
	{
		if (!keyPressedTime)
		{
			switch (m_state)
			{
				case UP:
					{
						bool isCanMove{ true };
						for (const auto& e : m_tilePos)
						{
							int nextX{ e.first + 1 };
							int nextY{ e.second };

							if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
							{
								isCanMove = false;
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
					}
					break;
				case RIGHT:
					break;
				default:
					break;
			}

		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				switch (m_state)
				{
					case UP:
						{
							bool isCanMove{ true };
							for (const auto& e : m_tilePos)
							{
								int nextX{ e.first + 1 };
								int nextY{ e.second };

								if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
								{
									isCanMove = false;
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
						}
						break;
					case RIGHT:
						break;
					default:
						break;
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
			switch (m_state)
			{
				case UP:
					{
						bool isCanMove{ true };
						for (const auto& e : m_tilePos)
						{
							int nextX{ e.first - 1 };
							int nextY{ e.second };

							if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
							{
								isCanMove = false;
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
					}
					break;
				case RIGHT:
					break;
				default:
					break;
			}		
		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				switch (m_state)
				{
					case UP:
						{
							bool isCanMove{ true };
							for (const auto& e : m_tilePos)
							{
								int nextX{ e.first - 1 };
								int nextY{ e.second };

								if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
								{
									isCanMove = false;
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
						}
						break;
					case RIGHT:
						break;
					default:
						break;
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
			switch (m_state)
			{
				case UP:
					{
						int nextX{ m_tilePos[3].first };
						int nextY{ m_tilePos[3].second + 1 };
						if (m_board[nextY * BOARD_COL + nextX] == ShapeTile::BLANK)
						{
							m_position.y += TILE_SIZE;
							for (auto& e : m_tilePos)
							{
								e.second += 1;
							}
						}
					}
					break;
				case RIGHT:
					break;
				default:
					break;
			}
		}
		else
		{
			if (keyPressedTime >= START_TIME_MOVE_TILE)
			{
				switch (m_state)
				{
					case UP:
						{
							int nextX{ m_tilePos[3].first };
							int nextY{ m_tilePos[3].second + 1 };
							if (m_board[nextY * BOARD_COL + nextX] == ShapeTile::BLANK)
							{
								m_position.y += TILE_SIZE;
								for (auto& e : m_tilePos)
								{
									e.second += 1;
								}
							}
						}
						break;
					case RIGHT:
						break;
					default:
						break;
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

void TileI::Rotate(DirectX::Keyboard::State const& kb, std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	if (kb.Q)
	{
		if (!m_isCCWRotate)
		{
			switch (m_state)
			{
				case UP:
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
					break;
				case RIGHT:
					break;
				default:
					break;
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
			switch (m_state)
			{
				case UP:
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
					break;
				case RIGHT:
					break;
				default:
					break;
			}
		}
	}
	else
	{
		m_isCWRotate = false;
	}
}

bool TileI::IsStuck(DX::StepTimer const& timer, std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	switch (m_state)
	{
		case UP:
			{
				int nextX{ m_tilePos[3].first };
				int nextY{ m_tilePos[3].second + 1 };
				if (m_board[nextY * BOARD_COL + nextX] != ShapeTile::BLANK)
				{
					m_stuckTime += timer.GetElapsedSeconds();
				}
			}
			break;
		case RIGHT:
			break;
		default:
			break;
	}

	if (m_stuckTime >= CAN_MOVE_TIME)
	{
		m_stuckTime = 0.0;
		return true;
	}

	return false;
}

void DX::TileI::InitTile()
{
	int tempPlus{};
	for (auto& e : m_tilePos)
	{
		e.first = START_X;
		e.second = START_Y + tempPlus++;
	}
}

void TileI::SpaceBar(std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
{
	m_isStuckBySpaceBar = true;

	switch (m_state)
	{
		case UP:
			{
				int nextX{ m_tilePos[3].first };
				int nextY{ m_tilePos[3].second};

				while (m_board[(nextY+1) * BOARD_COL + nextX] == ShapeTile::BLANK)
				{
					m_position.y += TILE_SIZE;
					nextY++;
				}

				m_tilePos[3].second = nextY;

			}
			break;
		case RIGHT:
			break;
		default:
			break;
	}

	
}
