#include "Window.hpp"
#include "GroupBox.hpp"
#include "Button.hpp"
#include "EditBox.hpp"
#include "Dropdown.hpp"
#include "Spinner.hpp"
#include "Static.hpp"
#include <windowsx.h>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "ComCtl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' \
processorArchitecture='*' \
publicKeyToken='6595b64144ccf1df' \
language='*'\"")
#pragma comment(linker, "/ENTRY:mainCRTStartup /subsystem:windows")

#ifdef UNICODE
using tifstream = std::wifstream;
using tofstream = std::wofstream;
#else
using tifstream = std::ifstream;
using tofstream = std::ofstream;
#endif

class ControlPanelWindow : public Window
{
	HFONT font;

	using NameList = std::vector < tstring > ;
	NameList players;
	NameList commentators;

	class TitleGroup : public GroupBox
	{
		EditBox type;
		Dropdown leftPlayer;
		Static leftScore;
		Spinner leftSpinner;
		Dropdown rightPlayer;
		Static rightScore;
		Spinner rightSpinner;
		const Static VS;
		const Static CHALLONGE;
		EditBox link;

		NameList & players;

		static LRESULT CALLBACK spinnerUpdate(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uSubclassId, DWORD_PTR dwRefData)
		{
			TitleGroup * self{ reinterpret_cast<TitleGroup*>(dwRefData) };

			switch (msg)
			{
				HANDLE_MSG(wnd, WM_NOTIFY, ([=](HWND wnd, int idFrom, NMHDR * hdr) -> LRESULT {
					if ((hdr->hwndFrom == self->leftSpinner.GetWindow() || hdr->hwndFrom == self->rightSpinner.GetWindow()) && hdr->code == UDN_DELTAPOS)
					{
						LPNMUPDOWN lpnmud{ reinterpret_cast<LPNMUPDOWN>(hdr) };
						int total{ lpnmud->iPos + lpnmud->iDelta };
						if (total < 0 || total > 9) return 1;

						std::ofstream out{ (hdr->hwndFrom == self->leftSpinner.GetWindow() ? "leftscore.txt" : "rightscore.txt"), std::ios::trunc };
						out << lpnmud->iPos + lpnmud->iDelta;

						return 0;
					}
					else return DefSubclassProc(wnd, msg, wParam, lParam);
				}));
			}
			return DefSubclassProc(wnd, msg, wParam, lParam);
		}

		bool setPosIfPlayerFound(Dropdown & dd, tstring name)
		{
			auto loc = std::find(players.begin(), players.end(), name);
			if (loc != players.end())
			{
				dd.SetCurrentSelection(loc - players.begin());
				return true;
			}
			else return false;
		}
		void commit(BaseWindow & bw, tstring file)
		{
			tofstream bf{ file.c_str() };
			bf << bw.GetText();
		}
		void commit(Dropdown & dd, tstring file)
		{
			UINT sel{ dd.GetCurrentSelection() };
			tstring name{};

			if (sel == CB_ERR)
			{
				name = dd.GetText();

				if (!setPosIfPlayerFound(dd, name))
				{
					Dropdown & other{ (dd == leftPlayer ? rightPlayer : leftPlayer) };
					tstring otherText{ other.GetText() };

					tofstream plFile{ "players.txt", std::ios::app };
					plFile << std::endl << name;
					plFile.close();

					ReloadPlayers();

					setPosIfPlayerFound(dd, name); //should logically always return true.
					if (!setPosIfPlayerFound(other, otherText)) other.SetText(otherText);
				}
			}
			else name = players[sel];

			tofstream strm{ file.c_str(), std::ios::trunc };
			strm << name;
		}
	public:
		TitleGroup(ControlPanelWindow& parent, NameList & p, HFONT font) :
			GroupBox{ parent, tstring{ TEXT("Titles") }, RECT{ 10, 10, 440, 90 }, font },
			type{ *this, RECT{ 10, 20, 200, 40 }, false, font },
			leftPlayer{ *this, RECT{ 56, 50, 200, 70 }, font },
			leftScore{ *this, TEXT(""), RECT{ 27, 50, 47, 70 }, Static::Justify::Center, true, font },
			leftSpinner{ *this, &leftScore, POINT{ 10, 50 }, 0, 9 },
			rightPlayer{ *this, RECT{ 230, 50, 374, 70 }, font },
			rightScore{ *this, TEXT(""), RECT{ 383, 50, 403, 70 }, Static::Justify::Center, true, font },
			rightSpinner{ *this, &rightScore, POINT{ 403, 50 }, 0, 9 },
			VS{ *this, TEXT("VS"), RECT{ 200, 53, 230, 73 }, Static::Justify::Center, false, font },
			CHALLONGE{ *this, TEXT("http://challonge.com/"), RECT{ 230, 23, 337, 43 }, Static::Justify::Left, false, font },
			link{ *this, RECT{ 335, 20, 420, 40 }, false, font },
			players{ p }
		{
			SetSubclass(spinnerUpdate, this);
			ReloadPlayers();
		}

		void CommitTitles()
		{
			commit(leftPlayer, TEXT("leftplayer.txt"));
			commit(rightPlayer, TEXT("rightplayer.txt"));
			commit(type, TEXT("info.txt"));

			//have to add prefix
			tofstream ln{ TEXT("bracketlink.txt"), std::ios::trunc };
			ln << "http://challonge.com/" << link.GetText();

			//the static scores are updated when the spinners are manipulated
		}
		void ResetScores()
		{
			leftSpinner.SetPosition(0);
			rightSpinner.SetPosition(0);

			tofstream left{ "leftscore.txt", std::ios::trunc };
			left << 0;

			tofstream right{ "rightscore.txt", std::ios::trunc };
			right << 0;
		}
		void ReloadPlayers()
		{
			leftPlayer.ResetContent();
			rightPlayer.ResetContent();
			players.clear();

			tifstream plFile{ "players.txt" };
			if (!plFile) return;

			while (!plFile.eof())
			{
				tstring player;
				std::getline(plFile, player);

				if (!player.empty() &&
					std::find(players.begin(), players.end(), player) == players.end()) players.push_back(player);
			}
			std::sort(players.begin(), players.end(), [](tstring lhs, tstring rhs) -> bool {
				char const * l{ lhs.c_str() };
				char const * r{ rhs.c_str() };
				for (; *l != 0 && *r != 0; ++l, ++r)
				{
					if (tolower(*l) != tolower(*r)) return tolower(*l) < tolower(*r);
					else if (*l != *r) return *l < *r;
				}
				return false;
			});

			for (auto name : players)
			{
				leftPlayer.AddString(name);
				rightPlayer.AddString(name);
			}
		}
	} titleGroup;
	class CommentatorGroup : public GroupBox
	{
		Dropdown left;
		Dropdown right;
		const Static L;
		const Static R;

		NameList & cmt;

		bool setPosIfCommentatorFound(Dropdown & dd, tstring name)
		{
			auto loc = std::find(cmt.begin(), cmt.end(), name);
			if (loc != cmt.end())
			{
				dd.SetCurrentSelection(loc - cmt.begin());
				return true;
			}
			else return false;
		}
		void commit(Dropdown & dd, tstring file)
		{
			UINT sel{ dd.GetCurrentSelection() };
			tstring name{};

			if (sel == CB_ERR)
			{
				name = dd.GetText();

				if (!setPosIfCommentatorFound(dd, name))
				{
					Dropdown & other{ dd == left ? right : left };
					tstring otherText{ other.GetText() };

					tofstream cmFile{ "commentators.txt", std::ios::app };
					cmFile << std::endl << name;
					cmFile.close();

					ReloadCommentators();

					setPosIfCommentatorFound(dd, name); //should logically always return true
					if (!setPosIfCommentatorFound(other, otherText)) other.SetText(otherText);
				}
			}
			else name = cmt[sel];

			tofstream strm{ file.c_str(), std::ios::trunc };
			strm << name;
		}
	public:
		CommentatorGroup(ControlPanelWindow& parent, NameList & c, HFONT font) :
			GroupBox{ parent, TEXT("Commentary"), RECT{ 10, 100, 220, 190 }, font },
			left{ *this, RECT{ 20, 20, 200, 40 }, font },
			right{ *this, RECT{ 20, 55, 200, 75 }, font },
			L{ *this, TEXT("L"), RECT{ 11, 23, 21, 43 }, Static::Justify::Left, false, font },
			R{ *this, TEXT("R"), RECT{ 10, 58, 20, 78 }, Static::Justify::Left, false, font },
			cmt{ c }
		{
			ReloadCommentators();
		}

		void CommitCommentators()
		{
			commit(left, "leftcommentator.txt");
			commit(right, "rightcommentator.txt");
		}
		void ReloadCommentators()
		{
			left.ResetContent();
			right.ResetContent();
			cmt.clear();

			tifstream cmFile{ "commentators.txt" };
			if (!cmFile) return;

			while (!cmFile.eof())
			{
				tstring player;
				std::getline(cmFile, player);

				if (!player.empty() &&
					std::find(cmt.begin(), cmt.end(), player) == cmt.end()) cmt.push_back(player);
			}
			std::sort(cmt.begin(), cmt.end(), [](tstring lhs, tstring rhs) -> bool {
				char const * l{ lhs.c_str() };
				char const * r{ rhs.c_str() };
				for (; *l != 0 && *r != 0; ++l, ++r)
				{
					if (tolower(*l) != tolower(*r)) return tolower(*l) < tolower(*r);
					else if (*l != *r) return *l < *r;
				}
				return false;
			});

			for (auto name : cmt)
			{
				left.AddString(name);
				right.AddString(name);
			}
		}
	} commentatorGroup;
	class ControlGroup : public GroupBox
	{
		Button applyTitles;
		Button resetScores;
	public:
		ControlGroup(ControlPanelWindow& parent, HFONT font) :
			GroupBox{ parent, TEXT("Update Controls"), RECT{ 230, 100, 440, 190 }, font },
			applyTitles{ *this, TEXT("Apply Titles"), RECT{ 10, 20, 100, 80 }, [&parent](){ parent.CommitTitles(); parent.CommitCommentators(); }, font },
			resetScores{ *this, TEXT("Reset Scores"), RECT{ 110, 20, 200, 80 }, [&parent](){ parent.ResetScores(); }, font }
		{
		}
	} controlGroup;

	HMENU menu;
	HMENU mFile;
	HMENU mHelp;

	static LRESULT CALLBACK windowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uSubclassId, DWORD_PTR dwRefData)
	{
		ControlPanelWindow * self{ reinterpret_cast<ControlPanelWindow*>(dwRefData) };
		switch (msg)
		{
			HANDLE_MSG(wnd, WM_CLOSE, [](HWND){ PostQuitMessage(0); return 0; });
			HANDLE_MSG(wnd, WM_COMMAND, [=](HWND wnd, int ctlid, HWND ctl, UINT code) -> LRESULT {
				if (ctlid == 0) //Reload Lists
				{
					self->titleGroup.ReloadPlayers();
					self->commentatorGroup.ReloadCommentators();
					return 0;
				}
				else if (ctlid == 1) // Exit
				{
					PostQuitMessage(0);
					return 0;
				}
				else if (ctlid == 2) // About
				{
					//Do something nicer than this eventually
					MessageBox(wnd, TEXT("Smashing Grounds Control Panel v2.0\n"
						"Coded by @evildrkirby\n"
						"Produced by @Ovis_Cantus"), TEXT("About"), MB_OK);
					return 0;
				}
				else if (ctlid == 3) // How To
				{
					//Do something nicer than this eventually
					MessageBox(wnd, TEXT("- Copy the signup list from Challonge into players.txt\n"
						"- Click File>Reload Lists to repopulate the player dropdown\n"
						"- Scores will update automatically whenever they are adjusted\n"
						"- Always click Apply Titles after making any other changes!"), TEXT("How To"), MB_OK);
					return 0;
				}
				else return DefSubclassProc(wnd, msg, wParam, lParam);
			});
		}
		return DefSubclassProc(wnd, msg, wParam, lParam);
	}
public:
	ControlPanelWindow() :
		Window{ "Smashing Grounds Control Panel", RECT{ 100, 100, 550, 300 }, true, false },
		font{ CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, TEXT("Microsoft Sans Serif")) },
		titleGroup{ *this, players, font },
		commentatorGroup{ *this, commentators, font },
		controlGroup{ *this, font }
	{
		menu = CreateMenu();
		mFile = CreatePopupMenu();
		AppendMenu(mFile, MF_ENABLED | MF_STRING, 0, TEXT("&Reload Lists"));
		AppendMenu(mFile, MF_SEPARATOR, 10, TEXT(""));
		AppendMenu(mFile, MF_ENABLED | MF_STRING, 1, TEXT("E&xit"));
		AppendMenu(menu, MF_POPUP | MF_ENABLED | MF_STRING, reinterpret_cast<UINT_PTR>(mFile), TEXT("&File"));

		mHelp = CreatePopupMenu();
		AppendMenu(mHelp, MF_ENABLED | MF_STRING, 3, TEXT("&How To..."));
		AppendMenu(mHelp, MF_ENABLED | MF_STRING, 2, TEXT("&About..."));
		AppendMenu(menu, MF_POPUP | MF_ENABLED | MF_STRING, reinterpret_cast<UINT_PTR>(mHelp), TEXT("&Help"));
		SetMenu(wnd, menu);

		SetSubclass(windowProc, this);
	}
	~ControlPanelWindow()
	{
		RemoveSubclass(0, windowProc);
	}

	void CommitTitles()
	{
		titleGroup.CommitTitles();
	}
	void CommitCommentators()
	{
		commentatorGroup.CommitCommentators();
	}
	void ResetScores()
	{
		titleGroup.ResetScores();
	}
};

int main()
{
	InitCommonControls();
	ControlPanelWindow win;
	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}