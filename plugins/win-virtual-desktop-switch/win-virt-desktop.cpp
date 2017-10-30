#include "win-virt-desktop.h"
#include <locale>
#include <codecvt>

std::string WStringToUTF8(const std::wstring &str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}
std::string GuidToString(const GUID &guid)
{
	std::wstring guidStr(40, L'\0');
	::StringFromGUID2(guid, const_cast<LPOLESTR>(guidStr.c_str()), guidStr.length());

	return WStringToUTF8(guidStr);
}

void WinVirtDesktop::EnumDesktops()
{
	IObjectArray *pObjectArray = nullptr;
	HRESULT hr = pDesktopManagerInternal->GetDesktops(&pObjectArray);

	if (SUCCEEDED(hr))
	{
		UINT count;
		hr = pObjectArray->GetCount(&count);
		if (SUCCEEDED(hr))
		{
			// We got this far so we'll clear our current cache
			desktops.clear();
			for (UINT i = 0; i < count; i++)
			{
				IVirtualDesktop *pDesktop = nullptr;
				if (FAILED(pObjectArray->GetAt(i, __uuidof(IVirtualDesktop),
					(void**)&pDesktop)))
					continue;

				GUID id = { 0 };
				if (SUCCEEDED(pDesktop->GetID(&id)))
				{
					desktops.push_back(GuidToString(id));
				}
				pDesktop->Release();
			}

			// Return and release
			pObjectArray->Release();
		}
	}
}

unsigned int WinVirtDesktop::GetDesktopCount()
{
	IObjectArray *pObjectArray = nullptr;
	HRESULT hr = pDesktopManagerInternal->GetDesktops(&pObjectArray);

	if (SUCCEEDED(hr))
	{
		UINT count;
		hr = pObjectArray->GetCount(&count);
		if(SUCCEEDED(hr))
		{
			// Return and release
			pObjectArray->Release();
			return count;
		}
	}

	// If we can't enum, assume 1
	return 1;
}

WinVirtDesktop::WinVirtDesktop()
{
	//TODO: Move COM initialization here
}


WinVirtDesktop::~WinVirtDesktop()
{
	// Release the COM objects
	if (pDesktopManagerInternal != nullptr)
		pDesktopManagerInternal->Release();
	if (pServiceProvider != nullptr)
		pServiceProvider->Release();
}
