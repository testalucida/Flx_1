#ifndef FLX_BOXTYPES_H
#define	FLX_BOXTYPES_H

#include <FL/Fl_Box.H>
#include <flx/flx_enums.h>

namespace flx {

#define FLX_GRADIENT_BOX (Fl_Boxtype)(FL_FREE_BOXTYPE)
#define FLX_GRADIENT_DOWN_BOX (Fl_Boxtype)(FL_FREE_BOXTYPE+1)
#define FLX_GRADIENT_BORDER_BOX (Fl_Boxtype)(FL_FREE_BOXTYPE+4)
#define FLX_GRADIENT_BORDER_DOWN_BOX (Fl_Boxtype)(FL_FREE_BOXTYPE+5)
#define FLX_FAT_BORDER_BOX (Fl_Boxtype)(FL_FREE_BOXTYPE+8)

class Flx_BoxDrawing {
public:
	static void init();
	/**deprecated*/
	static void init( Fl_Color color1, Fl_Color color2 = 0 );
	/**deprecated*/
	static void init( uchar r1, uchar g1, uchar b1, 
		              uchar r2, uchar g2, uchar b2 );
	/**deprecated*/
	static Fl_Color getBaseColor() { return _color1; }

	//methods associated with FLX_xxx_BOXes
	static void drawGradientBox( int x, int y, int w, int h, Fl_Color color );
	static void drawGradientDownBox( int x, int y, int w, int h, Fl_Color color );
	static void drawGradientBorderBox( int x, int y, int w, int h, Fl_Color color );
	static void drawGradientBorderDownBox( int x, int y, int w, int h, Fl_Color color );
	static void drawFlatBox( int x, int y, int w, int h, Fl_Color color );
    static void drawFatBorderBox( int x, int y, int w, int h, Fl_Color color );
	//workhorse methods
	static void drawBorder( int x, int y, int w, int h, 
							Fl_Color leftTop, Fl_Color rightBottom );

	static void drawGradientBox( int x, int y, int w, int h, 
								 Fl_Color color1, Fl_Color color2,
								 Flx_BorderType borderType = FLX_BORDER_NONE );

	static void drawFlatBox( int x, int y, int w, int h, 
							Fl_Color color,
							Flx_BorderType borderType = FLX_BORDER_NONE,
							Fl_Color backColor = FL_BACKGROUND_COLOR );

	static Fl_Color _color1;
	static Fl_Color _color2;
private:
	static bool _isInitialised;
};

//=============================================================

//class Flx_GradientBox : public Fl_Box {
//public:
//	Flx_GradientBox( int x, int y, int w, int h, const char *pLabel=0 );
//
//	  void setGradientColor( Fl_Color color );
//
//	  void setDrawBorder( bool drawBorder );
//
//	  void setBorderColor( Fl_Color color );
//
//protected:
//	void draw();
//
//private:
//	Fl_Color _color1;
//	Fl_Color _color2;
//	bool _drawBorder;
//	Fl_Color _borderColor;
//};

} //ns flx

#endif	/* DEFINES_H */
