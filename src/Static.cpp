#include "Static.hpp"

Static::Static(BaseWindow & parent, tstring text, RECT rect, Static::Justify jst, bool border, HFONT font) :
BaseWindow{ WC_STATIC, text, (jst == Static::Justify::Left ? SS_LEFT : (jst == Static::Justify::Center ? SS_CENTER : SS_RIGHT)) | WS_CHILD, (border ? WS_EX_STATICEDGE : 0), rect, &parent, font }
{
}

Static::~Static()
{
}