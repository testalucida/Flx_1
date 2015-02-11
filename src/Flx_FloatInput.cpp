/* 
 * File:   Fl_FloatInput.cpp
 * Author: max
 * 
 * Created on 3. Oktober 2013, 13:50
 */

#include <flx/Flx_FloatInput.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Input_.H>

#include <my/CharBuffer.h>
#include <my/convert.h>

#include <cstdlib>

namespace flx {

Flx_FloatInput::Flx_FloatInput( int x, int y, int w, int h, const char *pLabel ) 
: /*Fl_Float_Input*/Flx_NumericInput( x, y, w, h, pLabel )
{
    type( FL_FLOAT_INPUT );
    value( 0.00 );
//    textfont( fl_font() );
//	textsize( fl_size() );
//	labelfont( fl_font() );
//	labelsize( fl_size() );
}

void Flx_FloatInput::value( float f, const char *pFormat ) {
    Flx_NumericInput::value( my::Convert::toString( f, pFormat )->c_str() );
}

float Flx_FloatInput::floatValue() const {
    const char *pVal = Flx_NumericInput::value();
    return ( pVal ? std::atof( pVal ) : 0 );
}

Flx_FloatInput::~Flx_FloatInput( ) {
}

} //flx