#include <flx/Flx_DateChooser.h>
#include <flx/Flx_Output.h>
#include <flx/Flx_Button.h>
#include <flx/Flx_Calendar.h>

#include <my/convert.h>

#include <FL/fl_draw.H>

using namespace my;

namespace flx {

Flx_DateChooser::Flx_DateChooser( int x, int y, int h, const char *pLabel ) 
	: Flx_Group( x, y, 1, h ) 
{	
	int outputW = 80;
	int labelW=0, H;
	//int buttonW = 23;
	int hmargin = 10;

	if( pLabel ) {
		fl_measure( pLabel, labelW, H );
	}

	Fl_Widget *pParent = this->parent();
	Fl_Boxtype boxtype;
	Fl_Color color;
	if( pParent ) {
		boxtype = pParent->box();
		color = pParent->color();
	} else {
		boxtype = FL_FLAT_BOX;
		color = FL_WHITE;
	}

	//calc. group's size
	int w = labelW + outputW + h /*The button measures to hxh */;

	size( w, h );
	align( FL_ALIGN_LEFT );
	label( pLabel );
	labelsize( fl_size() ); 

	begin();
		_pOutputDate = new Flx_Output( x, y, outputW, h );
		//_pOutputDate->box( boxtype );
		_pOutputDate->visible_focus( 0 );

		setDate( MyDate( true ) );

		_pBtnCalendar = new Flx_Button( _pOutputDate->x() + _pOutputDate->w(),
										_pOutputDate->y(),
										_pOutputDate->h(), _pOutputDate->h(), "@8|<" );
		//_pBtnCalendar->box( boxtype );
		//_pBtnCalendar->color( color );
		_pBtnCalendar->signalSelected.connect< Flx_DateChooser, &Flx_DateChooser::onButtonHit >( this );
	end();
}

void Flx_DateChooser::onButtonHit( Flx_Button &, ActionParm & ) {
	Flx_Calendar cal;
	cal.setDate( _date.ToEurString().c_str() );
	MyDate date = cal.show( window()->x() + _pBtnCalendar->x(), window()->y() + _pBtnCalendar->y() );
	if( date.IsSet() ) {
		setDate( date );
        signalDateSelected.send( this, &date );
	}
}

void Flx_DateChooser::setDate( const MyDate &dt ) {
	_pOutputDate->value( dt.ToEurString().c_str() );
	_date = dt;
}

const MyDate &Flx_DateChooser::getDate() const {
	return _date;
}

} //flx