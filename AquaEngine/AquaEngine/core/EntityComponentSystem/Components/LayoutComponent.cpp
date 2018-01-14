#include "LayoutComponent.h"
#include "core\DirectX\DxResourcesManager.h"

#include <Windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace aqua
{
namespace ecs
{

std::vector<uint8_t> LoadPackageFile(const std::wstring & _path) 
{
  std::vector<uint8_t> fileData;

  auto file = CreateFileW(_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

  BOOL res = false;

  if (file != INVALID_HANDLE_VALUE) 
  {
    DWORD readed;
    LARGE_INTEGER pos, newPos, fileSize;

    pos.QuadPart = 0;
    newPos.QuadPart = 0;

    SetFilePointerEx(file, pos, &newPos, FILE_END);
    fileSize = newPos;
    SetFilePointerEx(file, pos, &newPos, FILE_BEGIN);

    fileData.resize(static_cast<size_t>(fileSize.QuadPart));

    res = ReadFile(file, fileData.data(), (DWORD)fileData.size(), &readed, nullptr);

    CloseHandle(file);
  }

  if (res == FALSE)
    return std::vector<uint8_t>();
  else
    return fileData;
}

bool LayoutComponent::InitializeComponent()
{
	std::wstring vs = L"E:\\Projects\\Project_Aqua\\Project_Aqua\\AquaEngine\\!VS_TMP\\Build\\v140\\Debug\\Win32\\SimpleVS.cso";
	std::wstring ps = L"E:\\Projects\\Project_Aqua\\Project_Aqua\\AquaEngine\\!VS_TMP\\Build\\v140\\Debug\\Win32\\SimplePS.cso";

	std::vector<uint8_t> vsData;
	std::vector<uint8_t> psData;

	vsData = LoadPackageFile(vs);
	HRESULT result = DxResourcesManager::GetInstance()->GetD3DDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_vertexShader);
	//assert

	psData = LoadPackageFile(ps);
	result = DxResourcesManager::GetInstance()->GetD3DDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_pixelShader);
	//assert

	D3D11_INPUT_ELEMENT_DESC layoutdesc[2];
	layoutdesc[0].SemanticName = "POSITION";
	layoutdesc[0].SemanticIndex = 0;
	layoutdesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layoutdesc[0].InputSlot = 0;
	layoutdesc[0].AlignedByteOffset = 0;
	layoutdesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutdesc[0].InstanceDataStepRate = 0;

	layoutdesc[1].SemanticName = "COLOR";
	layoutdesc[1].SemanticIndex = 0;
	layoutdesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layoutdesc[1].InputSlot = 0;
	layoutdesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layoutdesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutdesc[1].InstanceDataStepRate = 0;

	result = DxResourcesManager::GetInstance()->GetD3DDevice()->CreateInputLayout(layoutdesc, 2,
		vsData.data(), vsData.size(), &m_layout);
	//assert

	return SUCCEEDED(result);
}

}
}
