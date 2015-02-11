#include <flx/Flx_Output.h>

#include <FL/fl_draw.H>

namespace flx {

Flx_Output::Flx_Output( int x, int y, int w, int h, const char *pLabel )
	: Fl_Output( x, y, w, h, pLabel )
{
	textfont( fl_font() );
	textsize( fl_size() );
	labelfont( fl_font() );
	labelsize( fl_size() );
}


Flx_Output::~Flx_Output(void)
{
}

} //ns flx
