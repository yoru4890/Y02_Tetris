#include "pch.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DX;

TextureManager::TextureManager() : m_pDeviceResources{ nullptr }
{

}

TextureManager::~TextureManager()
{
	ReleaseAll();
}

void TextureManager::OnDeviceLost()
{
	for (auto& resource : m_shaderResources)
	{
		resource.second.Reset();
	}
}

void TextureManager::OnDeviceRestored()
{
	auto device = m_pDeviceResources->GetD3DDevice();
	for (auto& resource : m_shaderResources)
	{
		CreateWICTextureFromFile(
			device,
			resource.first.c_str(),
			nullptr,
			resource.second.ReleaseAndGetAddressOf()
		);
	}
}

void TextureManager::Initialize(DeviceResources* pDeviceResources)
{
	assert(pDeviceResources != nullptr);
	m_pDeviceResources = pDeviceResources;
}

void TextureManager::ReleaseAll()
{
	for (auto& resource : m_shaderResources)
	{
		resource.second.Reset();
	}
	m_shaderResources.clear();
}

ID3D11ShaderResourceView* TextureManager::CreateShaderResourceView(LPCWSTR filename)
{
	auto device = m_pDeviceResources->GetD3DDevice();
	assert(device != nullptr);
	auto result = m_shaderResources.insert(std::make_pair(filename, nullptr));
	
	if (result.second == true)
	{
		auto view = ComPtr<ID3D11ShaderResourceView>();
		CreateWICTextureFromFile(device, filename, NULL, view.GetAddressOf());
		result.first->second = view.Detach();
	}

	return result.first->second.Get();
}

void TextureManager::Remove(LPCWSTR filename)
{
	m_shaderResources.erase(filename);
}