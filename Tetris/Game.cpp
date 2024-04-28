#include "pch.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;
using namespace GameConstants;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false) : 
	PIVOT_POS
	{ {
		{},
		{},
		{ 1, 2 },	// I_SHAPE
		{ 1, 1 },	// J_SHAPE
		{ 2, 1 },	// L_SHAPE
		{ 1, 1 },	// O_SHAPE
		{ 2, 1 },	// S_SHAPE
		{ 2, 1 },	// T_SHAPE
		{ 1, 1 }	// Z_SHAPE
	} },
	m_board(), m_tiles(), m_oneTiles(), m_ghostTiles(), m_nextTiles(), m_randomNums(), m_tile(nullptr), m_ghost(nullptr), m_background(nullptr),
	m_nextBoard(nullptr), m_scoreBoard(nullptr), m_title(nullptr),m_accumulatedTime(), m_keyPressedTime(), m_timeSpeed(1.0f), m_isPressedSpaceBar(false), 
	m_lineClearCount(), m_totalLine(), m_level(), m_nextTileIndex() , m_gameState()
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
	JsonManager::Instance().ReleaseAll();
	TextureManager::Instance().ReleaseAll();
}

void Game::Initialize(HWND window, int width, int height)
{
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window);

	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	TextureManager::Instance().Initialize(m_deviceResources.get());

	InitRandomSeed();

	StageInitialize();
}

void Game::OpenTitle()
{
	m_gameState = GameState::TITLE;
}

void Game::StageInitialize()
{
	m_background = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::BACKGROUND),L"Assets/Board.png");
	m_background->SetPivot(0.0f, 0.0f);
	m_background->SetPosition(0.0f, 0.0f);

	m_nextBoard = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::BACKGROUND), L"Assets/Next_Board.png");
	m_nextBoard->SetPivot(0.0f, 0.0f);
	m_nextBoard->SetPosition(m_background->GetPosition().x + 400.0f, m_background->GetPosition().y + 0.0f);

	m_scoreBoard = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::BACKGROUND), L"Assets/Score_Board.png");
	m_scoreBoard->SetPivot(0.0f, 0.0f);
	m_scoreBoard->SetPosition(m_background->GetPosition().x + 400.0f, m_background->GetPosition().y + 416.0f);

	m_title = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::BACKGROUND), L"Assets/Title.png");
	auto size = m_deviceResources->GetOutputSize();
	m_title->SetPosition(size.right / 2.0f, size.bottom / 2.0f - 100.0f);

	for (int i{}, j{ GameConstants::BOARD_COL * (GameConstants::BOARD_ROW - 1) }; i < GameConstants::BOARD_COL; i++, j++)
	{
		m_board[i] = m_board[j] = GameConstants::ShapeTile::WALL;
	}

	for (int i{}, j{ GameConstants::BOARD_COL - 1 }; j < GameConstants::BOARD_SIZE; i += GameConstants::BOARD_COL, j += GameConstants::BOARD_COL)
	{		
		m_board[i] = m_board[j] = GameConstants::ShapeTile::WALL;
	}

	std::array<std::wstring, static_cast<int>(ShapeTile::SIZE)> tiles{ L"",L"", L"Assets/I.png",  L"Assets/J.png" ,  L"Assets/L.png" ,
		L"Assets/O.png" ,  L"Assets/S.png" ,  L"Assets/T.png" ,  L"Assets/Z.png" };
	std::array<std::wstring, static_cast<int>(ShapeTile::SIZE)> oneTiles{ L"",L"", L"Assets/LightBlue.png",  L"Assets/Blue.png" ,  L"Assets/Orange.png" ,
		L"Assets/Yellow.png" ,  L"Assets/Green.png" ,  L"Assets/Purple.png" ,  L"Assets/Red.png" };
	std::array<std::wstring, static_cast<int>(ShapeTile::SIZE)> ghostTiles{ L"",L"", L"Assets/I_Ghost.png",  L"Assets/J_Ghost.png" ,  L"Assets/L_Ghost.png" ,
		L"Assets/O_Ghost.png" ,  L"Assets/S_Ghost.png" ,  L"Assets/T_Ghost.png" ,  L"Assets/Z_Ghost.png" };

	m_tiles[2] = ActorManager::Instance().Create<TileI>(static_cast<int>(GameConstants::Layer::TILE), tiles[2].c_str());
	m_tiles[3] = ActorManager::Instance().Create<TileJ>(static_cast<int>(GameConstants::Layer::TILE), tiles[3].c_str());
	m_tiles[4] = ActorManager::Instance().Create<TileL>(static_cast<int>(GameConstants::Layer::TILE), tiles[4].c_str());
	m_tiles[5] = ActorManager::Instance().Create<TileO>(static_cast<int>(GameConstants::Layer::TILE), tiles[5].c_str());
	m_tiles[6] = ActorManager::Instance().Create<TileS>(static_cast<int>(GameConstants::Layer::TILE), tiles[6].c_str());
	m_tiles[7] = ActorManager::Instance().Create<TileT>(static_cast<int>(GameConstants::Layer::TILE), tiles[7].c_str());
	m_tiles[8] = ActorManager::Instance().Create<TileZ>(static_cast<int>(GameConstants::Layer::TILE), tiles[8].c_str());

	for (int i{ static_cast<int>(ShapeTile::I_SHAPE) }; i < static_cast<int>(ShapeTile::SIZE); i++)
	{
		m_oneTiles[i] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::BOARD), oneTiles[i].c_str());
		m_ghostTiles[i] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::GHOST), ghostTiles[i].c_str());
		m_nextTiles[i] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::BACKGROUND), tiles[i].c_str());

		m_ghostTiles[i]->SetPivot(PIVOT_POS[i].first * TILE_SIZE, PIVOT_POS[i].second * TILE_SIZE);
		m_ghostTiles[i]->SetPosition(-100.0f, -100.0f);
	}

	m_nextTiles[2]->SetRotation(PI / 2.0f);

	for (int i{ static_cast<int>(ShapeTile::I_SHAPE) }; i < static_cast<int>(ShapeTile::SIZE); i++)
	{
		m_tiles[i]->SetPivot(PIVOT_POS[i].first * TILE_SIZE, PIVOT_POS[i].second * TILE_SIZE);
	}

	for (auto& num : m_randomNums)
	{
		num = GenerateRandomNumber(static_cast<int>(ShapeTile::I_SHAPE), static_cast<int>(ShapeTile::Z_SHAPE));
	}

	OpenTitle();
}

void Game::LineClearCheck()
{
	for (int i{ BOARD_ROW - 2 }; i > 0; i--)
	{
		bool isLineClear{ true };
		while (isLineClear)
		{
			for (int j{ 1 }; j < BOARD_COL - 1; j++)
			{
				if (m_board[i * BOARD_COL + j] == ShapeTile::BLANK)
				{
					isLineClear = false;
					break;
				}
			}

			if (isLineClear)
			{
				m_lineClearCount++;
				m_totalLine++;
				if (m_lineClearCount == 10)
				{
					m_lineClearCount = 0;
					m_level++;
					m_timeSpeed += 1.0f;
				}
				for (int j{ i }; j > 1; j--)
				{
					for (int k{ 1 }; k < BOARD_COL - 1; k++)
					{
						m_board[j * BOARD_COL + k] = m_board[(j - 1) * BOARD_COL + k];
					}
				}

				for (int j{ 1 }; j < BOARD_COL; j++)
				{
					m_board[BOARD_COL + j] = ShapeTile::BLANK;
				}
			}
		}
	}
}

void Game::InitRandomSeed()
{
	unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	m_randomGenerator.seed(seed);
}

void Game::SpawnTile()
{
	m_keyPressedTime = 0.0;

	m_tile = m_tiles[m_randomNums[m_nextTileIndex]];
	m_ghost = m_ghostTiles[m_randomNums[m_nextTileIndex]];
	m_tile->SetPosition(TILE_SIZE * START_X, TILE_SIZE * START_Y);
	m_tile->InitTile(static_cast<ShapeTile>(m_randomNums[m_nextTileIndex]));

	m_randomNums[m_nextTileIndex] = GenerateRandomNumber(static_cast<int>(ShapeTile::I_SHAPE), static_cast<int>(ShapeTile::Z_SHAPE));
	m_nextTileIndex = (m_nextTileIndex + 1) % m_randomNums.size();

	if (m_tile->IsDie(m_board))
	{
		PlayStage();
	}
}

void Game::DrawStage()
{
	m_background->Draw(m_spriteBatch.get());
	m_nextBoard->Draw(m_spriteBatch.get());
	m_scoreBoard->Draw(m_spriteBatch.get());
	m_tile->Draw(m_spriteBatch.get());

	for (int i{}; i<BOARD_SIZE; i++)
	{
		if (m_board[i] != ShapeTile::BLANK && m_board[i] != ShapeTile::WALL)
		{
			m_oneTiles[static_cast<int>(m_board[i])]->SetPosition((i % BOARD_COL) * TILE_SIZE + TILE_SIZE / 2, (i / BOARD_COL) * TILE_SIZE + TILE_SIZE / 2);
			m_oneTiles[static_cast<int>(m_board[i])]->Draw(m_spriteBatch.get());
		}
	}

	m_ghost->Draw(m_spriteBatch.get());

	float nextX{528.0f}, nextY{120.0f};
	for (int i{}; i < 3; i++)
	{
		int num = (m_nextTileIndex + i) % m_randomNums.size();
		m_nextTiles[m_randomNums[num]]->SetPosition(nextX, nextY);
		m_nextTiles[m_randomNums[num]]->Draw(m_spriteBatch.get());
		nextY += 96.0f;
	}

	std::wstring levelNum{ std::to_wstring(m_level) };
	std::wstring linesNum{ std::to_wstring(m_totalLine) };

	DrawFont(L"NEXT", m_font->MeasureString(L"NEXT") / 2.f, { 528.0f,32.0f });
	DrawFont(L"LEVEL", m_font->MeasureString(L"LEVEL") / 2.f, { 528.0f,450.0f });
	DrawFont(levelNum, m_font->MeasureString(levelNum.c_str()) / 2.f, { 528.0f,510.0f });
	DrawFont(L"LINES", m_font->MeasureString(L"LINES") / 2.f, { 528.0f,580.0f });
	DrawFont(linesNum, m_font->MeasureString(linesNum.c_str()) / 2.f, { 528.0f,645.0f });
}

void Game::DrawTitle()
{
	m_title->Draw(m_spriteBatch.get());

	DrawFont(L"Press spacebar to start!", m_font->MeasureString(L"Press spacebar to start!") / 2.f, { m_title->GetPosition().x, 500.0f });
}

void Game::DrawFont(const std::wstring text, Vector2 origin, XMFLOAT2 fontPos)
{
	m_font->DrawString(m_spriteBatch.get(), text.c_str(), fontPos + Vector2(1.f, 1.f), Colors::Black, 0.f, origin);
	m_font->DrawString(m_spriteBatch.get(), text.c_str(), fontPos + Vector2(-1.f, 1.f), Colors::Black, 0.f, origin);
	m_font->DrawString(m_spriteBatch.get(), text.c_str(), fontPos + Vector2(-1.f, -1.f), Colors::Black, 0.f, origin);
	m_font->DrawString(m_spriteBatch.get(), text.c_str(), fontPos + Vector2(1.f, -1.f), Colors::Black, 0.f, origin);

	m_font->DrawString(m_spriteBatch.get(), text.c_str(), fontPos, Colors::White, 0.f, origin);
}

void Game::PlayStage()
{
	m_gameState = GameState::STAGE;

	m_lineClearCount = 0;

	for (int i{ 1 }; i < BOARD_ROW - 1; i++)
	{
		for (int j{ 1 }; j < BOARD_COL - 1; j++)
		{
			m_board[i * BOARD_COL + j] = ShapeTile::BLANK;
		}
	}

	SpawnTile();
}

int Game::GenerateRandomNumber(int min, int max)
{
	std::uniform_int_distribution<> dist(min, max);
	return dist(m_randomGenerator);
}

#pragma region Frame Update

void Game::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		});

	Render();
}

void Game::Update(DX::StepTimer const& timer)
{
	auto kb = m_keyboard->GetState();

	if (kb.Escape)
	{
		ExitGame();
	}

	switch (m_gameState)
	{
		case GameConstants::GameState::TITLE:
			if (kb.Space)
			{
				PlayStage();
				Sleep(500);
				m_isPressedSpaceBar = true;
			}
			break;
		case GameConstants::GameState::STAGE:
			if (m_tile->IsStuck())
			{
				m_tile->StackBoard(m_board);
				m_ghost->SetPosition(-100.0f, -100.0f);
				LineClearCheck();

				SpawnTile();

				return;
			}

			if (kb.Space)
			{
				if (!m_isPressedSpaceBar)
				{
					m_tile->SpaceBar(m_board);
					m_isPressedSpaceBar = true;
				}
			}
			else
			{
				m_isPressedSpaceBar = false;
			}

			m_tile->Move(timer.GetElapsedSeconds() * m_timeSpeed, kb, m_accumulatedTime, m_keyPressedTime, m_board);
			m_tile->Rotate(kb, m_board);

			m_tile->SetGhost(m_ghost, m_board);
			break;
		case GameConstants::GameState::PAUSE:
			break;
		case GameConstants::GameState::SIZE:
			break;
		default:
			break;
	}

	ActorManager::Instance().Update(timer.GetElapsedSeconds());

}
#pragma endregion

#pragma region Frame Render

void Game::Render()
{
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	switch (m_gameState)
	{
		case GameConstants::GameState::TITLE:
			DrawTitle();
			break;
		case GameConstants::GameState::STAGE:
			DrawStage();
			break;
		case GameConstants::GameState::PAUSE:
			break;
		case GameConstants::GameState::SIZE:
			break;
		default:
			break;
	}
	
	m_spriteBatch->End();

	m_deviceResources->PIXEndEvent();

	m_deviceResources->Present();
}

void Game::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	auto context = m_deviceResources->GetD3DeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::DarkSlateBlue);
	context->ClearDepthStencilView(
		depthStencil,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}

#pragma endregion

#pragma region Message Handlers

void Game::OnActivated()
{

}

void Game::OnDeactivated()
{

}

void Game::OnSuspending()
{

}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();
}

void Game::OnWindowMoved()
{
	auto r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height)) { return; }

	CreateWindowSizeDependentResources();
}

void Game::GetDefaultSize(int& width, int& height) const noexcept
{
	width = 1280;
	height = 720;
}
#pragma endregion

#pragma region Direct3D Resources

void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DeviceContext();

	m_commonStates = std::make_unique<CommonStates>(device);
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	m_font = std::make_unique<SpriteFont>(device, L"Assets/tetris.spritefont");
}

void Game::CreateWindowSizeDependentResources()
{

}

void Game::OnDeviceLost()
{

	m_spriteBatch.reset();
	m_commonStates.reset();
	m_font.reset();

	TextureManager::Instance().OnDeviceLost();
	ActorManager::Instance().OnDeviceLost();
}

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	TextureManager::Instance().OnDeviceRestored();
	ActorManager::Instance().OnDeviceRestored();
}

#pragma endregion