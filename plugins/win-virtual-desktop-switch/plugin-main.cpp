#include <obs-module.h>

#define PLUGIN_NAME "win-virtual-desktop-switch"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

bool obs_module_load(void)
{

	return true;
}

void obs_module_unload(void)
{
	
}