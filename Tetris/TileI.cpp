#include "pch.h"
#include "TileI.h"

using namespace DX;

TileI::TileI() : m_state()
{
}

TileI::~TileI()
{
}

bool TileI::Move(DX::StepTimer const& timer, DirectX::Keyboard::State const& kb, double& accumulatedTime, double& keyPressedTime)
{
	accumulatedTime += timer.GetElapsedSeconds();

	if (accumulatedTime >= 1.0)
	{
		accumulatedTime = 0.0;

		if (this->GetPosition().y < 544.0f)
		{
			this->SetPosition(this->GetPosition().x, this->GetPosition().y + 32.0f);
		}
		else
		{
			return false;
		}
	}


	if (kb.D)
	{
		if (!keyPressedTime)
		{
			if (this->GetPosition().x < 320.0f)
			{
				this->SetPosition(this->GetPosition().x + 32.0f, this->GetPosition().y);
			}

			keyPressedTime += timer.GetElapsedSeconds();
		}
		else
		{
			keyPressedTime += timer.GetElapsedSeconds();

			if (keyPressedTime >= 0.7)
			{
				if (this->GetPosition().x < 320.0f)
				{
					this->SetPosition(this->GetPosition().x + 32.0f, this->GetPosition().y);
				}

				keyPressedTime = 0.6;
			}
		}
	}
	else if (kb.A)
	{
		if (!keyPressedTime)
		{
			if (this->GetPosition().x > 32.0f)
			{
				this->SetPosition(this->GetPosition().x - 32.0f, this->GetPosition().y);
			}

			keyPressedTime += timer.GetElapsedSeconds();
		}
		else
		{
			keyPressedTime += timer.GetElapsedSeconds();

			if (keyPressedTime >= 0.7)
			{
				if (this->GetPosition().x > 32.0f)
				{
					this->SetPosition(this->GetPosition().x - 32.0f, this->GetPosition().y);
				}

				keyPressedTime = 0.6;
			}
		}
	}
	else
	{
		keyPressedTime = 0.0;
	}

	return true;
}

void TileI::Rotate()
{
}

bool TileI::IsStuck()
{
	return false;
}

void TileI::SpaceBar()
{
}
