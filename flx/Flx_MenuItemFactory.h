#ifndef _FLX_MENUITEMFACTORY_
#define _FLX_MENUITEMFACTORY_

#include <flx/flx_specialsignals.h>

#include <FL/Enumerations.H>

#include <vector>

class Fl_Widget;
class Fl_Menu_;
struct Fl_Menu_Item;
class Fl_Pixmap;
struct Fl_Label;

namespace flx {

struct MenuItemAction;

struct Icon {
    const Fl_Menu_ *pMenu;
	const char *pMenuPath;
	const Fl_Pixmap *pIcon;
};

/** store menu icons in a static vector - so *all* icons of the 
 *  application will be stored here. */
static std::vector < Icon > _icons; //store icons for items separately

class Flx_MenuItemFactory {
public:
    Flx_MenuItemFactory( Fl_Menu_ & );
    ~Flx_MenuItemFactory();
    
	/** Creates and adds a Fl_Menu_Item at a given position 
	    in any kind of a Fl_Menu_ derived menu strip.
	    If pos is omitted or pos == -1 the new Fl_Menu_Item will be appended.
	*/
	Fl_Menu_Item &addMenuItem( 
						const char* pMenupath, 
						int ident = 0,  //identify your menu item
						MenuItemSelectedSignal *pCallbackSignal = 0,
						int shortcut = 0, 						
						const char **ppXpm = 0,
						void *pUserdata = 0, 
						int flags = 0,
						int pos = -1 );
 
    Fl_Menu_Item &getMenuItem( int n );
    
    int getMenuItemCount();
    
    /** removing a menu item by respecting actions and icons. */
	void remove( int index );
    static void menuItemDraw( const Fl_Label *label, int x, int y, int w, int h, Fl_Align align );
	static void menuItemMeasure( const Fl_Label *label, int &w, int &h );
private:
       MenuItemAction *createActionAndIcon( 
                        const char* pMenupath, 
						int ident = 0,  //identify your menu item
						MenuItemSelectedSignal *pCallbackSignal = 0,
						int shortcut = 0, 						
						const char **ppXpm = 0,
						void *pUserdata = 0 );
    bool existsIcon( const char *pMenuPath );
    void removeIcon( const char *pMenuPath );
	static void staticMenuItemCallback( Fl_Widget *, void * );
	void deleteFactory();
    void deleteIcons();
private:
    Fl_Menu_ &_menu;
//	static Flx_MenuItemFactory *_pThis;
    std::vector< MenuItemAction *> _actions;
    std::vector < MenuItemAction* >::iterator _itrActions;
//    static std::vector < Icon > _icons; //store icons for items separately
	std::vector < Icon >::iterator _itrIcons;
    //static std::vector< Icon > *_pIcons;
};

} //flx

#endif