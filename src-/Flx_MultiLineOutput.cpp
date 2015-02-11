/* 
 * File:   Flx_MultiLineOutput.cpp
 * Author: max
 * 
 * Created on 29. September 2013, 14:48
 */

#include <flx/Flx_MultiLineOutput.h>

#include <FL/fl_draw.H>

namespace flx {

Flx_MultiLineOutput::Flx_MultiLineOutput( int x, int y, int w, int h, const char *pLabel )
: Fl_Multiline_Output( x, y, w, h, pLabel )
{
    textfont( fl_font() );
	textsize( fl_size() );
	labelfont( fl_font() );
	labelsize( fl_size() );
}


Flx_MultiLineOutput::~Flx_MultiLineOutput( ) {
}

} //flx