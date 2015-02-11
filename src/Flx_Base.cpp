#include <typeinfo>

#include <FL/Fl_Widget.H>

#include "flx/Flx_Base.h"

namespace flx {

Flx_Base::Flx_Base() 
    : _type( FLX_TYPE_UNKNOWN ) 
    , _pWidget( NULL )
{
}

Flx_Type Flx_Base::getFlxType() const {
    return _type;
}

const char *Flx_Base::toString() const {
    _toString.clear();
    _toString.add( typeid( *_pWidget ).name() );
    _toString.add( " - Flx_Type " );
    _toString.addInt( _type );
    _toString.add( " - label " );
    _toString.add( _pWidget->label() );
    _toString.add( " at " );
    char address[20];
    sprintf( address, "%p", _pWidget );
    _toString.add( address );
    _toString.add( "; x=" );
    _toString.addInt( _pWidget->x() );
    _toString.add( ", y=" );
    _toString.addInt( _pWidget->y() );
    _toString.add( ", w=" );
    _toString.addInt( _pWidget->w() );
    _toString.add( ", h=" );
    _toString.addInt( _pWidget->h() );
    
    return _toString.get();
}

my::CharBuffer &Flx_Base::toCharBuffer() const {
    toString();
    return _toString;
}

void Flx_Base::scaleDown() {
    if( _pWidget ) {
        int sz = _pWidget->labelsize();
        sz = sz > 2 ? sz - 2 : sz;
        _pWidget->labelsize( sz );
    }
}

void Flx_Base::scaleUp() {
    if( _pWidget ) {
        _pWidget->labelsize( _pWidget->labelsize() + 2 );
    }
}

}
