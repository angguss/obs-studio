#pragma once

#include <obs-module.h>

#include <string>
#include <objbase.h>
#include <ObjectArray.h>
#include <vector>

// CLSID/Com interfaces pulled from
// http://www.cyberforum.ru/blogs/105416/blog3671.html

const CLSID CLSID_ImmersiveShell = {
	0xC2F03A33, 0x21F5, 0x47FA, 0xB4, 0xBB, 0x15, 0x63, 0x62, 0xA2, 0xF2, 0x39 };

const CLSID CLSID_VirtualDesktopAPI_Unknown = {
	0xC5E0CDCA, 0x7B6E, 0x41B2, 0x9F, 0xC4, 0xD9, 0x39, 0x75, 0xCC, 0x46, 0x7B };

const IID IID_IVirtualDesktopManagerInternal = {
	0xEF9F1A6C, 0xD3CC, 0x4358, 0xB7, 0x12, 0xF8, 0x4B, 0x63, 0x5B, 0xEB, 0xE7 };

const CLSID CLSID_IVirtualNotificationService = {
	0xA501FDEC, 0x4A09, 0x464C, 0xAE, 0x4E, 0x1B, 0x9C, 0x21, 0xB8, 0x49, 0x18 };

EXTERN_C const IID IID_IVirtualDesktopManagerInternal;

struct IApplicationView : public IUnknown
{
public:

};

EXTERN_C const IID IID_IVirtualDesktop;

MIDL_INTERFACE("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4")
IVirtualDesktop : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE IsViewVisible(
		IApplicationView *pView,
		int *pfVisible) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetID(
		GUID *pGuid) = 0;
};

enum AdjacentDesktop
{
	// We can go up/down too?
	LeftDirection = 3,
	RightDirection = 4
};
//10240
//MIDL_INTERFACE("AF8DA486-95BB-4460-B3B7-6E7A6B2962B5")
//unknown build
//MIDL_INTERFACE("AF8DA486-95BB-4460-B3B7-6E7A6B2962B5")
//15063
MIDL_INTERFACE("f31574d6-b682-4cdc-bd56-1827860abec6")
IVirtualDesktopManagerInternal : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetCount(
		UINT *pCount) = 0;

	virtual HRESULT STDMETHODCALLTYPE MoveViewToDesktop(
		IApplicationView *pView,
		IVirtualDesktop *pDesktop) = 0;

	// 10240
	virtual HRESULT STDMETHODCALLTYPE CanViewMoveDesktops(
		IApplicationView *pView,
		int *pfCanViewMoveDesktops) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetCurrentDesktop(
		IVirtualDesktop** desktop) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetDesktops(
		IObjectArray **ppDesktops) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetAdjacentDesktop(
		IVirtualDesktop *pDesktopReference,
		AdjacentDesktop uDirection,
		IVirtualDesktop **ppAdjacentDesktop) = 0;

	virtual HRESULT STDMETHODCALLTYPE SwitchDesktop(
		IVirtualDesktop *pDesktop) = 0;

	virtual HRESULT STDMETHODCALLTYPE CreateDesktopW(
		IVirtualDesktop **ppNewDesktop) = 0;

	virtual HRESULT STDMETHODCALLTYPE RemoveDesktop(
		IVirtualDesktop *pRemove,
		IVirtualDesktop *pFallbackDesktop) = 0;

	// 10240
	virtual HRESULT STDMETHODCALLTYPE FindDesktop(
		GUID *desktopId,
		IVirtualDesktop **ppDesktop) = 0;
};

std::string GuidToString(const GUID &guid);
std::string WStringToUTF8(const std::wstring &str);

class WinVirtDesktop
{
protected:
	bool isInCurrentVirtualDesktop = false;
	std::string currentDesktopId;
	std::string chosenDesktopId;
	std::vector<std::string> desktops;
	// Fill our desktops vector with data
	void EnumVirtualDesktops();

	// This can be public for now, we'll sort it out properly later
public:
	IServiceProvider *pServiceProvider = nullptr;
	IVirtualDesktopManagerInternal *pDesktopManagerInternal = nullptr;

	// Get the list
	std::vector<std::string> &GetDesktops();
	
	unsigned int GetDesktopCount();

	WinVirtDesktop();
	~WinVirtDesktop();

};
