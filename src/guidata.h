#pragma once
#include <d3d11.h>
#include <iostream>
#include <format>
#include <array>
#include <Windows.h>
#include <dwmapi.h>
#include "gui/gui.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

enum ENEXT_WINDOW { NW_PROJECTPICKER, NW_PROJECTCREATOR, NW_EXIT };

struct FWindowGuiInfo {
	HINSTANCE instance;
	INT cmdShow;
	LRESULT(CALLBACK* window_procedure)(HWND, UINT, WPARAM, LPARAM);
	ENEXT_WINDOW* nextWindow;
};

