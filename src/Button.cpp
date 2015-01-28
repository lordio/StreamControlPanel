#include "Button.hpp"
#include <windowsx.h>

Button::Button(BaseWindow& parent, tstring label, RECT rect, OnClick handler, HFONT font):
BaseWindow{WC_BUTTON, label, WS_CHILD | WS_TABSTOP, 0, rect, &parent, font},
parent{ parent },
onClick{ handler }
{
	ptSubclass = parent.SetSubclass(BtnProc, this);
}
Button::~Button()
{
	parent.RemoveSubclass(ptSubclass, BtnProc);
}

void Button::SetOnClick(Button::OnClick handler)
{
	onClick = handler;
}

LRESULT CALLBACK Button::BtnProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uSubclassID, DWORD_PTR dwRefData)
{
	Button * self{ reinterpret_cast<Button*>(dwRefData) };

	switch (msg)
	{
		HANDLE_MSG(wnd, WM_COMMAND, [=](HWND wnd, int ctlid, HWND ctl, UINT code) -> LRESULT {
			if (ctl == self->GetWindow() && code == BN_CLICKED)
			{
				self->onClick();
				return 0;
			}
			else return DefSubclassProc(wnd, msg, wParam, lParam);
		});
	}
	return DefSubclassProc(wnd, msg, wParam, lParam);
}