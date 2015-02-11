#include <flx/Flx_IntInput.h>

#include <FL/Fl.H>
#include <FL/Fl_Input_.H>
#include <FL/fl_draw.H>

using namespace my;

namespace flx {

Flx_IntInput::Flx_IntInput( int x, int y, int w, int h, const char *pLabel ) 
	: Flx_NumericInput( x, y, w, h, pLabel )
{
    type( FL_INT_INPUT );
//	textfont( fl_font() );
//	textsize( fl_size() );
//	labelfont( fl_font() );
//	labelsize( fl_size() );
}

 const CharBufferPtr &Flx_IntInput::getInput() const {
     CharBufferPtr p( new CharBuffer( value() ) );
     return p;
 }

Flx_IntInput::~Flx_IntInput(void) {
}

}
