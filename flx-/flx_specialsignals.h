#ifndef _FLX_SPECIALSIGNALS_H_
#define _FLX_SPECIALSIGNALS_H_

#include <my/Signal.hpp>

class Fl_Menu_;

namespace flx {

class Flx_Tree;
struct MenuItemAction;
struct TreeItemAction;

typedef my::Signal< Fl_Menu_, MenuItemAction > MenuItemSelectedSignal;
typedef MenuItemSelectedSignal ShortcutSignal;
typedef my::Signal< Flx_Tree, TreeItemAction > TreeItemSignal;

} //flx

#endif