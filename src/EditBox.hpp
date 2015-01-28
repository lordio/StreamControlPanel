#ifndef EDITBOX
#define EDITBOX

#include "BaseWindow.hpp"

class EditBox : public BaseWindow
{
public:
	EditBox(BaseWindow & parent, RECT rect, bool multiline = false, HFONT font = NULL);
	~EditBox();

	//an uncommon set-only property
	void SetCueBannerText(tstring text, bool showWhenFocused = false);
};

#endif