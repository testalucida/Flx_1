
#include <FL/fl_draw.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <flx/flx_enums.h>
#include <flx/Flx_ToolBar.h>
//#include <flx/Flx_InputChoice.h>

namespace flx {

////////////////////////////////////////////////////////////////////////
////////////// Flx_ToolButton //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

Flx_ToolButton::Flx_ToolButton( Fl_Boxtype type, Fl_Boxtype downtype ) : 
	Flx_Button( 0, 0, 0, 0 )
{
	init( type, downtype );
}

Flx_ToolButton::Flx_ToolButton( const char *pLabel, Fl_Boxtype type, Fl_Boxtype downtype ) : 
	Flx_Button( 0, 0, 0, 0, pLabel )
{
	init( type, downtype );
}

Flx_ToolButton::Flx_ToolButton( const char **ppXpm, const char **ppXpmDeact, Fl_Boxtype type, Fl_Boxtype downtype ) : 
	Flx_Button( 0, 0, 0, 0 )
{
	Fl_Pixmap *pPixmap = new Fl_Pixmap( ppXpm );
	image( pPixmap );
	Fl_Pixmap *pPixmapDeact = new Fl_Pixmap( ppXpmDeact );
	deimage( pPixmapDeact );

	init( type, downtype );
}

Flx_ToolButton::Flx_ToolButton( int x, int y, int w, int h, Fl_Boxtype type, Fl_Boxtype downtype ) :
	Flx_Button( x, y, w, h )
{
	init( type, downtype );
}

Flx_ToolButton::~Flx_ToolButton() {
}

void Flx_ToolButton::init( Fl_Boxtype type, Fl_Boxtype downtype ) {
	box( type );
	down_box( downtype );
	visible_focus( 0 );
	_ident = -1;
}


////////////////////////////////////////////////////////////////////////
////////////// Flx_ToolBar /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

Flx_ToolBar::Flx_ToolBar( int x, int y, int w, int h ) 
	: Fl_Group( x, y, w, h ),
	  XSPACING( 5 ),
	  YSPACING( 1 ),
	  TOOLHEIGHT( h - 2*YSPACING ),
	  TOOLBAR_LABELSIZE( 11 ),
	  _isCloseable( false ),
	  _pCloseBtn( NULL ),
	  _labelwidth( 0 ),
	  _labelSuppressed( false ),
	  _pStrut( new Fl_Box( x, y, w, 1 ) )
{
    box( FL_FLAT_BOX );
	resizable( _pStrut );
	end();
}

void Flx_ToolBar::setLabel( const char *pLabel ) {
	labelsize( TOOLBAR_LABELSIZE );
	label( pLabel );
	labelcolor( FL_WHITE );
	align( FL_ALIGN_LEFT | FL_ALIGN_INSIDE );
	_label.clear();
	_label.append( pLabel );
	int h=0;
	fl_font( 0, TOOLBAR_LABELSIZE );
	fl_measure( pLabel, _labelwidth, h );
}

Flx_ToolButton &Flx_ToolBar::addButton( const char **xpm, 
										const char **xpm_deact,
										const char *pTooltip,
										int ident,
										void *pUserdata ) 
{

	Flx_ToolButton &btn = createButton( getMaxX(), pTooltip, ident, pUserdata );
	Fl_Pixmap *pixmap = new Fl_Pixmap( xpm );
    btn.image( pixmap );

    Fl_Pixmap *pixmap_dea = new Fl_Pixmap( xpm_deact );
    btn.deimage( pixmap_dea );
	return btn;
}

Flx_ToolButton &Flx_ToolBar::createButton( int x, 
											const char *pTooltip,
											int ident,
											void *pUserdata ) 
{
	int Y = y() + 2;
	int H = h() - 4;
	Flx_ToolButton *pBtn = 
		new Flx_ToolButton( x, Y, H, H, box(), FL_DOWN_BOX );
		//new Flx_ToolButton( x, y() + YSPACING, TOOLHEIGHT, TOOLHEIGHT, box(), FL_DOWN_BOX );
	pBtn->tooltip( pTooltip );
	pBtn->visible_focus( 0 );
	pBtn->setIdent( ident );
	pBtn->signalSelected.connect< Flx_ToolBar, &Flx_ToolBar::onToolButtonSelected >( this );
	resizeStrut( pBtn );

	add( pBtn );
	return *pBtn;
}

void Flx_ToolBar::onToolButtonSelected( Flx_Button &btn, ActionParm &action ) {
	Flx_ToolButton &toolBtn = (Flx_ToolButton&)btn;
	ToolAction toolAction;
	toolAction.ident = toolBtn.getIdent();
	toolAction.pSelectedButton = &toolBtn;
	toolAction.pUserdata = btn.user_data();
	signalToolButtonSelected.send( *this, toolAction );
}

//void Flx_ToolBar::addCheckbox( const char *pLabel,
//					  ON_WIDGET_CALLBACK cb,
//					  Fl_Align labelAlign,
//					  bool isChecked,
//					  int ident,
//					  void *pUserdata )
//{
//	Fl_Check_Button *pBtn = new Fl_Check_Button( getMaxX(), y() + YSPACING, 18, 18, "C" );
//	
//	resizeStrut( pBtn );
//	
//	add( pBtn );
//}

//void Flx_ToolBar::addChoice( Fl_Choice &choice ) {
//	choice.resize( getMaxX(), y() + YSPACING, choice.w(), h() - 2*YSPACING );
//
//	resizeStrut( &choice );
//
//	add( choice );
//}

//void Flx_ToolBar::addChoice( Flx_InputChoice &choice ) {
//	choice.resize( getMaxX(), y() + YSPACING, choice.w(), h() - 2*YSPACING );
//
//	resizeStrut( &choice );
//
//	add( choice );
//}

void Flx_ToolBar::addLabel( const char *pLabel ) {
	fl_font( 0, TOOLBAR_LABELSIZE );
	int w, h;
	fl_measure( pLabel, w, h );
	Fl_Box *pBox = 
		new Fl_Box( getMaxX(), 
		            y() + YSPACING, 
					w, 
					this->h() - 2*YSPACING,
					pLabel ); 
	pBox->align( FL_ALIGN_LEFT | FL_ALIGN_INSIDE );
	pBox->labelsize( TOOLBAR_LABELSIZE );

	resizeStrut( pBox );

	add( pBox );
}

void Flx_ToolBar::addSeparator() {
	Fl_Box *pBox = new Fl_Box( getMaxX(), y() + YSPACING, 8, h() - 2*YSPACING, "|" );
	pBox->align( FL_ALIGN_CENTER | FL_ALIGN_INSIDE );
	resizeStrut( pBox );
	add( pBox );
}

void Flx_ToolBar::resize( int x, int y, int w, int h ) {
	if( _labelwidth > w ) {
		if( !_labelSuppressed ) {
			label( "" );
			_labelSuppressed = true;
		}
	}  else  {
		if( _labelSuppressed ) {
			label( _label.c_str() );
			_labelSuppressed = false;
		}
	}

	Fl_Group::resize( x, y, w, h );
	
	if( _pCloseBtn && _isCloseable ) {
		int X = this->x() + this->w() - TOOLHEIGHT - XSPACING; /*TOOLHEIGHT = toolwidth*/
		X = X < this->x() ? this->x() : X;
		int W = w < TOOLHEIGHT ? w : TOOLHEIGHT;
		_pCloseBtn->resize( X, _pCloseBtn->y(), W, _pCloseBtn->h() );
	}
}


int Flx_ToolBar::getMaxX() {
	int xmax = 0;
	for( int i = 0, imax = children(); i < imax; i++ ) {
		if( child(i) != _pStrut ) {
			int x = child(i)->x() + child(i)->w();
			xmax = x > xmax ? x : xmax;
		}
	}
	return xmax + XSPACING;
}

void Flx_ToolBar::resizeStrut( Fl_Widget *pMostRightWidget ) {
	int maxx = pMostRightWidget->x() + pMostRightWidget->w();
	int ww = this->w() - maxx;
	_pStrut->resize( maxx, _pStrut->y(), ww, 1 );
}

Fl_Widget *Flx_ToolBar::getTool( int ident ) const {
	for( int i = 0, imax = children(); i < imax; i++ ) {
		Fl_Widget *pW =  child( i );
		if( typeid( *(pW) ).name() == typeid( Flx_ToolButton ).name() ) {
			Flx_ToolButton &btn = *(Flx_ToolButton*)pW;
			if( btn.getIdent() == ident ) {
				return pW;
			}
		}
	}
	return (Fl_Widget*)0;
}

Flx_ToolBar::~Flx_ToolBar() {
    
}

} //ns myfltk