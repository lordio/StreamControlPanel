#ifndef DROPDOWN
#define DROPDOWN

#include "BaseWindow.hpp"

class Dropdown : public BaseWindow
{
public:
	Dropdown(BaseWindow & parent, RECT rect, HFONT font = NULL);
	~Dropdown();

	void SetExtendedUI(bool set);
	void AddString(tstring str);
	void ResetContent();
	UINT GetCurrentSelection();
	void SetCurrentSelection(UINT pos);
};

bool operator==(Dropdown const& l, Dropdown const& r);

#endif