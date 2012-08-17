/**
 * \file main.cpp
 *
 * \section COPYRIGHT
 *
 * Dart Embedding Example
 *
 * ---------------------------------------------------------------------
 *
 * Copyright (c) 2012 Don Olmstead
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any source
 *   distribution.
 */

#include <DartEmbed/GamePad.hpp>
#include <DartEmbed/Isolate.hpp>
#include "PlatformWindows.hpp"
using namespace DartEmbed;

namespace
{
	/// Handle to the window
	HWND handle;

	//---------------------------------------------------------------------

	void InitWindow(std::int32_t width, std::int32_t height)
	{
		// Set the window styles
		DWORD dwStyle   = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		DWORD dwExStyle = WS_EX_APPWINDOW;

		// Get the window size
		RECT rect;
		rect.left   = 0;
		rect.right  = width - 1;
		rect.top    = 0;
		rect.bottom = height - 1;

		AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

		std::int32_t fullWidth  = rect.right - rect.left + 1;
		std::int32_t fullHeight = rect.bottom + rect.top + 1;

		// Get the working area
		RECT wa;

		SystemParametersInfo(SPI_GETWORKAREA, 0, &wa, 0);

		// Create the window
		handle = CreateWindowExA
		(
			dwExStyle,
			WINDOW_CLASS_NAME,
			WINDOW_CLASS_NAME,
			dwStyle,
			wa.left, wa.top,
			fullWidth,
			fullHeight,
			0,
			0,
			0,
			0
		);
	}

	//---------------------------------------------------------------------

	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	//---------------------------------------------------------------------

	DWORD WINAPI __scriptThread(LPVOID param)
	{
		// Load the script and invoke
		Isolate* isolate = Isolate::loadScript("server.dart");
		isolate->invokeFunction("main");

		return 0;
	}
} // end anonymous namespace

//---------------------------------------------------------------------

int main()
{
	// Register window class
	WNDCLASSEXA wc;
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_CLASSDC;
	wc.lpfnWndProc   = MsgProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = GetModuleHandle(0);
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm       = 0;

	RegisterClassExA(&wc);

	// Open the window
	InitWindow(640, 480);

	// Initialize the virtual machine
	VirtualMachine::initialize();

	// Start the thread
	DWORD scriptThreadId;
	HANDLE scriptThread = CreateThread(
		0,
		0,
		__scriptThread,
		0,
		0,
		&scriptThreadId
	);

	// Start the message pump
	MSG msg = {0};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			updateGamePads();
			//updateInterpreter();
		}
	}

	// Destroy the virtual machine
	VirtualMachine::terminate();

	// Wait for the thread to exit
	WaitForSingleObject(scriptThread, INFINITE);

	return 0;
}
