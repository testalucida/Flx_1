#include <flx/Flx_Box.h>

#include <FL/fl_draw.H>

namespace flx {

Flx_Box::Flx_Box( int x, int y, int w, int h, const char *pLabel )
	: Fl_Box( x, y, w, h, pLabel )
    , _underline( 0 )
{
	box( FL_FLAT_BOX );
	color( fl_lighter(  FL_GRAY ) );
	labelsize( _defaultSize );
	align( FL_ALIGN_LEFT | FL_ALIGN_INSIDE );
}

void Flx_Box::draw() {
    Fl_Box::draw();
    int Y = y() + h() - 3;
    if( _underline > 0 ) {
        fl_line( x(), Y, x()+w(), Y );
    }
    if( _underline > 1 ) {
        Y -= 2;
        fl_line( x(), Y, x()+w(), Y );
    }
}

Flx_Box::~Flx_Box() {
}


//int Flx_Box::handle( int evt ) {
//}

} //ns flx
