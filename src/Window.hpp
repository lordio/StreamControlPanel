#include "BaseWindow.hpp"

class Window : public BaseWindow
{
	static WNDCLASSEX cls;
	class RegCls final
	{
	public:
		RegCls();
		~RegCls();
	};
	friend class RegCls;
	static RegCls _c;
public:
	Window(tstring title, RECT rect, bool hasMenu = false, bool resizable = true, HFONT font = NULL);
	virtual ~Window();
};