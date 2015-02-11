#include <flx/Flx_MenuItemFactory.h>

#include <flx/flx_signalparms.h>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Pixmap.H>

#include <algorithm>
#include <string.h>

namespace flx {

#ifndef FLX_MENUITEM_LABEL
#define FLX_MENUITEM_LABEL FL_FREE_LABELTYPE
#endif

Flx_MenuItemFactory::Flx_MenuItemFactory( Fl_Menu_ &menu ) : _menu( menu ) {
    Fl::set_labeltype( FLX_MENUITEM_LABEL, menuItemDraw, menuItemMeasure );
    //printf( "menu address: %p\n", &menu );
}

void Flx_MenuItemFactory::staticMenuItemCallback( Fl_Widget *pW, void *pUserdata ) {
    //Fl_Menu_ *pMenu= (Fl_Menu_*)pW;
    MenuItemAction *pAction = (MenuItemAction*)pUserdata;
    if( pAction->pSignalSelected ) {
        pAction->pSignalSelected->send( (Fl_Menu_*)pW, pAction );
    }
}

Fl_Menu_Item &Flx_MenuItemFactory::addMenuItem( 
                                        const char* pMenuPath, 
                                        int ident,  //identify your menu item	
                                        MenuItemSelectedSignal *pCallbackSignal,
                                        int shortcut, 						
                                        const char **ppXpm,
                                        void *pUserdata, 
                                        int flags,
                                        int pos )
{
    MenuItemAction *pAction = createActionAndIcon( 
                               pMenuPath, ident, pCallbackSignal, shortcut, 
                               ppXpm, pUserdata );
	int n;
    if( pos > -1 ) {
		n = _menu.insert( pos, pMenuPath, shortcut, staticMenuItemCallback, pAction, flags );
	} else {
		n = _menu.add( pMenuPath, shortcut, staticMenuItemCallback, pAction, flags );
	}
    
	//Fl_Menu_Item *pItem = const_cast<Fl_Menu_Item*>( _menu.find_item( pMenuPath ) );
    Fl_Menu_Item *pItem = (Fl_Menu_Item*)&(_menu.menu()[n]);
    pItem->labeltype( FLX_MENUITEM_LABEL );
	return *pItem;
}

 Fl_Menu_Item &Flx_MenuItemFactory::getMenuItem( int n ) {
    Fl_Menu_Item *pItem = (Fl_Menu_Item*)&(_menu.menu()[n]);
	return *pItem;
 }

int Flx_MenuItemFactory::getMenuItemCount() {
    return _menu.size();
}
 
MenuItemAction *Flx_MenuItemFactory::createActionAndIcon( 
                                    const char* pMenuPath, 
                                    int ident,  //identify your menu item
                                    MenuItemSelectedSignal *pCallbackSignal,
                                    int shortcut, 						
                                    const char **ppXpm,
                                    void *pUserdata )
{
    MenuItemAction *pAction = new MenuItemAction;
    pAction->pSignalSelected = pCallbackSignal;
	pAction->ident = ident;
    pAction->label = pMenuPath;
	pAction->pUserdata = pUserdata;
    
    if( ppXpm ) {
        if( !existsIcon( pMenuPath ) ) {
            Fl_Pixmap *pIcon = new Fl_Pixmap( ppXpm );
            const char *p;
            const char *lbl = strrchr( pMenuPath, '/' );
            p = ( lbl ) ? ++lbl : pMenuPath;
            Icon icon = {&_menu, p, pIcon};
            _icons.push_back( icon );
        }
	}
    
	_actions.push_back( pAction );
    return pAction;
}

bool Flx_MenuItemFactory::existsIcon( const char *pMenuPath ) {
    for( _itrIcons = _icons.begin(); _itrIcons != _icons.end(); _itrIcons++ ) {
        if( !strcmp( _itrIcons->pMenuPath, pMenuPath ) ) {
            return true;
        }
    }
    return false;
}

void Flx_MenuItemFactory::deleteFactory() {
	std::for_each( _actions.begin(), _actions.end(), []( MenuItemAction *pAction ) {
		delete pAction;
	});
    
    deleteIcons();
}

//delete Icons of *this* instance which means deleting icons of _menu
void Flx_MenuItemFactory::deleteIcons() {
    std::vector< Icon >::iterator itr;
    int n = _icons.size();
    for( itr = _icons.begin(); itr != _icons.end(); itr++ ) {
        Icon icon = *itr;
        if( icon.pMenu == &_menu ) {
            delete icon.pIcon;
            _icons.erase( itr );
            break;
        }
        if( n != _icons.size() ) {
            //one icon deleted and therefore escaped the loop;
            //enter loop again by recursive call
            deleteIcons();
        }
	}
}

void Flx_MenuItemFactory::remove( int index ) {
	_menu.remove( index );
	//removing icons (if any) and actions:
	_itrActions = _actions.begin() + index; 
	removeIcon( (*_itrActions)->label.get() );
	_actions.erase( _itrActions );
    delete *_itrActions;
}

void Flx_MenuItemFactory::removeIcon( const char *pMenuPath ) {
	for( _itrIcons = _icons.begin(); _itrIcons != _icons.end(); _itrIcons++ ) {
		if( !strcmp( _itrIcons->pMenuPath, pMenuPath ) ) {
            delete _itrIcons->pIcon;
			_icons.erase( _itrIcons );
			return;
		}
	}
}

void Flx_MenuItemFactory::menuItemDraw( const Fl_Label *label, 
								int x, int y, int w, int h, 
								Fl_Align align ) 
{
	Fl_Pixmap *pIcon = NULL;
	std::vector < Icon >::iterator itrIcons;
	for( itrIcons = _icons.begin(); itrIcons != _icons.end(); itrIcons++ ) {
		if( !strcmp( itrIcons->pMenuPath, label->value ) ) {
			fl_draw_pixmap( itrIcons->pIcon->data(), x, y+2 );
            break;
		}
	}
	int baseline = y + (h/2 + fl_size()/2);
	fl_color( label->color );
    //fl_color( FL_WHITE );
	fl_draw( label->value, x+22, baseline );
}

void Flx_MenuItemFactory::menuItemMeasure( const Fl_Label *label, int &w, int &h ) {
	int imageH = 20;
	int fontsize = fl_size() + 4;
	h = imageH > fontsize ? imageH : fontsize;
	int labelW = 0, labelH;
	fl_measure( label->value, labelW, labelH );
	w = 20 + 8 + labelW + 4;
}

Flx_MenuItemFactory::~Flx_MenuItemFactory() {
    deleteFactory();
}

} //flx