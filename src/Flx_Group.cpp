#include <flx/Flx_Group.h>
#include <flx/flx_boxtypes.h>

#include <FL/fl_draw.H>

namespace flx {

Flx_Group::Flx_Group( int x, int y, int w, int h, const char *pLabel )
	: Fl_Group( x, y, w, h, pLabel )
{
	Flx_BoxDrawing::init();
	//box( FL_FLAT_BOX );
	color( FL_WHITE );
	//fl_font( FL_HELVETICA, 12 );
    labelfont( fl_font() );
    labelsize( fl_size() );
}


Flx_Group::~Flx_Group(void) {
}

} //flx