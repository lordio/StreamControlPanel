#include "Spinner.hpp"

Spinner::Spinner(BaseWindow & parent, BaseWindow * buddy, POINT loc, int min, int max) :
BaseWindow{ UPDOWN_CLASS, TEXT(""), UDS_SETBUDDYINT | WS_CHILD, NULL, RECT{ loc.x, loc.y, loc.x + 17, loc.y + 20 }, &parent }
{
	SetBuddy(buddy);
	SetRange(min, max);
	SetPosition(0);
}
Spinner::~Spinner()
{
}

void Spinner::SetBuddy(BaseWindow * buddy)
{
	SendMessage(wnd, UDM_SETBUDDY, (buddy ? reinterpret_cast<WPARAM>(buddy->GetWindow()) : 0), 0);
}
void Spinner::SetRange(int min, int max)
{
	SendMessage(wnd, UDM_SETRANGE32, min, max);
}
void Spinner::SetPosition(int pos)
{
	SendMessage(wnd, UDM_SETPOS32, 0, pos);
}