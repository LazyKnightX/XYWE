#include "DllModule.h"
#include <Windows.h>

static BOOL IsForegroundWindow(HWND hWar3Wnd)
{
	HWND hWnd = ::GetParent(hWar3Wnd);
	if (NULL == hWnd)
	{
		return (::GetForegroundWindow() == hWar3Wnd);
	}
	else
	{
		return (::GetForegroundWindow() == hWnd);
	}
}

void LockingMouse()
{
	static BOOL s_bClipped = FALSE;

	if ((g_DllMod.IsWindowMode)                     // ����ģʽ
		&& (g_DllMod.IsLockingMouse)                // ��������ħ�������
		&& (g_DllMod.hWar3Wnd != NULL)              // ħ�޴��ڴ���
		&& IsForegroundWindow(g_DllMod.hWar3Wnd))   // ħ�޴��ڴ��ڼ���״̬
	{
		RECT rcClient;
		POINT Point;
		::GetCursorPos(&Point);
		::GetClientRect(g_DllMod.hWar3Wnd, &rcClient);
		::ClientToScreen(g_DllMod.hWar3Wnd, (LPPOINT)&rcClient); 
		::ClientToScreen(g_DllMod.hWar3Wnd, (LPPOINT)&rcClient+1);
		if(::PtInRect(&rcClient, Point))
		{
			::ClipCursor(&rcClient);
			s_bClipped = TRUE;
		}
	}
	else if(s_bClipped)
	{
		::ClipCursor(NULL);
		s_bClipped = FALSE;
	}
}
