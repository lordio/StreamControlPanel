#ifndef BASEWINDOW
#define BASEWINDOW

#include <Windows.h>
#include <string>
#include <CommCtrl.h>

#ifdef UNICODE
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

class BaseWindow
{
	UINT_PTR nextSubclass{};
protected:
	HWND wnd;
public:
	BaseWindow(tstring wClass, tstring text, DWORD styles, DWORD exStyles, RECT rect, BaseWindow * parent, HFONT font = NULL);
	virtual ~BaseWindow();

	HWND GetWindow() const;

	void SetFont(HFONT font);

	tstring GetText() const;
	void SetText(tstring text);

	void RemoveSubclass(UINT_PTR id, SUBCLASSPROC proc);
	UINT_PTR SetSubclass(SUBCLASSPROC proc, void * ptr);
};

#endif