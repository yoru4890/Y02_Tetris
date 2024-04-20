#include "pch.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false) : m_board(), m_tile(nullptr), m_accumulatedTime(), m_keyPressedTime(), m_tiles(), m_background()
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
	m_background = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::Background),
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

	m_tiles[2] = ActorManager::Instance().Create<TileI>(static_cast<int>(GameConstants::Layer::Tile), L"Assets/I.png");
	//m_tiles[3] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::Tile), L"Assets/J.png");
	//m_tiles[4] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::Tile), L"Assets/L.png");
	//m_tiles[5] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::Tile), L"Assets/O.png");
	//m_tiles[6] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::Tile), L"Assets/S.png");
	//m_tiles[7] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::Tile), L"Assets/T.png");
	//m_tiles[8] = ActorManager::Instance().Create<Actor>(static_cast<int>(GameConstants::Layer::Tile), L"Assets/Z.png");

	int randomNum{ GenerateRandomNumber(2,8) };

	m_tile = m_tiles[2];
	m_tile->SetPivot(0.0f, 0.0f);
	m_tile->SetPosition(160.0f, 32.0f);
	// TODO
	// 이미지파일들 어떻게 저장시키고 다룰지 고민

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
	m_tile->SetPivot(0.0f, 0.0f);
	m_tile->SetPosition(160.0f, 32.0f);
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

	if (!(m_tile->Move(timer, kb, m_accumulatedTime, m_keyPressedTime)))
	{
		SpawnTile();
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

	
	m_background->Draw(m_spriteBatch.get());
	m_tile->Draw(m_spriteBatch.get());
	//ActorManager().Instance().Draw(m_spriteBatch.get());
	
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