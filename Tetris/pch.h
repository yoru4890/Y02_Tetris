//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <dxgi1_6.h>

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <array>
#include <map>
#include <list>
#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <cmath>
#include <iostream>

#include <stdio.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#pragma warning(push)
#pragma warning(disable:26812)

#include "Audio.h"
#include "CommonStates.h"
#include "WICTextureLoader.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PrimitiveBatch.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"

#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:26812)
#pragma warning(disable:26451)
#pragma warning(disable:26495)
#pragma warning(disable:6319)
#pragma warning(disable:6386)
#pragma warning(disable:6385)
#include "rapidjson/document.h"
#pragma warning(pop)

#include "GameConstants.h"
#include "Utility.h"
#include "StepTimer.h"
#include "DeviceResources.h"
#include "TextureManager.h"
#include "JSONManager.h"
#include "Actor.h"
#include "AnimatedActor.h"
#include "ActorManager.h"
#include "TileBase.h"
#include "TileI.h"
#include "Game.h"

namespace DX
{
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) noexcept : result(hr) {}

		const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(
				s_str,
				"Failure with HRESULT of %08X",
				static_cast<unsigned int>(result)
			);
			return s_str;
		}

	private:
		HRESULT result;
	};
		inline void ThrowIfFailed(HRESULT hr)
		{
			if (FAILED(hr))
			{
				throw com_exception(hr);
			}
		}

}