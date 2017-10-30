#include <obs-module.h>
#include <objbase.h>

#include "win-virt-desktop.h"


#define PLUGIN_NAME "win-virtual-desktop-switch"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

// How this should work:
// 1 - List virtual desktops
// 2 - Choose one to remain persistant
// 3 - If the current virtual desktop is not equal to the chosen, 
//	   place a black box on screen
// 4 - Update this every loop

static void *win_virt_create(obs_data_t *settings, obs_source_t *source)
{
	WinVirtDesktop *winvirt = new WinVirtDesktop();

	// Let's do some COM stuff
	// Initialize COM interface. Does OBS already do this elsewhere?
	::CoInitialize(NULL);
	IServiceProvider *pServiceProvider = nullptr;

	HRESULT hr = ::CoCreateInstance(CLSID_ImmersiveShell, NULL, CLSCTX_LOCAL_SERVER,
		__uuidof(IServiceProvider), (PVOID*)&pServiceProvider);

	if (SUCCEEDED(hr))
	{
		winvirt->pServiceProvider = pServiceProvider;

		IVirtualDesktopManagerInternal* pDesktopManagerInternal = nullptr;
		hr = pServiceProvider->QueryService(CLSID_VirtualDesktopAPI_Unknown,
			&pDesktopManagerInternal);

		if (SUCCEEDED(hr))
		{
			winvirt->pDesktopManagerInternal = pDesktopManagerInternal;
		}
	}

	return winvirt;
}

static void win_virt_destroy(void *data)
{
	WinVirtDesktop *winvirt = (WinVirtDesktop*)data;
	// This will clearup leftover COM stuff
	delete winvirt;
}

static void win_virt_update(void *data, obs_data_t *settings)
{
	// We'll stick the checking logic here.
}

static bool win_virt_desktop_changed(obs_properties_t *props,
	obs_property_t *list, obs_data_t *settings)
{
	// If this is changed we want to get the data we created
	// and change the selected desktop.
	return true;
}

static obs_properties_t *win_virt_get_properties(void *data)
{
	WinVirtDesktop *winvirt = (WinVirtDesktop*)data;

	obs_properties_t *props = obs_properties_create();

	obs_property_t *list = obs_properties_add_list(props, "desktop_id",
		"List of current virtual desktops",
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_set_modified_callback(list, win_virt_desktop_changed);

	std::vector<std::string> &desktops = winvirt->GetDesktops();
	for (auto it = desktops.begin(), end = desktops.end(); it != end; it++)
		obs_property_list_add_string(list, (*it).c_str(), (*it).c_str());

	//obs_property_list_add_string(list, "desktop1", "Desktop1guid");

	obs_properties_add_bool(props, "use_cur_desktop", "Just use whatever desktop OBS is on");

	UNUSED_PARAMETER(data);

	return props;
}

static const char *win_virt_get_name(void*)
{
	return PLUGIN_NAME;
}

static void win_virt_get_defaults(obs_data_t *settings)
{
	obs_data_set_default_bool(settings, "use_cur_desktop", false);
}

bool obs_module_load(void)
{
	struct obs_source_info info = {};

	info.id = PLUGIN_NAME;
	info.type = OBS_SOURCE_TYPE_INPUT;
	// We'll pretend we're an async video source for now
	// so we get called every frame? seems reasonable
	info.output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_DO_NOT_DUPLICATE;
	
	info.create = win_virt_create;
	info.destroy = win_virt_destroy;
	info.update = win_virt_update;

	info.get_properties = win_virt_get_properties;
	info.get_name = win_virt_get_name;
	info.get_defaults = win_virt_get_defaults;

	obs_register_source(&info);

	return true;
}

void obs_module_unload(void)
{
	
}