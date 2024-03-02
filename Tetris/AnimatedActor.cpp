#include "pch.h"

using namespace DirectX;
using namespace DX;

AnimatedActor::AnimatedActor() :
	Actor(),
	m_animations{ nullptr },
	m_animation{ nullptr },
	m_paused{ false },
	m_isLoop{ false },
	m_totalElapsed{ 0.0f },
	m_timePerFrame{ 0.0f },
	m_frameIndex{ 0 },
	m_frameCount{ 0 }
{
}

AnimatedActor::~AnimatedActor()
{
}

void AnimatedActor::LoadAnimation(const LPCWSTR jsonFile)
{
	m_animations = JsonManager::Instance().CreateDocument(jsonFile);
}

void AnimatedActor::Update(double dt)
{
	if (m_paused)
	{
		return;
	}

	m_totalElapsed += static_cast<float>(dt);

	int prevFrame = m_frameIndex;

	if (m_totalElapsed > m_timePerFrame)
	{
		if (m_isLoop)
		{
			m_frameIndex = (m_frameIndex + 1) % m_frameCount;
		}
		else
		{
			m_frameIndex = std::min(m_frameIndex + 1, m_frameCount - 1);
		}

		if (prevFrame != m_frameIndex)
		{
			m_totalElapsed -= m_timePerFrame;
			SetAnimationIndex(m_frameIndex);
		}
	}
}

void AnimatedActor::SetAnimation(const LPCWSTR aniName)
{
	if (m_animations != nullptr)
	{
		const auto& anims = (*m_animations)[L"animations"].GetArray();
		for (auto& elem : anims)
		{
			if (std::wcscmp(elem[L"name"].GetString(), aniName) == 0)
			{
				m_animation = &elem;
				m_isLoop = elem[L"loop"].GetBool();
				const auto& frames = elem[L"frames"].GetArray();
				m_frameCount = frames.Size();

				Reset();
				SetAnimationIndex(0);
				return;
			}
		}

		Utility::DebugString(L"[AnimatedActor] : Can't find animation %s", aniName);
	}
}

void AnimatedActor::SetAnimationIndex(int index)
{
	if (m_animation != nullptr)
	{
		const auto& frame = (*m_animation)[L"frames"].GetArray()[index];

		SetFrame(frame[L"name"].GetString());
		m_timePerFrame = frame[L"duration"].GetFloat();
	}
}
