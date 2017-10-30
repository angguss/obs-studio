project(win-virtual-desktop-switch)

if(DISABLE_WIN_VIRT_DESKTOP)
	message(STATUS "win virtual desktop switch plugin disabled")
	return()
endif()

include(IDLFileHelper)

set(win-decklink_HEADERS
	win-virt-desktop.h
	)

set(win-decklink_SOURCES
	plugin-main.cpp
	)

# add_idl_files(win-decklink-sdk_GENERATED_FILES
# 	${win-decklink-sdk_IDLS})

include_directories(
	${CMAKE_CURRENT_BINARY_DIR})

add_library(win-virtual-desktop-switch MODULE
	${win-decklink_SOURCES}
	${win-decklink_HEADERS})

target_link_libraries(win-virtual-desktop-switch
	libobs)

install_obs_plugin_with_data(win-virtual-desktop-switch ../data)
