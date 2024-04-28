#pragma once
#include "Actor.h"

namespace DX
{
	class TileBase : public Actor
	{
	public:
		TileBase();
		virtual ~TileBase();

		virtual void InitTile(GameConstants::ShapeTile shapeTile);
		virtual void Rotate(DirectX::Keyboard::State const& kb, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) {}
		virtual void CCWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int xDir = 0, int yDir = 0) {}
		virtual void CWRotate(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board, int xDir = 0, int yDir = 0) {}

		void Move(
			const double& elapsedSeconds,
			DirectX::Keyboard::State const& kb,
			double& accumulatedTime,
			double& keyPressedTime,
			const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board
		);
		bool MoveDown(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board);
		void MoveRight(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board);
		void MoveLeft(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board);
		void SpaceBar(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board);
		void SetGhost(Actor* const& m_ghost, const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board);

		void StackBoard(std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board);

		bool IsStuck() { return m_isStuck; }
		bool IsStuckBySpaceBar() { return m_isStuckBySpaceBar; };
		bool IsDie(const std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board);

	protected:

		enum State
		{
			UP,
			RIGHT,
			DOWN,
			LEFT,

			SIZE
		};

		const float ROTATION_OFFSET{ 1.5f };
		std::array<std::pair<const float, const float>, 36> START_POS;

		std::pair<float, float> basePos;
		bool m_isStuckBySpaceBar{};
		bool m_isCCWRotate{};
		bool m_isCWRotate{};
		bool m_isStuck{};
		double m_stuckTime{ 0.0 };
		int m_state{};

		std::array<std::pair<float, float>, 4> m_tilePos;
		std::array<std::pair<float, float>, 4> m_nextTilePos;
		GameConstants::ShapeTile m_shape{};
		
	};

}

