#pragma once
#include "../guidata.h"

namespace RMG {
	class GUIMiniMenuWindow {
	public:
		GUIMiniMenuWindow();
		~GUIMiniMenuWindow() {};

		void update();

		bool createProjectPressed = false;
		bool openProjectPressed = false;
		bool exitPressed = false;

		ID3D11ShaderResourceView* icon_texture;

	private:
		void init();
	};

	class GUIProjectCreatorWindow {
	public:
		GUIProjectCreatorWindow();
		~GUIProjectCreatorWindow() {};

		void update();

		ID3D11ShaderResourceView* obby_button_texture;

	private:
		void init();
	};
	
	class GUIProjectPickerWindow {
	public:
		GUIProjectPickerWindow();
		~GUIProjectPickerWindow() {};

		void update();
	private:
		void init();
	};
}