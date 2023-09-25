#include "guidata.h"
#include "gui/gui.h"
#include "windows/projectpicker.h"
#include "windows/projectcreator.h"
#include "windows/minimenu.h"

#define STB_IMAGE_IMPLEMENTATION
#include <imageloader.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return 0L;
	}

	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0L;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmdShow)
{
	ENEXT_WINDOW nextWindow = NW_EXIT;
	FWindowGuiInfo windowInfo{};
	windowInfo.instance = instance;
	windowInfo.cmdShow = cmdShow;
	windowInfo.window_procedure = window_procedure;
	windowInfo.nextWindow = &nextWindow;

	RMG::InitMiniMenu(windowInfo);
	while (nextWindow != ENEXT_WINDOW::NW_EXIT)
	{
		switch (nextWindow)
		{
		case NW_PROJECTPICKER:
			RMG::InitProjectPicker(windowInfo);
			break;
		case NW_PROJECTCREATOR:
			RMG::InitProjectCreator(windowInfo);
			break;
		case NW_EXIT:
			return 0;
		}
	}
	
	return 0;
}