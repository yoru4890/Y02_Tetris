#pragma once

class Game : public DX::IDeviceNotify
{
public:
	Game() noexcept(false);
	virtual ~Game();

	Game(Game&&) = default;
	Game& operator= (Game&&) = default;

	Game(Game const&) = delete;
	Game& operator= (Game const&) = delete;

	virtual void Initialize(HWND window, int width, int height);

	virtual void Tick();

	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnSuspending();
	virtual void OnResuming();
	virtual void OnWindowMoved();
	virtual void OnWindowSizeChanged(int width, int height);
	virtual void GetDefaultSize(int& width, int& height) const noexcept;

	void StageInitialize();
	void LineClearCheck(); // TODO
	int GenerateRandomNumber(int min, int max);
	void InitRandomSeed();
	void SpawnTile();
	void DrawBoard();

protected:
	virtual void Update(DX::StepTimer const& timer);
	virtual void Render();
	virtual void Clear();
	virtual void CreateDeviceDependentResources();
	virtual void CreateWindowSizeDependentResources();

private:
	std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>	m_board;
	std::array<DX::TileBase*, static_cast<int>(GameConstants::ShapeTile::UNKNOWN)> m_tiles;
	std::array<DX::Actor*, static_cast<int>(GameConstants::ShapeTile::UNKNOWN)> m_oneTiles;
	std::array<DX::Actor*, static_cast<int>(GameConstants::ShapeTile::UNKNOWN)> m_ghostTiles;
	DX::TileBase* m_tile;
	DX::Actor* m_ghost;
	DX::Actor* m_background;
	std::mt19937								m_randomGenerator;
	double										m_accumulatedTime;
	double										m_keyPressedTime;
	bool m_isPressedSpaceBar;
	

protected:
	int m_currentFrame{};
	double m_timeToNextFrame{ 0.1f };

	std::unique_ptr<DX::DeviceResources>	m_deviceResources;
	DX::StepTimer							m_timer;
	std::unique_ptr<DirectX::Keyboard>		m_keyboard;
	std::unique_ptr<DirectX::Mouse>			m_mouse;

	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates>	m_commonStates;
};

