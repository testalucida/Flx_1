#include <flx/Flx_MenuBar.h>
#include <flx/flx_boxtypes.h>
#include <flx/Flx_MenuItemFactory.h>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Pixmap.H>

#include <stdio.h>
#include <string.h>

namespace flx {

//std::vector< Icon > *Flx_MenuBar::_pIcons = NULL;
    
//#ifndef FLX_MENUITEM_LABEL
//#define FLX_MENUITEM_LABEL FL_FREE_LABELTYPE
//#endif

Flx_MenuBar::Flx_MenuBar( int x, int y, int w, int h ) 
	: Fl_Menu_Bar( x, y, w, h )
{	
//	Fl::set_labeltype( FLX_MENUITEM_LABEL, menuItemDraw, menuItemMeasure );
	//doesn't work Fl::set_labeltype( _FL_IMAGE_LABEL, menuItemDraw, menuItemMeasure );
    _pItemFactory = new Flx_MenuItemFactory( *this );
	box( FLX_GRADIENT_BOX );
	down_box( FLX_GRADIENT_DOWN_BOX );
	labelfont( fl_font() );
	labelsize( fl_size() );
	textfont( fl_font() );
	textsize( fl_size() );
//	_pIcons = &_icons;
	Fl_Color colr = fl_color_average( fl_darker( fl_color() ), fl_lighter( fl_color() ), 0.5 );
	selection_color( colr );
}

Flx_MenuBar::~Flx_MenuBar(void) {
    delete _pItemFactory;
//	for( _itrActions = _actions.begin(); _itrActions != _actions.end(); _itrActions++ ) {
//		MenuItemAction *pAc = *_itrActions;
//		delete pAc;
//	}
}

//void Flx_MenuBar::staticMenuItemCallback( Fl_Widget *pWi, void *pUserdata ) {
//	Flx_MenuBar *pMb = (Flx_MenuBar*)pWi;
//	//int ident = (int)pUserdata;
//	MenuItemAction *pAction = (MenuItemAction*)pUserdata;
//	pMb->menuItemCallback( pAction );
//}
//
//void Flx_MenuBar::menuItemCallback( const MenuItemAction *micb ) {
//	//MenuItemAction action = getAction( ident );
//	signalMenuItemSelected.send( *this, *micb );
//}

//MenuItemAction Flx_MenuBar::getAction( int ident ) {
//	for( _itrActions = _actions.begin(); _itrActions != _actions.end(); _itrActions++ ) {
//		if( (*_itrActions).ident == ident ) {
//			return *_itrActions;
//		}
//	}
//	throw( "Flx_MenuBar::getAction(): ident not found" );
//}

Fl_Menu_Item &Flx_MenuBar::addMenuItem( const char* menupath, 
							  int shortcut, 
							  int ident,  //identify your menu item
							  const char **ppXpm,
							  void *pUserdata, 
							  int flags ) 
{
    
    return _pItemFactory->addMenuItem( 
            menupath, ident, &signalMenuItemSelected, shortcut,
            ppXpm, pUserdata, flags );
}

//Fl_Menu_Item *Flx_MenuBar::insertMenuItem( int pos, 
//							  const char *pLabel, 
//							  int shortcut, 
//							  int ident,  
//							  Fl_Pixmap *pIcon,
//							  void *pUserdata, 
//							  int flags  ) 
//{
//	Fl_Menu_Bar::insert( pos, pLabel, shortcut, staticMenuItemCallback, NULL, flags );
//	return createActionAndIcon( pLabel, ident, pIcon, pUserdata );
//}
//
//Fl_Menu_Item *Flx_MenuBar::createActionAndIcon( const char *pMenuPath, 
//												int ident, 
//												Fl_Pixmap *pIcon, 
//												void *pUserdata ) 
//{
//	Fl_Menu_Item *pItem = const_cast<Fl_Menu_Item*>( find_item( pMenuPath ) );
//	pItem->labeltype( FLX_MENUITEM_LABEL );
//
//	MenuItemAction *pAction = new MenuItemAction;
//	pAction->ident = ident;
//	pAction->pSelectedItem = pItem;
//	pAction->pUserdata = pUserdata;
//	pItem->user_data( pAction );
//	_actions.push_back( pAction );
//
//
//	if( pIcon ) {
//		//pItem->image( pIcon );
//		//Icon icon = {pMenuPath, pIcon};
//		const char *lbl = strrchr( pMenuPath, '/' );
//		if( lbl ) {
//			Icon icon = {++lbl, pIcon};
//			_icons.push_back( icon );
//		}
//	}
//	return pItem;
//}

void Flx_MenuBar::dumpMenuBar() {
	printf( "=========== DUMPING FLX_MENUBAR =============\n" );
	for( int i = 0, imax = menu()->size(); i < imax; i++ ) {
		Fl_Menu_Item *pItem = (Fl_Menu_Item*)&( menu()[i] ); //->label();
		printf( "MenuItem at pos %d = %s\n", i, pItem->label() ? pItem->label() : "NULL" );
	}
	printf( "=========== DUMP END =============\n" );
}

//void Flx_MenuBar::menuItemDraw( const Fl_Label *label, 
//								int x, int y, int w, int h, 
//								Fl_Align align ) 
//{
//	Fl_Pixmap *pIcon = NULL;
//	std::vector < Icon >::iterator itrIcons;
//	for( itrIcons = _pIcons->begin(); itrIcons != _pIcons->end(); itrIcons++ ) {
//		if( strcmp( itrIcons->pMenuPath, label->value ) == 0 ) {
//			fl_draw_pixmap( itrIcons->pIcon->data(), x, y+2 );
//		}
//	}
//	int baseline = y + (h/2 + fl_size()/2);
//	fl_color( label->color );
//	fl_draw( label->value, x+30, baseline );
//}
//
//void Flx_MenuBar::menuItemMeasure( const Fl_Label *label, int &w, int &h ) {
//	int imageH = 20;
//	int fontsize = fl_size() + 4;
//	h = imageH > fontsize ? imageH : fontsize;
//	int labelW = 0, labelH;
//	fl_measure( label->value, labelW, labelH );
//	w = 20 + 8 + labelW + 4;
//}

int Flx_MenuBar::getItemIndexByName( const char *findname ) {
    char menupath[1024] = "";            // File/Export
    for ( int t=0; t < size(); t++ ) {
        Fl_Menu_Item *m = (Fl_Menu_Item*)&( menu()[t] );
        if ( m->submenu() ) {
            // Submenu?
            if ( menupath[0] ) strcat(menupath, "/");
            strcat(menupath, m->label());
            if ( strcmp(menupath, findname) == 0 ) return(t);
        } else {
            if ( m->label() == NULL ) {
                // End of submenu? Pop back one level.
                char *ss = strrchr(menupath, '/');
                if( ss ) *ss = 0;
                else menupath[0] = '\0';
                continue;
            }
            // Menu item?
            char itempath[1024];         // eg. Edit/Copy
            strcpy(itempath, menupath);
            if ( itempath[0] ) strcat(itempath, "/");
            strcat(itempath, m->label());
            if ( strcmp(itempath, findname) == 0 ) {
                return(t);
            }
        }
    }
    return(-1);
}

Fl_Menu_Item *Flx_MenuBar::getItemByName( const char *pName ) {
    int index = getItemIndexByName( pName );
    if ( index == -1 ) return(NULL);
    Fl_Menu_Item *m = (Fl_Menu_Item*)&( menu()[index] );
    return(m);
}


void Flx_MenuBar::clearItemsBelow( const char *pMenuPath ) {
    int index = getItemIndexByName( pMenuPath );
    if ( index == -1 ) return;
    int level = 0;
    ++index;    // skip the item itself, so we delete stuff _below_ it
    while ( index <= size() ) {
        Fl_Menu_Item *m = (Fl_Menu_Item*)&( menu()[index] );
        if ( m->label() == NULL ) {
            // Pop out of submenu, keep track of level
            if ( --level < 0 ) return;
        } else {
            // Descending into a submenu? Keep track of leveL
            if ( m->submenu() )
                ++level;
        }
        // Remove all menu items *and* submenus
        remove( index );
    }
}

void Flx_MenuBar::remove( int index ) {
    _pItemFactory->remove( index );
//	Fl_Menu_Bar::remove( index );
//	//removing icons (if any) and actions:
//	_itrActions = _actions.begin() + index;
//	removeIcon( (*_itrActions)->ident );
//	_actions.erase( _itrActions );
}

void Flx_MenuBar::removeIcon( int ident ) {
//	for( _itrIcons = _icons.begin(); _itrIcons != _icons.end(); _itrIcons++ ) {
//		//if( _itrIcons->ident == ident ) {
//		//	_icons.erase( _itrIcons );
//		//	return;
//		//}
//	}
}

void Flx_MenuBar::draw() {
	Fl_Menu_Bar::draw();
}

} //ns flx