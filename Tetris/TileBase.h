#pragma once
#include "Actor.h"
namespace DX
{
	class TileBase : public Actor
	{
	public:
		TileBase();
		virtual ~TileBase();

		virtual void Move(DX::StepTimer const& timer, DirectX::Keyboard::State const& kb, double& accumulatedTime, double& keyPressedTime);
		virtual void Rotate(DirectX::Keyboard::State const& kb);
		virtual bool IsStuck(DX::StepTimer const& timer);
		virtual void SpaceBar();

		bool IsStuckBySpaceBar() { return m_isStuckBySpaceBar; };
		void SetStuckBySpaceBar(bool isStuck);

	protected:
		double m_stuckTime{ 0.0 };
		bool m_isStuckBySpaceBar{};
		bool m_isCCWRotate{};
		bool m_isCWRotate{};
	};

}

