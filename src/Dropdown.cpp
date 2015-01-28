#include "Dropdown.hpp"
#include <windowsx.h>

Dropdown::Dropdown(BaseWindow & parent, RECT rect, HFONT font) :
BaseWindow{ WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | WS_VSCROLL | WS_CHILD | WS_TABSTOP, WS_EX_CLIENTEDGE, rect, &parent, font }
{
	SetExtendedUI(true);
}
Dropdown::~Dropdown()
{
}

void Dropdown::SetExtendedUI(bool set)
{
	ComboBox_SetExtendedUI(wnd, (set ? TRUE : FALSE));
}
void Dropdown::AddString(tstring str)
{
	ComboBox_AddString(wnd, str.c_str());
}
void Dropdown::ResetContent()
{
	ComboBox_ResetContent(wnd);
}
UINT Dropdown::GetCurrentSelection()
{
	return ComboBox_GetCurSel(wnd);
}
void Dropdown::SetCurrentSelection(UINT pos)
{
	ComboBox_SetCurSel(wnd, pos);
}

bool operator==(Dropdown const& l, Dropdown const& r)
{
	return l.GetWindow() == r.GetWindow();
}