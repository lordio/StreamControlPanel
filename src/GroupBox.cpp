#include "GroupBox.hpp"

GroupBox::GroupBox(BaseWindow & parent, tstring label, RECT rect, HFONT font) :
	BaseWindow{ WC_BUTTON, label, BS_GROUPBOX | WS_CHILD, NULL, rect, &parent, font }
{
}
GroupBox::~GroupBox()
{
}