#pragma once

#include <obs-module.h>

#include <string>

class WinVirtDesktopDetector
{
protected:
	bool isInCurrentVirtualDesktop = false;
	std::string currentDesktopId;
	std::string chosenDesktopId;
};
