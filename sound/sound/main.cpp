#pragma comment(lib,"winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <string>

NOTIFYICONDATA Icon = { 0 };
HWND window;
short indexOfHotkeys = 0;
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdsh)
{
	MSG SoftwareMainMessage = { 0 };

	WNDCLASSEX main = { 0 };
	main.cbSize = sizeof(WNDCLASSEX);
	main.hInstance = hInst;
	main.lpszClassName = TEXT("Main");
	main.lpfnWndProc = SoftwareMainProcedure;
	RegisterClassEx(&main);

	window = CreateWindowEx(0, TEXT("Main"), NULL, 0, 0, 0, 0, 0, NULL, NULL, hInst, NULL);

	Icon.cbSize = sizeof(NOTIFYICONDATA);
	Icon.hWnd = window;
	Icon.uVersion = NOTIFYICON_VERSION;
	Icon.uCallbackMessage = WM_USER;
	Icon.hIcon = (HICON)LoadImage(NULL, TEXT("golds.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	Icon.uFlags = NIF_MESSAGE | NIF_ICON;
	Shell_NotifyIcon(NIM_ADD, &Icon);

	RegisterHotKey(NULL, 1, NULL, 0x34);

	INPUT inputs[1];
	UINT ret;

	const char array[4]{ 0x35 , 0x36, 0x37, 0x38 };

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wScan = 0;
	inputs[0].ki.dwFlags = 0;
	inputs[0].ki.time = 0;
	inputs[0].ki.dwExtraInfo = 0;

	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL))
	{
		if (SoftwareMainMessage.message == WM_HOTKEY)
		{
			if (indexOfHotkeys == 4)
				indexOfHotkeys = 0;

			inputs[0].ki.wVk = array[indexOfHotkeys];
			ret = SendInput(1, inputs, sizeof(INPUT));


			++indexOfHotkeys;
		}

		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}
	return 0;
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) 
	{
	case WM_CREATE:
		break;

	case WM_USER:
		if (lp == WM_RBUTTONDOWN)
			if (MessageBox(NULL, TEXT("Delete process?"), TEXT("Tray"), MB_YESNO) == IDYES)
				DestroyWindow(window);
		break;

	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &Icon);
		PostQuitMessage(0);
		break;
	
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}