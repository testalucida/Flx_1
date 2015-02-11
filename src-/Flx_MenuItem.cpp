#include <flx/Flx_MenuItem.h>

#include <flx/Flx_SignalBox.h>
#include <FL/Fl_Pixmap.H>

#include <FL/Fl.H>
#include <FL/names.h>
#include <FL/fl_draw.H>

#include <stdio.h>

using namespace my;

namespace flx {

Flx_MenuItem::Flx_MenuItem(  int x, int y, const char *pLabel, int shortcut, const char **pXpm ) 
	: Fl_Pack( x, y, 1, H )
	, _h( H )
	, _shortcut( 0 )
	, _shortcutComing( false )
	, _pImg( 0 )
{
	type( Fl_Pack::HORIZONTAL );
	begin();
		//image box
		_pImageBox = new Flx_SignalBox( x, y, IMAGEBOX_W, H );
		if( pXpm ) {
			_pImg = new Fl_Pixmap( pXpm );
			_pImageBox->image( _pImg );
		}
		_pImageBox->box( FL_FLAT_BOX );
		_pImageBox->color( fl_rgb_color( 218, 237, 212 ) );
		//label box
		_pLabelBox = new Flx_SignalBox( x+IMAGEBOX_W, y, 1, H );
		setLabel( pLabel );
		_pLabelBox->box( FL_FLAT_BOX );
		_pLabelBox->color( fl_rgb_color( 210, 227, 204 ) );
		_pLabelBox->labelfont( fl_font() );
		_pLabelBox->labelsize( fl_size() );
		_pLabelBox->align( FL_ALIGN_LEFT | FL_ALIGN_INSIDE );
		//shortcut box
		_pShortcutBox = new Flx_SignalBox( x+IMAGEBOX_W+_pLabelBox->w(), y, 
										   SHORTCUTBOX_W, H );
		_pShortcutBox->box( FL_FLAT_BOX );
		_pShortcutBox->color( _pImageBox->color() );
		_pShortcutBox->labelfont( _pLabelBox->labelfont() );
		_pShortcutBox->align( _pLabelBox->align() );
		if( shortcut > 0 ) {
			createShortcut( shortcut );
		}
	end();
	size( _pImageBox->w() + _pLabelBox->w() + _pShortcutBox->w(), H );

}

Flx_MenuItem::~Flx_MenuItem(void) {
	if( _pImg ) {
		delete[] _pImg;
	}
}

const char *Flx_MenuItem::createShortcut( int shortcut ) {
	_shortcut = shortcut;
	if( ( _shortcut & FL_CTRL ) == FL_CTRL ) {
		strcpy( _shortcutLabel, "Ctrl+" );
		char c[2];
		c[0] = shortcut - FL_CTRL;
		c[1] = '\0';
		strcat( _shortcutLabel, c );
		_pShortcutBox->label( _shortcutLabel );
	}
	return _shortcutLabel;
}

void Flx_MenuItem::size( int w, int h ) {
	int labelW = w - IMAGEBOX_W - SHORTCUTBOX_W;
	_pLabelBox->size( labelW, h );
	Fl_Group::size( w, h );
}

void Flx_MenuItem::printSizes() {
	printf( "ImageBox: %d, %d, %d, %d\n", _pImageBox->x(), _pImageBox->y(), _pImageBox->w(), _pImageBox->h() );
	printf( "LabelBox: %d, %d, %d, %d\n", _pLabelBox->x(), _pLabelBox->y(), _pLabelBox->w(), _pLabelBox->h() );
	printf( "MenuItem: %d, %d, %d, %d\n", this->x(), this->y(), this->w(), this->h() );
}

void Flx_MenuItem::setLabel( const char *pLabel ) {
	int w = getLabelWidth( pLabel ) + 6; //bug in fl_measure()??
	_pLabelBox->label( pLabel );
	_pLabelBox->size( w, _h );
}

int Flx_MenuItem::getLabelWidth( const char *pLabel ) const {
	int w=0, h;
	//fl_font( FL_HELVETICA, 11 ); //--> wird in Flx_Window gesetzt
	fl_measure( pLabel, w, h, 0 );
	return w;
}

int Flx_MenuItem::handle( int evt ) {
	//printf( "Flx_MenuItem::handle: evt = %s (%d)\n", fl_eventnames[evt], evt );
	switch( evt ) {
	case FL_PUSH: //(1)
		{
			ActionParm action;
			signalSelected.send( *this, action );
		}
	case FL_SHORTCUT:
		if( _shortcutComing ) {
			if( Fl::event_key() <= 'z' ) {
				printf( "Flx_MenuItem::handle( FL_SHORTCUT): evt = %s (%d), event_key=%d\n", 
						fl_eventnames[evt], evt, Fl::event_key() );
				ActionParm action;
				signalSelected.send( *this, action );
			}
			_shortcutComing = false;
		} else {
			_shortcutComing = true;
		}
		return 1;
		break;
	case FL_KEYDOWN:
		//if( _shortcutComing ) {
		//	printf( "Flx_MenuItem::handle(FL_KEYDOWN): evt = %s (%d), event_key=%d\n", 
		//			fl_eventnames[evt], evt, Fl::event_key() );
		//	ActionParm action;
		//	signalSelected.send( *this, action );
		//	_shortcutComing = false;
		//	return 1;
		//}
		break;
	default:
		break;
	}
	return Fl_Pack::handle( evt );
}

} //ns flx
