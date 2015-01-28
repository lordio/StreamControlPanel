#ifndef SPINNER
#define SPINNER

#include "BaseWindow.hpp"

class Spinner : public BaseWindow
{
public:
	Spinner(BaseWindow & parent, BaseWindow * buddy, POINT loc, int min, int max);
	~Spinner();

	void SetBuddy(BaseWindow * buddy);
	void SetRange(int min, int max);
	void SetPosition(int pos);
};

#endif