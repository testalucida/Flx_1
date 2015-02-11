#ifndef _FLX_MENUITEM_
#define _FLX_MENUITEM_

#include <my/Signal.hpp>
#include <flx/flx_signalparms.h>

#include <FL/Fl_Group.H>
#include <FL/Fl_Pack.H>

class Fl_Pixmap;

namespace flx {

class Flx_SignalBox;

class Flx_MenuItem : public Fl_Pack {
public:
	my::Signal < Flx_MenuItem, ResizeParm > signalResized;
	my::Signal < Flx_MenuItem, ActionParm > signalSelected;
public:
	/** creates a Flx_MenuItem.
		Use shortcut like this: FL_CTRL+'n' */
	Flx_MenuItem( int x, int y, const char *pLabel=0, int shortcut=0, const char **pXpm=0 );
	virtual ~Flx_MenuItem(void);
	static int getDefaultHeight() { return H; }
	void setLabel( const char *pLabel );
	void size( int w, int h );
	//void setLabelWidth( int w );
	void printSizes();
protected:
	int handle( int );
private:
	int getLabelWidth( const char *pLabel ) const;
	const char *createShortcut( int shortcut );
private:
	static const int H = 25; //itemheight
	static const int IMAGEBOX_W = 25;
	static const int SHORTCUTBOX_W = 40;
	int _h;
	int _shortcut;
	char _shortcutLabel[16];
	bool _shortcutComing;
	Fl_Pixmap *_pImg;
	Flx_SignalBox *_pImageBox;
	Flx_SignalBox *_pLabelBox;
	Flx_SignalBox *_pShortcutBox;
};

} //ns flx

#endif

