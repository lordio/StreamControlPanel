#ifndef GROUPBOX
#define GROUPBOX

#include "BaseWindow.hpp"

class GroupBox : public BaseWindow
{
public:
	GroupBox(BaseWindow & parent, tstring label, RECT rect, HFONT font = NULL);
	virtual ~GroupBox();
};

#endif