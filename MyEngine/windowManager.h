#pragma once

#include <windows.h>
#include <windowsx.h>
#include "stdafx.h"

bool static gWindowRunning = true; //TODO: Come up with something better

// Callback function for windows messages
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class WindowManager
{
private:
	HWND mHwnd; // Handle to the window

	LPCTSTR mWindowName;
	LPCTSTR mWindowTitle;

	int mWidth;
	int mHeight;

	bool mFullscreen;
	bool mWindowCreated;

	bool initializeWindow(HINSTANCE hInstance, int nShowCmd);

public:

	WindowManager(); // default
	WindowManager(HINSTANCE hInstance,
		int nShowCmd,
		int width, int height,
		bool fullscreen,
		LPCTSTR name, LPCTSTR title);

	~WindowManager();

	int GetWidth() const;
	int GetHeight() const;

	bool IsWindowCreated() const;
	bool IsFullscreen() const;

	HWND& GetHWND();
};
