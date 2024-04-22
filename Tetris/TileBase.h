#pragma once
#include "Actor.h"

namespace DX
{
	class TileBase : public Actor
	{
	public:
		TileBase();
		virtual ~TileBase();

		virtual void Move(
			DX::StepTimer const& timer,
			DirectX::Keyboard::State const& kb,
			double& accumulatedTime,
			double& keyPressedTime,
			std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board
		) {}
		virtual void Rotate(DirectX::Keyboard::State const& kb, std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) {}
		virtual bool IsStuck(DX::StepTimer const& timer, std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board);
		virtual void InitTile();
		virtual void SpaceBar(std::array<GameConstants::ShapeTile, GameConstants::BOARD_SIZE>& m_board) {}

		bool IsStuckBySpaceBar() { return m_isStuckBySpaceBar; };
		void SetStuckBySpaceBar(bool isStuck);

	protected:
		double m_stuckTime{ 0.0 };
		bool m_isStuckBySpaceBar{};
		bool m_isCCWRotate{};
		bool m_isCWRotate{};
		std::array<std::pair<int, int>, 4> m_tilePos;
	};

}

