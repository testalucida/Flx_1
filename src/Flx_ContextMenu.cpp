#include <flx/Flx_ContextMenu.h>
#include <flx/flx_boxtypes.h>
#include <flx/Flx_MenuItemFactory.h>

#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>

#include "flx/flx_signalparms.h"

namespace flx {

Flx_ContextMenu::Flx_ContextMenu() 
	: Fl_Menu_Button(0,0,0,0) 
{
    _pItemFactory = new Flx_MenuItemFactory( *this );
    
    box( FL_FLAT_BOX );
	down_box( FL_FLAT_BOX );
	labelfont( fl_font() );
	labelsize( fl_size() );
	textfont( fl_font() );
	textsize( fl_size() );
    color( fl_rgb_color( 224, 224, 224 ) );
    selection_color( FL_YELLOW ); //only effects a bright selection color
    //selection_color( fl_rgb_color( 181, 181, 181 ) );
}

Flx_ContextMenu::~Flx_ContextMenu() {
    delete _pItemFactory;
}


void Flx_ContextMenu::setActive( const char* label, bool active ) {
    int idx = find_index( label );
    if( idx > -1 ) {
        int flags = mode( idx );
        if( active ) {
                mode( idx, ( flags &= ~FL_MENU_INACTIVE ) );
        } else {
                mode( idx, ( flags |= FL_MENU_INACTIVE ) );
        }
    } else {
        throw( "Flx_ContextMenu::setActive(): Item not found" );
    }
}

void Flx_ContextMenu::addMenuItem( const char* pLabel, 
                                    int shortcut, 
                                    int ident,  
                                    int pos,
                                    const char **ppXpm,
                                    void *pUserdata, 
                                    int flags )
{
    _pItemFactory->addMenuItem( pLabel, ident, &signalShortcut, shortcut, ppXpm, 
                                pUserdata, flags, -1 );
}

void Flx_ContextMenu::replaceLabel( const char *pOldLabel, 
                                    const char *pNewLabel ) 
{
    for( int i = 0, imax = _pItemFactory->getMenuItemCount(); i < imax; i++ ) {
        Fl_Menu_Item &item = _pItemFactory->getMenuItem( i );
        const char *pLabel = item.label();
        if( pLabel && !strcmp( pOldLabel, pLabel ) ) {
            item.label( pNewLabel );
            return;
        }
    }
}

void *Flx_ContextMenu::getUserData( const Fl_Menu_Item *pItem ) const {
    MenuItemAction *pAction = (MenuItemAction*)pItem->user_data();
    return pAction->pUserdata;
}

MenuItemAction &Flx_ContextMenu::getAction( const Fl_Menu_Item *pItem ) const {
    return *((MenuItemAction*)pItem->user_data());
}

//void Flx_ContextMenu::insertMenuItem( int pos, 
//									  const char *pLabel, 
//									  Fl_Pixmap *pIcon,
//									  Fl_Pixmap *pDeIcon,
//									  int shortcut, 								     
//									  int ident, 
//									  void *pUserdata, 
//									  int flags ) 
//{
//	//const Fl_Menu_Item *pItem = find_item( pLabel );
//	insert( pos, pLabel, shortcut, NULL, (void*)ident, flags );
//}
//
//void Flx_ContextMenu::addMenuItem( const char *pLabel, 
//                                   Fl_Pixmap *pIcon,
//                                   Fl_Pixmap *pDeIcon,
//                                   int shortcut, 								     
//                                   int ident, 
//                                   void *pUserdata, 
//                                   int flags ) 
//{
//    int pos = size() - 1;
//	insertMenuItem( pos, pLabel, pIcon, pDeIcon, shortcut, ident, pUserdata, flags );
//}

} //flx