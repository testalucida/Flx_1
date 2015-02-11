#include <flx/flx_boxtypes.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace flx {

Fl_Color Flx_BoxDrawing::_color1 = fl_lighter( FL_GRAY );
Fl_Color Flx_BoxDrawing::_color2 = fl_darker( FL_GRAY );
bool Flx_BoxDrawing::_isInitialised = false;

void Flx_BoxDrawing::init() {
	if( !_isInitialised ) {
		Fl::set_boxtype( FLX_GRADIENT_BOX, Flx_BoxDrawing::drawGradientBox, 1, 1, 2, 2 );
		Fl::set_boxtype( FLX_GRADIENT_DOWN_BOX, Flx_BoxDrawing::drawGradientDownBox, 1, 1, 2, 2 );
		Fl::set_boxtype( FLX_GRADIENT_BORDER_BOX, Flx_BoxDrawing::drawGradientBorderBox, 1, 1, 2, 2 );
		Fl::set_boxtype( FLX_GRADIENT_BORDER_DOWN_BOX, Flx_BoxDrawing::drawGradientBorderDownBox, 1, 1, 2, 2 );
        Fl::set_boxtype( FLX_FAT_BORDER_BOX, Flx_BoxDrawing::drawFatBorderBox, 8, 8, 16, 16 );
		//Fl::set_boxtype( FL_FLAT_BOX, Flx_BoxDrawing::drawFlatBox, 1, 1, 2, 2 );
		_isInitialised = true;
	}
}

void Flx_BoxDrawing::init( Fl_Color color1, Fl_Color color2 ) {
	_color1 = color1;
	_color2 = color2; // ? color2 : fl_darker( _color1 );
	init();
	//if( !_isInitialised ) {
	//	Fl::set_boxtype( FLX_GRADIENT_BOX, Flx_BoxDrawing::drawGradientBox, 1, 1, 2, 2 );
	//	Fl::set_boxtype( FLX_GRADIENT_DOWN_BOX, Flx_BoxDrawing::drawGradientDownBox, 1, 1, 2, 2 );
	//	Fl::set_boxtype( FLX_GRADIENT_BORDER_BOX, Flx_BoxDrawing::drawGradientBorderBox, 1, 1, 2, 2 );
	//	Fl::set_boxtype( FLX_GRADIENT_BORDER_DOWN_BOX, Flx_BoxDrawing::drawGradientBorderDownBox, 1, 1, 2, 2 );
	//	//Fl::set_boxtype( FL_FLAT_BOX, Flx_BoxDrawing::drawFlatBox, 1, 1, 2, 2 );
	//	_isInitialised = true;
	//}
}

void Flx_BoxDrawing::init( uchar r1, uchar g1, uchar b1, 
		                  uchar r2, uchar g2, uchar b2 ) 
{
	init( fl_rgb_color( r1, g1, b1 ), fl_rgb_color( r2, g2, b2 ) );
}

void Flx_BoxDrawing::drawGradientBox( int x, int y, int w, int h, Fl_Color color1 ) {
	//drawGradientBox( x, y, w, h, _color1, _color2, FLX_BORDER_NONE );
	drawGradientBox( x, y, w, h, fl_lighter( color1 ), fl_darker( color1 ), FLX_BORDER_NONE );
}

void Flx_BoxDrawing::drawGradientDownBox( int x, int y, int w, int h, Fl_Color color1 ) {
	//Fl_Color downcolor = fl_color_average( _color2,  fl_darker( _color2 ), .88f );
	Fl_Color downcolor = fl_color_average( color1,  fl_darker( color1 ), .88f );
	//fprintf( stderr, "drawGradientDownBox\n" );
	drawGradientBox( x, y, w, h, downcolor, downcolor, FLX_BORDER_NONE );
}

void Flx_BoxDrawing::drawGradientBorderBox( int x, int y, int w, int h, Fl_Color color1 ) {
	//drawGradientBox( x, y, w, h, _color1, _color2, FLX_BORDER_UP, color1 );
	drawGradientBox( x, y, w, h, FL_WHITE, color1, FLX_BORDER_UP );
}

void Flx_BoxDrawing::drawGradientBorderDownBox( int x, int y, int w, int h, Fl_Color color1 ) {
	//drawGradientBox( x, y, w, h, _color2, _color2, FLX_BORDER_DOWN );
	drawGradientBox( x, y, w, h, fl_darker( color1 ), color1, FLX_BORDER_DOWN );
}

void Flx_BoxDrawing::drawFlatBox( int x, int y, int w, int h, Fl_Color color1 ) {
	drawFlatBox( x, y, w, h, _color2, FLX_BORDER_NONE, color1 );
}

static const int fat = 6;
void Flx_BoxDrawing::drawFatBorderBox( int x, int y, int w, int h, Fl_Color c ) {
	fl_color( c );
	fl_rectf( x, y, w, h );
	fl_color( FL_DARK_GREEN ) ;
	
	fl_rectf( x, y, w, fat );
	fl_rectf( x, y+fat, fat, h-2*fat );
	fl_rectf( x, y+h-fat, w, fat );
	fl_rectf( x+w-fat, y+fat, fat, h-2*fat );
}

/////////////////////////////////////////////////////////////////////////

void Flx_BoxDrawing::drawGradientBox( int x, int y, int w, int h, 
									  Fl_Color color1, Fl_Color color2,
									  Flx_BorderType borderType ) 
{
	Fl_Color c1 = color1, c2 = color2, c = color1; //FL_WHITE;
	int x1 = x;
	int y1 = y;
	int rectH = 2;
	int h1 = h-rectH;
	h1 += ( h1 & 1 ); //force h1 even
	float deltaweight = float(2.0/h);
	float weight = 1.0f;
	rectH = rectH < 1 ? 1 : rectH;
	for( int i = 0; i <= h1; i += rectH ) {
		fl_rectf( x1, y1, w, rectH, c );
		weight -= deltaweight;
		c = fl_color_average( c1, c2, weight );
		y1 += rectH;
	}

	if( borderType == FLX_BORDER_NONE ) return;

	Fl_Color leftTop, bottomRight;
	if( borderType == FLX_BORDER_UP ) {
		leftTop = fl_lighter( color2 );
		bottomRight = fl_darker( color2 );
	} else if( borderType == FLX_BORDER_DOWN ) {
		leftTop = fl_darker( color2 );
		bottomRight = fl_lighter( color2 );
	}
	drawBorder( x, y, w, h, leftTop, bottomRight );
}


void Flx_BoxDrawing::drawBorder( int x, int y, int w, int h, 
				 Fl_Color leftTop, Fl_Color rightBottom ) 
{
	//Fl_Color lt2 = fl_darker( leftTop );
	
	//outer left/top:
	fl_color( leftTop );
	fl_line( x, y+h, x, y, x+w, y );

	//outer right/bottom:
	fl_color( rightBottom );
	fl_line( x+w, y, x+w, y+h, x, y+h );

	////inner left/top:
	//fl_color( lt2 );
	//fl_line( x+1, y+h-1, x+1, y+1, x+w-1, y+1 );

	////inner right/bottom:
	//fl_color( fl_darker( rightBottom ) );
	//fl_line( x+w-1, y+1, x+w-1, y+h-1, x+1, y+h-1 );
}

 void Flx_BoxDrawing::drawFlatBox( int x, int y, int w, int h, Fl_Color color,
					Flx_BorderType borderType, Fl_Color backColor )
 {
	 fl_rectf( x, y, w, h, color );
 }
//======================================================================

//Flx_GradientBox::Flx_GradientBox( int x, int y, int w, int h, const char *pLabel ) :
//	Fl_Box( x, y, w, h, pLabel ),
//	_color1( Flx_BoxDrawing::_color1 ), //default
//	_color2( Flx_BoxDrawing::_color2 ),
//	_drawBorder( true ),
//	_borderColor( FL_GRAY )
//	{
//	}
//
//	void Flx_GradientBox::setGradientColor( Fl_Color color ) {
//		_color1 = color;
//		_color2 = fl_darker( _color1 );
//	}
//
//	void Flx_GradientBox::setDrawBorder( bool drawBorder ) {
//		_drawBorder = drawBorder;
//	}
//
//	void Flx_GradientBox::setBorderColor( Fl_Color color ) {
//		_borderColor = color;
//	}
//
//void Flx_GradientBox::draw() {
//	Fl_Color color1 = _color1;
//	Fl_Color color2 = _color2;
//	int x1 = x();
//	int x2 = x() + w();
//	int y1 = y();
//	int h1 = h();
//	int y2 = y() + h1;
//	int rectH = 2;
//	for( int i = 0; i < h1; i += rectH ) {
//		color1 = fl_color_average( color1, color2, .97f );
//		fl_rectf( x1, y1, w(), rectH, color1 );
//		y1 += rectH;
//	}
//	Fl_Box::draw_label();
//}

} //ns flx