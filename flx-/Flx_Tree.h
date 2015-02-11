/* 
 * File:   Flx_Tree.h
 * Author: max
 *
 * Created on 20. Dezember 2013, 10:52
 */

#ifndef FLX_TREE_H
#define	FLX_TREE_H

#include <FL/Fl_Tree.H>

#include <flx/flx_specialsignals.h>

#include "flx_signalparms.h"

namespace flx {

class Flx_Tree : public Fl_Tree {
public:
    TreeItemSignal signalTreeItem;
public:
    Flx_Tree( int x, int y, int w, int h, const char * pLabel = 0 );
    static void staticOnTreeAction( Fl_Widget *, void * );
    void onTreeAction();
    Fl_Tree_Item &getSelectedItem() const { return *_pSelectedItem; }
    virtual ~Flx_Tree();
private:
    Fl_Tree_Item *_pSelectedItem;
};

} //flx
#endif	/* FLX_TREE_H */

