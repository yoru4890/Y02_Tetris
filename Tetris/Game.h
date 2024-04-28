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

	void OpenTitle();
	void StageInitialize();
	void LineClearCheck();
	void InitRandomSeed();
	void SpawnTile();
	void DrawStage();
	void DrawTitle();
	void DrawFont(const std::wstring text, DirectX::SimpleMath::Vector2 origin, DirectX::XMFLOAT2 fontPos);
	void PlayStage();
	int GenerateRandomNumber(int min, int max);

protected:
	virtual void Update(DX::StepTimer const& timer);
	virtual void Render();
	virtual void Clear();
	virtual void CreateDeviceDependentResources();
	virtual void CreateWindowSizeDependentResources();

private:
	std::array<std::pair<const int, const int>, 9> PIVOT_POS;
	std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>	m_board;
	std::array<DX::TileBase*, static_cast<int>(GameConstants::ShapeTile::SIZE)> m_tiles;
	std::array<DX::Actor*, static_cast<int>(GameConstants::ShapeTile::SIZE)> m_oneTiles;
	std::array<DX::Actor*, static_cast<int>(GameConstants::ShapeTile::SIZE)> m_ghostTiles;
	std::array<DX::Actor*, static_cast<int>(GameConstants::ShapeTile::SIZE)> m_nextTiles;
	std::array<int, 3> m_randomNums;
	DX::TileBase* m_tile;
	DX::Actor* m_ghost;
	DX::Actor* m_background;
	DX::Actor* m_nextBoard;
	DX::Actor* m_scoreBoard;
	DX::Actor* m_title;
	std::mt19937								m_randomGenerator;
	double										m_accumulatedTime;
	double										m_keyPressedTime;
	double										m_timeSpeed;
	bool										m_isPressedSpaceBar;
	int											m_lineClearCount;
	int											m_totalLine;
	int											m_level;
	int											m_nextTileIndex;
	GameConstants::GameState					m_gameState;
	
protected:
	int m_currentFrame{};
	double m_timeToNextFrame{ 0.1f };

	std::unique_ptr<DX::DeviceResources>	m_deviceResources;
	DX::StepTimer							m_timer;
	std::unique_ptr<DirectX::Keyboard>		m_keyboard;
	std::unique_ptr<DirectX::Mouse>			m_mouse;

	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates>	m_commonStates;
	std::unique_ptr<DirectX::SpriteFont>	m_font;
};

