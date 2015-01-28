#ifndef STATIC
#define STATIC

#include "BaseWindow.hpp"

class Static : public BaseWindow
{
public:
	enum class Justify
	{
		Left,
		Center,
		Right
	};

	Static(BaseWindow & parent, tstring text, RECT rect, Justify jst = Justify::Left, bool border = false, HFONT font = NULL);
	~Static();
};

#endif