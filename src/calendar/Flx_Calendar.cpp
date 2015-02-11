
#include <flx/Flx_Calendar.h>
#include <src/calendar/DayTable.h>
#include <src/calendar/CalendarController.h>
#include <src/calendar/strings.h>

#include <flx/Flx_Choice.h>
#include <flx/Flx_Button.h>
#include <flx/Flx_ReturnButton.h>
#include <flx/flx_signalparms.h>

#include <my/datetime.h>
#include <my/compat.h>
#include <my/exceptions.h>

#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>

#include <stdio.h>

using namespace my;
using namespace std;


namespace flx {

static const int ROWHEIGHT = 20;
static const int COLWIDTH = 20;
static const int MARGIN = 2;
static const int BAR_H = 25;
static const int TABLE_H = 7*ROWHEIGHT;
static const int TABLE_W = 8*COLWIDTH + 2*MARGIN;
static const int BUTTON_H = 20;
static const int BUTTONGROUP_H = 24;
	

Flx_Calendar::Flx_Calendar( int x, int y ) : 
	Fl_Double_Window( x, y, 
			          TABLE_W + 2*MARGIN, 
					  BAR_H + TABLE_H + BUTTONGROUP_H + 2*MARGIN )
	
	, _isDragging(false) 
{
    //fl_register_images();
    box(FL_DOWN_BOX);
    clear_border();

    //make a gray bar as a replacement for the title bar,
    //containing 2 comboboxes for entering year and month
    Fl_Group *pBarGrp = new Fl_Group( MARGIN, MARGIN, TABLE_W, BAR_H );
    pBarGrp->box(FL_FLAT_BOX);
    pBarGrp->begin();
    {
        _pMoCho = new Flx_Choice(45, 4, 55, BAR_H - 5);
        _pMoCho->box(FL_FLAT_BOX);
        _pMoCho->textsize(10);
		_pMoCho->visible_focus( 0 );


        _pYCho = new Flx_Choice(45 + 2 + _pMoCho->w(), 4, 55, BAR_H - 5);
        _pYCho->textsize(10);
		_pYCho->visible_focus( 0 );
    }
    pBarGrp->color(FL_DARK3);
    pBarGrp->end();

    _pDayTable = new DayTable( MARGIN, MARGIN + BAR_H, TABLE_W, TABLE_H );
 
    _pDayTable->end();

    _pBtnOk = new Flx_ReturnButton( MARGIN + COLWIDTH, 
			                        this->h() - 2*MARGIN - BUTTON_H, 
									70, 
									BUTTON_H, "OK" );
	_pBtnOk->labelsize( 10 );
	_pBtnOk->callback( staticOnOkCancel, this );
	_pBtnCancel = new Flx_Button( _pBtnOk->x() + _pBtnOk->w() + MARGIN,
			                        _pBtnOk->y(), _pBtnOk->w(), _pBtnOk->h(),
									"Abbrechen" );
	_pBtnCancel->labelsize( _pBtnOk->labelsize() );
	_pBtnCancel->callback( staticOnOkCancel, this );
    end();
	resizable( this );

	//controls all this stuff:
	_pCalCtrl = new CalendarController( *_pMoCho, *_pYCho, *_pDayTable );

	//make DayTable cells the right size:
	_pDayTable->col_width_all( COLWIDTH );
    _pDayTable->row_header_width( COLWIDTH );
	_pDayTable->row_height_all( ROWHEIGHT );
    _pDayTable->signalSelected.
            connect< Flx_Calendar, &Flx_Calendar::onDayTableSelected >( this );
}

/** sets the initially selected date */
void Flx_Calendar::setDate( const MyDate &date ) {
    _pCalCtrl->setDate( date );
}

/** sets the initially selected date */
void Flx_Calendar::setDate( const char *date ) {
	MyDate dt;
	MyDate::FromString( dt, date );
	setDate( dt );
}

/** shows this dialog at the given position */
MyDate Flx_Calendar::show( int x, int y ) {
	position( x, y );
    Fl_Double_Window::set_modal();
	Fl_Double_Window::show();
    
	while( shown() ) {
		Fl::wait();
	}
	return _selectedDate;
}

void Flx_Calendar::onDayTableSelected( Flx_Table &, SelectionEvent &evt ) {
    if( evt.doubleClicked ) {
        onOkCancel( true );
    }
    if( Fl::event_clicks() > 0 ) {
        onOkCancel( true );
    }
}

void Flx_Calendar::staticOnOkCancel( Fl_Widget *pWi, void *pUserdata ) {
	Flx_Calendar *pThis = (Flx_Calendar*)pUserdata;
	Fl_Button *pBtn = (Fl_Button*) pWi;
	pThis->onOkCancel( ( strcmp( pBtn->label(), "OK" ) == 0 ) ? true : false );
}

/** called by OkCancelGroup */
void Flx_Calendar::onOkCancel( bool ok ) {
	if ( ok ) {
		_pCalCtrl->getSelectedDate( _selectedDate );
    } else {
        _selectedDate = MyDate();
    }
	hide();
}


/** handles the dragging of this Flx_Calendar window */
int Flx_Calendar::handle(int event) {
    switch (event) {
        case FL_PUSH:
            _xMouse = Fl::event_x(), _yMouse = Fl::event_y();
            if (_yMouse < BAR_H) {
                _isDragging = true;
            }
            break;
        case FL_RELEASE:
            _isDragging = false;
            break;
        case FL_DRAG:
        {
            if (_isDragging) {
                int dx = Fl::event_x() - _xMouse;
                int dy = Fl::event_y() - _yMouse;
                this->position(x() + dx, y() + dy);
            }
            //return 1;
        }
            break;
        default:
            break;
    }
    return Fl_Double_Window::handle(event);
}

} //namespace flx
