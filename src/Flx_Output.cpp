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

void Flx_Output::const_value( const char *pVal ) { 
    _constValue.clear();
    _constValue.add( pVal ); 
    value( _constValue.get() );
}

void Flx_Output::const_value( const std::string &val ) { 
    const_value( val.c_str() ); 
}

//void Flx_Output::value( char *pVal ) { 
//    const_value( pVal );
//    Fl_Output::value( _constValue.get() ); 
//}
//
//char *Flx_Output::value() { 
//    return _constValue.get(); 
//}

Flx_Output::~Flx_Output(void)
{
}

} //ns flx
