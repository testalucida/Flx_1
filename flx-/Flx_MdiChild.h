#ifndef _FLX_MDICHILD_H_
#define _FLX_MDICHILD_H_

#include <FL/Fl_Group.H>

#include <flx/Flx_Group.h>

#include <my/Signal.hpp>
#include <my/CharBuffer.h>

class Fl_Box;
class Fl_Scroll;

namespace flx {

class Flx_Button;
struct ActionParm;

/** MousePosition where cursor changes */
enum MousePosition {
	POS_ANY = 0,
	POS_N = 1,
	POS_E = 2,
	POS_S = 4,
	POS_W = 8,
	POS_NE = 3,
	POS_SE = 6,
	POS_SW = 12,
	POS_NW = 9
};

/** cursor at position */
static Fl_Cursor cursors[] = {
	FL_CURSOR_DEFAULT, //0
	FL_CURSOR_NS, //1
	FL_CURSOR_WE, //2
	FL_CURSOR_NE, //3
	FL_CURSOR_NS, //4
	FL_CURSOR_DEFAULT, //5 -- Dummy1
	FL_CURSOR_NW, //6
	FL_CURSOR_DEFAULT, //7 -- Dummy1
	FL_CURSOR_WE, //8
	FL_CURSOR_NW, //9
	FL_CURSOR_DEFAULT, //10 -- Dummy1
	FL_CURSOR_DEFAULT, //11 -- Dummy2
	FL_CURSOR_NE //12
};

enum ControlBoxActions {
	CONTROLBOXACTION_CLOSE,
	CONTROLBOXACTION_MINI,
	CONTROLBOXACTION_MAXI
};

struct ControlBoxAction {
	ControlBoxActions action;
};

////////////////////////////////////////////////////////////////
////////////// Flx_TitleBar //////////////////////////////////
////////////////////////////////////////////////////////////////
/**
	Title for a Flx_MidChild.
	Consists of an Fl_Image on the left side,
	a centered title Fl_Box,
	and a right sided Fl_ControlBox.
*/
class Flx_TitleBar : public Flx_Group {
public:
	my::Signal< Flx_TitleBar, ControlBoxAction > signalControl;
public:
	Flx_TitleBar( int x, int y, int w, const char *pTitle = NULL );
	void onControlButton( Flx_Button &, ActionParm & );
	bool isMouseOverControlBox() const;
    bool isMouseOverTitle() const;
protected:
	void draw();
private:
	int _titleSize;
	static const int _titleHeight = 25;
	Fl_Box *_pImageBox;
    Flx_Group *_pTitleGroup;
	Flx_Button *_pCloseBtn;
    Flx_Button *_pMinBtn;
    Flx_Button *_pMaxBtn;
	my::CharBuffer _title;
};

//////////////////////////////////////////////////////////////
////////// Flx_MdiChild ////////////////////////////////////
//////////////////////////////////////////////////////////////
class Flx_MdiChild : public Fl_Group {
public:
	my::Signal< Flx_MdiChild, ControlBoxAction > signalControlBoxAction;
public:
	Flx_MdiChild( int x, int y, int w, int h, const char *pLable=NULL );
	~Flx_MdiChild();
	Fl_Group &getClientArea() const;
	Fl_Group &getClientAreaContent() const;
	void onControl( Flx_TitleBar &, ControlBoxAction & );
    /** checks if "this" is on top. If not removes and adds "this"
      to bring it on top. 
     Returns true if removing/adding was performed. */
    bool bringMeOnTop();
protected:
	int handle( int );
	void draw();
private:
	void drag();
	MousePosition checkResizeCursor();
	void resize();
	void resizeHorz( int dx );
	void resizeVert( int dy );
	bool canResizeVert( int dy ) const;
	bool canResizeHorz( int dx ) const;
    bool isMouseOverMeOrOneOfMyChildren() const;
    bool isMouseOverMeOrOneOfMyChildren( const Fl_Widget *pBelowMouse,
                                         const Fl_Group *pCheck ) const;
private:
	MousePosition _mousePos;
	bool _dragging;
	int _x, _y;
	Flx_TitleBar *_pTitleBar;
	Fl_Scroll *_pClientScroll;
	Fl_Group *_pClientArea;
};

} //ns flx

#endif