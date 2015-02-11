/* 
 * File:   Flx_ContextMenu.h
 * Author: max
 *
 * Created on 10. Dezember 2013, 17:24
 */

#ifndef FLX_CONTEXTMENU_H
#define	FLX_CONTEXTMENU_H

#ifndef NULL
#define NULL 0
#endif

#include <FL/Fl_Menu_Button.H>

#include <flx/flx_specialsignals.h>

#include "flx_signalparms.h"

class Fl_Pixmap;

namespace flx {
    
class Flx_MenuItemFactory;

class Flx_ContextMenu : public Fl_Menu_Button {
public:
    ShortcutSignal signalShortcut; 
public:
    Flx_ContextMenu();
    virtual ~Flx_ContextMenu();
    void setActive(const char* label, bool active);
    void addMenuItem( const char* pLabel, 
					  int shortcut, 
					  int ident,  
					  int pos = -1,
					  const char **ppXpm=NULL,
					  void *pUserdata=NULL, 
					  int flags=0 ); 
    void replaceLabel( const char *pOldLabel, const char *pNewLabel );
    void *getUserData( const Fl_Menu_Item * ) const;
    MenuItemAction &getAction( const Fl_Menu_Item * ) const;
    /** inserts a menu item at the given position */
//    void insertMenuItem(int pos,
//            const char *pLabel,
//            Fl_Pixmap *pIcon = NULL,
//            Fl_Pixmap *pDeIcon = NULL,
//            int shortcut = 0,
//            int ident = -1,
//            void *pUserdata = NULL,
//            int flags = 0);
//    /** adds a menu item at the end of the existing
//        menu items but before "number of table entries" */
//    void addMenuItem(                
//            const char *pLabel,
//            Fl_Pixmap *pIcon = NULL,
//            Fl_Pixmap *pDeIcon = NULL,
//            int shortcut = 0,
//            int ident = -1,
//            void *pUserdata = NULL,
//            int flags = 0 );
private:
    Flx_MenuItemFactory *_pItemFactory;
};
    
} //flx

#endif	/* FLX_CONTEXTMENU_H */

