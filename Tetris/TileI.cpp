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
	if (m_isStuckBySpaceBar)
	{
		m_stuckTime += timer.GetElapsedSeconds();

		if (m_stuckTime >= TILE_DOWN_TIME/2)
		{
			m_isStuck = true;
		}
		return;
	}

	accumulatedTime += timer.GetElapsedSeconds();

	switch (m_state)
	{
		case State::UP:
			{
				if (accumulatedTime >= TILE_DOWN_TIME)
				{
					accumulatedTime = 0.0;

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
					else
					{
						m_isStuck = true;
					}
				}

				if (kb.D)
				{
					if (!keyPressedTime)
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
					else
					{
						if (keyPressedTime >= START_TIME_MOVE_TILE)
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
							keyPressedTime = KEY_PRESSED_INTERVAL_TIME;
						}
					}

					keyPressedTime += timer.GetElapsedSeconds();
				}
				else if (kb.A)
				{
					if (!keyPressedTime)
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
					else
					{
						if (keyPressedTime >= START_TIME_MOVE_TILE)
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
							
							keyPressedTime = KEY_PRESSED_INTERVAL_TIME;
						}
					}

					keyPressedTime += timer.GetElapsedSeconds();

				}
				else if (kb.S)
				{
					if (!keyPressedTime)
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
					else
					{
						if (keyPressedTime >= START_TIME_MOVE_TILE)
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
			break;
		case State::RIGHT:
			{
				if (accumulatedTime >= TILE_DOWN_TIME)
				{
					accumulatedTime = 0.0;

					int nextX{ m_tilePos[0].first };
					int nextY{ m_tilePos[0].second + 1 };
					bool isCanMove{ true };

					for (int i{}; i<4; i++)
					{
						if (m_board[nextY * BOARD_COL + nextX + i] != ShapeTile::BLANK)
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
					}
					else
					{
						m_isStuck = true;
					}
				}

				if (kb.D)
				{
					if (!keyPressedTime)
					{	
						int nextX{ m_tilePos[3].first + 1 };
						int nextY{ m_tilePos[3].second };

						if (m_board[nextY * BOARD_COL + nextX] == ShapeTile::BLANK)
						{
							m_position.x += TILE_SIZE;

							for (auto& e : m_tilePos)
							{
								e.first++;
							}
						}
					}
					else
					{
						if (keyPressedTime >= START_TIME_MOVE_TILE)
						{
							int nextX{ m_tilePos[3].first + 1 };
							int nextY{ m_tilePos[3].second };

							if (m_board[nextY * BOARD_COL + nextX] == ShapeTile::BLANK)
							{
								m_position.x += TILE_SIZE;

								for (auto& e : m_tilePos)
								{
									e.first++;
								}
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
						int nextX{ m_tilePos[0].first - 1 };
						int nextY{ m_tilePos[0].second };

						if (m_board[nextY * BOARD_COL + nextX] == ShapeTile::BLANK)
						{
							m_position.x -= TILE_SIZE;

							for (auto& e : m_tilePos)
							{
								e.first--;
							}
						}
					}
					else
					{
						if (keyPressedTime >= START_TIME_MOVE_TILE)
						{
							int nextX{ m_tilePos[0].first - 1 };
							int nextY{ m_tilePos[0].second };

							if (m_board[nextY * BOARD_COL + nextX] == ShapeTile::BLANK)
							{
								m_position.x -= TILE_SIZE;

								for (auto& e : m_tilePos)
								{
									e.first--;
								}
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
						int nextX{ m_tilePos[0].first };
						int nextY{ m_tilePos[0].second + 1 };

						bool isCanMove{ true };

						for (int i{}; i < 4; i++)
						{
							if (m_board[nextY * BOARD_COL + nextX + i] != ShapeTile::BLANK)
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
						}
					}
					else
					{
						if (keyPressedTime >= START_TIME_MOVE_TILE)
						{
							int nextX{ m_tilePos[0].first };
							int nextY{ m_tilePos[0].second + 1 };

							bool isCanMove{ true };

							for (int i{}; i < 4; i++)
							{
								if (m_board[nextY * BOARD_COL + nextX + i] != ShapeTile::BLANK)
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
			break;
		default:
			break;
	}

}

void TileI::Rotate(DirectX::Keyboard::State const& kb, std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
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

void DX::TileI::InitTile()
{
	int tempPlus{};
	for (auto& e : m_tilePos)
	{
		e.first = START_X;
		e.second = START_Y + tempPlus++;
	}

	m_state = UP;
	m_isStuck = false;
	m_rotation = 0.f;
	m_stuckTime = 0.0;
	m_shape = ShapeTile::I_SHAPE;
}

void TileI::SpaceBar(std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board)
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
