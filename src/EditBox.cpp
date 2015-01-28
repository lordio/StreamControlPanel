#include "EditBox.hpp"
#include <windowsx.h>

EditBox::EditBox(BaseWindow & parent, RECT rect, bool multiline, HFONT font) :
BaseWindow{ WC_EDIT, TEXT(""), (multiline ? ES_MULTILINE | ES_WANTRETURN : 0) | WS_CHILD | WS_TABSTOP, WS_EX_CLIENTEDGE, rect, &parent, font }
{
}
EditBox::~EditBox()
{
}

void EditBox::SetCueBannerText(tstring text, bool showWhenFocused)
{
	Edit_SetCueBannerTextFocused(wnd, text.c_str(), showWhenFocused);
}