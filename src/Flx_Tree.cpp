/* 
 * File:   Flx_Tree.cpp
 * Author: max
 * 
 * Created on 20. Dezember 2013, 10:52
 */

#include <flx/Flx_Tree.h>
#include <flx/flx_signalparms.h>

namespace flx {
    
Flx_Tree::Flx_Tree( int x, int y, int w, int h, const char * pLabel ) 
: Fl_Tree( x, y, w, h, pLabel )
, _pSelectedItem( NULL )
{
    callback( Flx_Tree::staticOnTreeAction, this );
}

void Flx_Tree::staticOnTreeAction( Fl_Widget *pW, void *pUserdata ) {
    ((Flx_Tree*)pUserdata)->onTreeAction();
}

void Flx_Tree::onTreeAction() {
    Fl_Tree_Reason reason = callback_reason();
    Fl_Tree_Item *pItem = callback_item();
    
    if( reason == FL_TREE_REASON_SELECTED ) {
        _pSelectedItem = pItem;
    } else if( reason == FL_TREE_REASON_DESELECTED && pItem == _pSelectedItem ) {
        _pSelectedItem = NULL;
    }
    
    
    TreeItemAction action;
    action.pItem = pItem;
    action.reason = reason;
    signalTreeItem.send( *this, action );
}

Flx_Tree::~Flx_Tree( ) {
}

}//flx