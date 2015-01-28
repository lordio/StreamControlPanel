#include "Window.hpp"

WNDCLASSEX Window::cls{
	sizeof(WNDCLASSEX), //cbSize
	CS_HREDRAW | CS_VREDRAW, //style
	DefWindowProc, //lpfnWndProc
	0, //cbClsExtra
	0, //cbWndExtra
	GetModuleHandle(NULL), //hInstance
	LoadIcon(NULL, IDI_APPLICATION), //hIcon
	LoadCursor(NULL,IDC_ARROW), //hCursor
	(HBRUSH) COLOR_WINDOW, //hbrBackground
	NULL, //lpszMenuName
	TEXT("TopLevelWindow"), //lpszClassName
	LoadIcon(NULL, IDI_APPLICATION) //hIconSm
};

Window::RegCls Window::_c;

Window::RegCls::RegCls()
{
	RegisterClassEx(&Window::cls);
}
Window::RegCls::~RegCls()
{
	UnregisterClass(Window::cls.lpszClassName, GetModuleHandle(NULL));
}

Window::Window(tstring title, RECT rect, bool hasMenu, bool resizable, HFONT font) :
	BaseWindow{ cls.lpszClassName, title, WS_OVERLAPPEDWINDOW & ~(resizable ? 0 : WS_THICKFRAME), WS_EX_OVERLAPPEDWINDOW, rect, nullptr, font }
{
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, (hasMenu ? TRUE : FALSE), WS_EX_OVERLAPPEDWINDOW);
	MoveWindow(wnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}
Window::~Window()
{
}