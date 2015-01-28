#ifndef BUTTON
#define BUTTON

#include "BaseWindow.hpp"
#include <functional>

class Button : public BaseWindow
{
public:
	using OnClick = std::function < void() >;
private:
	BaseWindow& parent;
	OnClick onClick{ [](){} };
	UINT_PTR ptSubclass;

	static LRESULT CALLBACK BtnProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
public:
	//parent reference rather than pointer, because null is invalid
	Button(BaseWindow& parent, tstring label, RECT rect, OnClick handler, HFONT font = NULL);
	~Button();

	void SetOnClick(OnClick handler);
};

#endif