#ifndef _FLX_MENUBAR_
#define _FLX_MENUBAR_

#include <flx/flx_signalparms.h>
#include <flx/flx_specialsignals.h>
#include <flx/Flx_MenuItemFactory.h>
#include <my/Signal.hpp>

#include <FL/Fl_Menu_Bar.H>
#include <FL/Enumerations.H>

#include <vector>

struct Fl_Menu_Item;
class Fl_Pixmap;

namespace flx {

//#define FLX_MENUITEM_LABEL FL_FREE_LABELTYPE
//
//	struct Icon {
//		const char *pMenuPath;
//		const Fl_Pixmap *pIcon;
//	};


class Flx_MenuBar : public Fl_Menu_Bar {
public:
	//my::Signal < Flx_MenuBar, const MenuItemAction > signalMenuItemSelected;
    MenuItemSelectedSignal signalMenuItemSelected;
public:
	/** C'tor 
	    end() will be called after having added 
	    Flx_MenuBar and Flx_ToolBar. 
	*/
	Flx_MenuBar( int x, int y, int w, int h );

	/** Dtor */
	virtual ~Flx_MenuBar(void);

	/** Adds a menu item.
		@param menupath: specify menupath like so: "File/New..." 
		@param shortcut: FL_CTRL+'x'
		@param ident: an arbitrary unique integer to identify your menu item
		@pUserdata: fltk user_data
		@flags: e.g. FL_MENU_VALUE, FL_SUBMENU, ... (see Fl_Menu_.H)
	*/
    Fl_Menu_Item &addMenuItem( const char* menupath, 
			          int shortcut, 
			          int ident,  
					  const char **ppXpm=0,
			          void *pUserdata=0, 
			          int flags=0 ); 

	int getItemIndexByName( const char *pName );
	Fl_Menu_Item *getItemByName( const char *pName );
	Fl_Menu_Item *insertMenuItem( int pos, 
						const char *pLabel, 
						int shortcut, 
						int ident,  
						Fl_Pixmap *pIcon,
						void *pUserdata=0, 
						int flags=0  );

	void clearItemsBelow( const char *pMenuName );
	/** shadowing Fl_Menu_Bar::remove() due to actions and icons. */
	void remove( int index );
	void dumpMenuBar();
	/** static methods for own label drawing */
//	static void menuItemDraw( const Fl_Label *label, int x, int y, int w, int h, Fl_Align align );
//	static void menuItemMeasure( const Fl_Label *label, int &w, int &h );
protected:
	void draw();
private:
//	Fl_Menu_Item *createActionAndIcon( const char *pMenuPath, int ident, Fl_Pixmap *pIcon, void *pUserdata );
//	static void staticMenuItemCallback( Fl_Widget *, void * );
//	void menuItemCallback( const MenuItemAction * );
	//MenuItemAction getAction( int ident );
	void removeIcon( int ident );

private:
    Flx_MenuItemFactory *_pItemFactory;
	//std::vector < MenuItemAction > _actions; //one action each Fl_Menu_Item 
	//std::vector < MenuItemAction* > _actions; //one action each Fl_Menu_Item 
	//std::vector < MenuItemAction >::iterator _itrActions;
//	std::vector < MenuItemAction* >::iterator _itrActions;
//	std::vector < Icon > _icons; //store icons for items separately
//	std::vector < Icon >::iterator _itrIcons;
//	static std::vector< Icon > *_pIcons;
};

}

#endif