#include <Windows.h>
#include <iostream>

enum
{
	MAIN,
	SERIAL_MONITOR,
	REGISTERS,
	MEMORY,
	LAYERS,
	COUNT
};

const wchar_t* WINDOW_NAMES[COUNT] = {
	L"POCU Breadboard Simulator",
	L"Serial Monitor",
	L"Registers",
	L"Memory",
	L"Layers",
};

const UINT WINDOW_FLAGS[COUNT] = {
	SWP_SHOWWINDOW,
	SWP_SHOWWINDOW,
	SWP_SHOWWINDOW,
	SWP_SHOWWINDOW,
	SWP_SHOWWINDOW | SWP_NOSIZE,
};

int wmain()
{
	HWND hWnds[COUNT] = { nullptr };
	DWORD pids[COUNT] = { 0 };
	RECT rcs[COUNT] = { 0 };
	
	int windowIndex;

	for (windowIndex = MAIN; windowIndex < COUNT; ++windowIndex)
	{
		hWnds[windowIndex] = FindWindowW(nullptr, WINDOW_NAMES[windowIndex]);
		if (hWnds[windowIndex] != nullptr)
		{
			GetWindowThreadProcessId(hWnds[windowIndex], &pids[windowIndex]);
		}
	}

	if (hWnds[MAIN] == nullptr)
	{
		return 1;
	}
	if (pids[MAIN] == 0)
	{
		return 2;
	}

	HMONITOR hMonitor = MonitorFromWindow(hWnds[MAIN], MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	if (GetMonitorInfoW(hMonitor, &mi) == 0) {
		return 3;
	}

	long workingAreaWidth = mi.rcWork.right - mi.rcWork.left;
	long workingAreaHeight = mi.rcWork.bottom - mi.rcWork.top;

	rcs[SERIAL_MONITOR].left = mi.rcWork.left;
	rcs[SERIAL_MONITOR].right = rcs[SERIAL_MONITOR].left + 300;
	rcs[SERIAL_MONITOR].top = mi.rcWork.top;
	rcs[SERIAL_MONITOR].bottom = workingAreaHeight;

	rcs[REGISTERS].left = rcs[SERIAL_MONITOR].right;
	rcs[REGISTERS].right = rcs[REGISTERS].left + 600;
	rcs[REGISTERS].top = mi.rcWork.top;
	rcs[REGISTERS].bottom = rcs[REGISTERS].top + 200;

	rcs[MEMORY].left = rcs[SERIAL_MONITOR].right;
	rcs[MEMORY].right = rcs[MEMORY].left + 600;
	rcs[MEMORY].top = rcs[REGISTERS].bottom;
	rcs[MEMORY].bottom = workingAreaHeight;

	GetWindowRect(hWnds[LAYERS], &rcs[LAYERS]);
	rcs[LAYERS].left = rcs[SERIAL_MONITOR].right;
	rcs[LAYERS].top = workingAreaHeight - (rcs[LAYERS].bottom - rcs[LAYERS].top);
	rcs[LAYERS].right = 0;
	rcs[LAYERS].bottom = 0;

	rcs[MAIN].left = max(rcs[REGISTERS].right, rcs[MEMORY].right);
	rcs[MAIN].right = workingAreaWidth;
	rcs[MAIN].top = mi.rcWork.top;
	rcs[MAIN].bottom = workingAreaHeight;

	for (windowIndex = MAIN; windowIndex < COUNT; ++windowIndex)
	{
		if (hWnds[windowIndex] == nullptr || pids[MAIN] != pids[windowIndex])
		{
			continue;
		}
		SetWindowPos(hWnds[windowIndex], HWND_TOP, rcs[windowIndex].left, rcs[windowIndex].top, rcs[windowIndex].right - rcs[windowIndex].left, rcs[windowIndex].bottom - rcs[windowIndex].top, WINDOW_FLAGS[windowIndex]);
		ShowWindow(hWnds[windowIndex], SW_SHOWNORMAL);
		SetForegroundWindow(hWnds[windowIndex]);
	}

	return 0;
}
