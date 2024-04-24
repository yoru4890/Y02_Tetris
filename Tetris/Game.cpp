#include "pch.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;
using namespace GameConstants;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false) : 
	m_board(), m_tiles(), m_oneTiles(), m_ghostTiles(), m_tile(nullptr), m_ghost(nullptr), m_background(nullptr),
	m_accumulatedTime(), m_keyPressedTime(), m_isPressedSpaceBar(false)
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

void Game::StageInitialize()
{
	m_background = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::BACKGROUND),
		L"Assets/Board.png"
	);
	m_background->SetPivot(0.0f, 0.0f);
	m_background->SetPosition(0.0f, 0.0f);

	for (int i{}, j{ GameConstants::BOARD_COL * (GameConstants::BOARD_ROW - 1) }; i < GameConstants::BOARD_COL; i++, j++)
	{
		m_board[i] = m_board[j] = GameConstants::ShapeTile::WALL;
	}

	for (int i{}, j{ GameConstants::BOARD_COL - 1 }; j < GameConstants::BOARD_SIZE; i += GameConstants::BOARD_COL, j += GameConstants::BOARD_COL)
	{		
		m_board[i] = m_board[j] = GameConstants::ShapeTile::WALL;
	}

	std::array<std::wstring, static_cast<int>(ShapeTile::UNKNOWN)> tiles{ L"",L"", L"Assets/I.png",  L"Assets/J.png" ,  L"Assets/L.png" ,
		L"Assets/O.png" ,  L"Assets/S.png" ,  L"Assets/T.png" ,  L"Assets/Z.png" };
	std::array<std::wstring, static_cast<int>(ShapeTile::UNKNOWN)> oneTiles{ L"",L"", L"Assets/LightBlue.png",  L"Assets/Blue.png" ,  L"Assets/Orange.png" ,
		L"Assets/Yellow.png" ,  L"Assets/Green.png" ,  L"Assets/Purple.png" ,  L"Assets/Red.png" };
	std::array<std::wstring, static_cast<int>(ShapeTile::UNKNOWN)> ghostTiles{ L"",L"", L"Assets/I_Ghost.png",  L"Assets/J_Ghost.png" ,  L"Assets/L_Ghost.png" ,
		L"Assets/O_Ghost.png" ,  L"Assets/S_Ghost.png" ,  L"Assets/T_Ghost.png" ,  L"Assets/Z_Ghost.png" };

	m_tiles[2] = ActorManager::Instance().Create<TileI>(static_cast<int>(GameConstants::Layer::TILE), tiles[2].c_str());
	/*m_tiles[3] = ActorManager::Instance().Create<TileJ>(static_cast<int>(GameConstants::Layer::TILE), tiles[3].c_str());
	m_tiles[4] = ActorManager::Instance().Create<TileL>(static_cast<int>(GameConstants::Layer::TILE), tiles[4].c_str());
	m_tiles[5] = ActorManager::Instance().Create<TileO>(static_cast<int>(GameConstants::Layer::TILE), tiles[5].c_str());
	m_tiles[6] = ActorManager::Instance().Create<TileS>(static_cast<int>(GameConstants::Layer::TILE), tiles[6].c_str());
	m_tiles[7] = ActorManager::Instance().Create<TileT>(static_cast<int>(GameConstants::Layer::TILE), tiles[7].c_str());
	m_tiles[8] = ActorManager::Instance().Create<TileZ>(static_cast<int>(GameConstants::Layer::TILE), tiles[8].c_str());*/

	for (int i{ 2 }; i < static_cast<int>(ShapeTile::UNKNOWN); i++)
	{
		m_oneTiles[i] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::BOARD), oneTiles[i].c_str());
		m_ghostTiles[i] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::GHOST), ghostTiles[i].c_str());
	}


	SpawnTile();

}

void Game::LineClearCheck()
{
}

int Game::GenerateRandomNumber(int min, int max)
{
	std::uniform_int_distribution<> dist(min, max);
	return dist(m_randomGenerator);
}

void Game::InitRandomSeed()
{
	unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
	m_randomGenerator.seed(seed);
}

void Game::SpawnTile()
{
	m_keyPressedTime = 0.0;

	int randomNum{ GenerateRandomNumber(2,8) };

	m_tile = m_tiles[2];
	// m_ghost = m_ghostTiles[2];
	// TODO 고스트 위치조정
	m_tile->SetPosition(m_tile->GetSize().x / 2 + TILE_SIZE * 5, m_tile->GetSize().y / 2 + TILE_SIZE);
	m_tile->SetStuckBySpaceBar(false);
	m_tile->InitTile();
}

void Game::DrawBoard()
{

	m_background->Draw(m_spriteBatch.get());
	m_tile->Draw(m_spriteBatch.get());

	for (int i{}; i<BOARD_SIZE; i++)
	{
		if (m_board[i] != ShapeTile::BLANK && m_board[i] != ShapeTile::WALL)
		{
			m_oneTiles[static_cast<int>(m_board[i])]->SetPosition((i % BOARD_COL) * TILE_SIZE + TILE_SIZE / 2, (i / BOARD_COL) * TILE_SIZE + TILE_SIZE / 2);
			m_oneTiles[static_cast<int>(m_board[i])]->Draw(m_spriteBatch.get());
		}
	}

	//ActorManager().Instance().Draw(m_spriteBatch.get());
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
	if (m_tile->IsStuck())
	{
		m_tile->StackBoard(m_board);

		SpawnTile();
		


		// LineClear 시키고 m_board값 변화 (함수필요)
		return;
	}
	
	auto kb = m_keyboard->GetState();
	
	if (kb.Escape)
	{
		ExitGame();
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

	m_tile->Move(timer, kb, m_accumulatedTime, m_keyPressedTime, m_board);
	m_tile->Rotate(kb, m_board);
	
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

	DrawBoard();
	
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

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
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

}

void Game::CreateWindowSizeDependentResources()
{

}

void Game::OnDeviceLost()
{

	m_spriteBatch.reset();
	m_commonStates.reset();

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