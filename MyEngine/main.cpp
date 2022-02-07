#include "Dx12Engine.h"

void mainloop(Dx12Engine& dx12Engine); // main application loop

void Update(Dx12Engine& dx12Engine); // update the game logic

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nShowCmd)
{
	// Create the engine
	Dx12Engine dx12Engine;
	
	// Enable debug layer
	dx12Engine.EnableDebugLayer();

	// Create the window
	dx12Engine.CreateMainWindow(
		hInstance, nShowCmd,
		1920, 1080,
		false,
		L"DirectX window", L"My engine");

	if (!dx12Engine.GetMainWindow().IsWindowCreated())
	{
		MessageBox(0, L"Window creation - Failed", L"Error", MB_OK);
		return 1;
	}

	// Initialize DirectX12 components
	if (!dx12Engine.Initialize())
	{
		MessageBox(0, L"Failed to initialize DirectX12", L"Error", MB_OK);
		return 1;
	}

	// Start the main loop
	mainloop(dx12Engine);

	return 0;
}

void mainloop(Dx12Engine& dx12Engine)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (gWindowRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// run game code
			Update(dx12Engine); // update the game logic
			dx12Engine.Render(); // execute the command queue (rendering the scene is the result of the gpu executing the command lists)
		}
	}
}

void Update(Dx12Engine& dx12Engine)
{
	// update app logic, such as moving the camera or figuring out what objects are in view
	dx12Engine.UpdateGameLogic();
}