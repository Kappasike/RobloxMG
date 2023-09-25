#include "projectpicker.h"

namespace RMG {
	INT InitProjectPicker(FWindowGuiInfo& windowInfo)
	{
		WNDCLASSEXW wc{};
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = windowInfo.window_procedure;
		wc.hInstance = windowInfo.instance;
		wc.lpszClassName = L"RMG";
		wc.hIcon = LoadIcon(windowInfo.instance, MAKEINTRESOURCE(1));

		RegisterClassExW(&wc);

		HWND window = CreateWindow(wc.lpszClassName, L"Roblox Game Generator", WS_BORDER | WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 100, 125, GetSystemMetrics(SM_CXSCREEN) - 200, GetSystemMetrics(SM_CYSCREEN) - 250, NULL, NULL, windowInfo.instance, NULL);

		SendMessage(window, WM_SETICON, ICON_BIG, (LPARAM)wc.hIcon);
		SendMessage(window, WM_SETICON, ICON_SMALL, (LPARAM)wc.hIcon);

		{
			RECT client_area{};
			GetClientRect(window, &client_area);

			RECT window_area{};
			GetWindowRect(window, &window_area);

			POINT diff{};
			ClientToScreen(window, &diff);

			const MARGINS margins{
				window_area.left + (diff.x - window_area.left),
				window_area.top + (diff.y - window_area.top),
				client_area.right,
				client_area.bottom
			};

			DwmExtendFrameIntoClientArea(window, &margins);
		}

		//BOOL USE_DARK_MODE = TRUE;
		//DwmSetWindowAttribute(window, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &USE_DARK_MODE, sizeof(USE_DARK_MODE));

		DXGI_SWAP_CHAIN_DESC sd{};
		sd.BufferDesc.RefreshRate.Numerator = 240U;
		sd.BufferDesc.RefreshRate.Denominator = 1U;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1U;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2U;
		sd.OutputWindow = window;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		constexpr D3D_FEATURE_LEVEL levels[2]{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_0
		};

		ID3D11Device* device{ nullptr };
		ID3D11DeviceContext* device_context{ nullptr };
		IDXGISwapChain* swap_chain{ nullptr };
		ID3D11RenderTargetView* render_target_view{ nullptr };
		D3D_FEATURE_LEVEL level{};

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0U,
			levels,
			2U,
			D3D11_SDK_VERSION,
			&sd,
			&swap_chain,
			&device,
			&level,
			&device_context
		);

		ID3D11Texture2D* back_buffer{ nullptr };
		swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

		if (back_buffer)
		{
			device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
			back_buffer->Release();
		}
		else {
			return 1;
		}

		ShowWindow(window, windowInfo.cmdShow);
		UpdateWindow(window);
		ImGui::CreateContext();

		RMG::GUIProjectPickerWindow guiWindow{};

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(device, device_context);

		bool running = 1;
		while (running) {
			MSG message;
			while (PeekMessage(&message, nullptr, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);

				if (message.message == WM_QUIT)
				{
					running = 0;
				}
			}
			if (!running) break;
			std::this_thread::sleep_for(std::chrono::milliseconds(2));

			//hi

			if (swap_chain)
			{
				device_context->OMSetRenderTargets(0, 0, 0);
				render_target_view->Release();

				HRESULT hr;
				hr = swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

				ID3D11Texture2D* pBuffer;
				hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

				hr = device->CreateRenderTargetView(pBuffer, NULL, &render_target_view);

				pBuffer->Release();

				device_context->OMSetRenderTargets(1, &render_target_view, NULL);

				D3D11_VIEWPORT vp;
				vp.Width = 0;
				vp.Height = 0;
				vp.MinDepth = 0.0f;
				vp.MaxDepth = 1.0f;
				vp.TopLeftX = 0;
				vp.TopLeftY = 0;
				device_context->RSSetViewports(1, &vp);
			}

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			guiWindow.update();

			// rendering stuff below here

			ImGui::Render();

			constexpr float color[4]{ 0.f,0.f,0.f,0.f };
			device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
			device_context->ClearRenderTargetView(render_target_view, color);

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			swap_chain->Present(0U, 0U); // no v sync for me bro
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		ImGui::DestroyContext();

		if (swap_chain) swap_chain->Release();

		if (device_context) device_context->Release();

		if (device) device->Release();

		if (render_target_view) render_target_view->Release();

		DestroyWindow(window);
		UnregisterClassW(wc.lpszClassName, wc.hInstance);

		*windowInfo.nextWindow = ENEXT_WINDOW::NW_EXIT;

		return 0;
	}

	void EndProjectPicker()
	{

	}
}