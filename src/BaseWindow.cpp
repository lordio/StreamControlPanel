#include "BaseWindow.hpp"
#include <memory>
#include <windowsx.h>

BaseWindow::BaseWindow(tstring wClass, 
	tstring text, 
	DWORD styles, 
	DWORD exStyles, 
	RECT rect, 
	BaseWindow * parent, 
	HFONT font) :
	wnd{ CreateWindowEx(exStyles, 
		wClass.c_str(), 
		text.c_str(), 
		styles | WS_VISIBLE, 
		rect.left, rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		(parent ? parent->GetWindow() : NULL),
		NULL,
		GetModuleHandle(NULL),
		NULL) }
{
	if (font != NULL) SetFont(font);
}
BaseWindow::~BaseWindow()
{
	DestroyWindow(wnd);
}

HWND BaseWindow::GetWindow() const
{
	return wnd;
}
tstring BaseWindow::GetText() const
{
	auto len = GetWindowTextLength(wnd) + 1;
	std::unique_ptr<TCHAR []> tp{ new TCHAR[len] };
	GetWindowText(wnd, tp.get(), len);
	return tp.get();
}
void BaseWindow::SetText(tstring str)
{
	SetWindowText(wnd, str.c_str());
}
void BaseWindow::SetFont(HFONT font)
{
	SetWindowFont(wnd, font, TRUE);
}

void BaseWindow::RemoveSubclass(UINT_PTR id, SUBCLASSPROC proc)
{
	RemoveWindowSubclass(wnd, proc, id);
}
UINT_PTR BaseWindow::SetSubclass(SUBCLASSPROC proc, void * ptr)
{
	SetWindowSubclass(wnd, proc, nextSubclass, reinterpret_cast<DWORD_PTR>(ptr));
	return nextSubclass++;
}