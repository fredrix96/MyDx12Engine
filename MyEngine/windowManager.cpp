#include "windowManager.h"

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				gWindowRunning = false;
				DestroyWindow(hWnd);
			}
		}
		return 0;

	case WM_DESTROY: // x button on top right corner of window was pressed
		gWindowRunning = false;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,
		msg,
		wParam,
		lParam);
}

bool WindowManager::initializeWindow(HINSTANCE hInstance, int showWnd)
{
	if (mFullscreen)
	{
		HMONITOR hmon = MonitorFromWindow(mHwnd,
			MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hmon, &mi);

		mWidth = mi.rcMonitor.right - mi.rcMonitor.left;
		mHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;
	}

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mWindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	mHwnd = CreateWindowEx(NULL,
		mWindowName,
		mWindowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		mWidth, mHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!mHwnd)
	{
		MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	if (mFullscreen)
	{
		SetWindowLong(mHwnd, GWL_STYLE, 0);
	}

	ShowWindow(mHwnd, showWnd);
	UpdateWindow(mHwnd);

	return true;
}

WindowManager::WindowManager(HINSTANCE hInstance, int nShowCmd, int width, int height, bool fullscreen, LPCTSTR name, LPCTSTR title)
	: mHwnd(NULL), mWindowName(name), mWindowTitle(title), mWidth(width), mHeight(height), mFullscreen(fullscreen)
{
	mWindowCreated = initializeWindow(hInstance, nShowCmd);
}

WindowManager::~WindowManager()
{
}

int WindowManager::GetWidth() const
{
	return mWidth;
}

int WindowManager::GetHeight() const
{
	return mHeight;
}

bool WindowManager::IsWindowCreated() const
{
	return mWindowCreated;
}

bool WindowManager::IsFullscreen() const
{
	return mFullscreen;
}

HWND & WindowManager::GetHWND()
{
	return mHwnd;
}
