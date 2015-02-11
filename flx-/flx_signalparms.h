#ifndef _FLX_SIGNALPARMS_H_
#define _FLX_SIGNALPARMS_H_

#include <flx/flx_specialsignals.h>

#include <my/CharBuffer.h>

struct Fl_Menu_Item;
class Fl_Tree_Item;

namespace flx {

//mouse click
struct ClickParm {
	int mouseBtn;
	bool doubleClicked;
	int x;
	int y;
};

//widget was resized
struct ResizeParm {
	int x;
	int y;
	int w;
	int h;
};

// a button, menu item or tool item was selected
struct ActionParm {
    
};

struct Action {
	int ident;
	void *pUserdata;
};

//class Flx_MenuBar;
struct MenuItemAction : public Action {
    my::CharBuffer label;
    MenuItemSelectedSignal *pSignalSelected;
};

class Flx_ToolButton;
struct ToolAction : public Action {
	Flx_ToolButton *pSelectedButton;
};

//mouse was moved
struct MouseParm {
	int x;
	int y;
};

struct TreeItemAction {
    Fl_Tree_Item *pItem;
    int reason; //one of Fl_Tree_Reason (see Fl_Tree.H)
};

} //ns  flx

#endif