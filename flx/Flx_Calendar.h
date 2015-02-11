#ifndef _FLX_CALENDAR_H_
#define _FLX_CALENDAR_H_

#include <FL/Fl_Double_Window.H>

#include <my/TableData.h>
#include <my/datetime.h>
#include <flx/Flx_Table.h>
//#include <FL/Fl_Table.H>

namespace flx {
	class DayTable;
	class CalendarController;
	class Flx_Choice;
	class Flx_Button;
	class Flx_ReturnButton;
}

//class Fl_Choice;
//class Fl_Button;
//class Fl_Return_Button;

namespace flx {

/**
	A calendar widget for selecting a date.

	Use it like this:

	#include "flx/calendar/mycalendar.hpp"

	class Test {
		MyCalendar *pCal;
	public:

		Test() {       
			...
			pCal = new MyCalendar();
			pCal->callbackOnSelection(bind(&Test::onSelection, this, _1));
			
			MyDate dte( 1, 1, 2012 ); 
			pCal->date( dte ); //initialize the calendar widget with 1.1.2012
			//if you omit the 2 lines above, the widget will be preset with
			//current date

			pCal->show( 200, 200 );

		}

		void onSelection( MyDate &date ) {
			const char *ps = pCal->date().ToEurString().c_str();
			//const string &s = datum.ToEurString();
			fprintf(stderr, "DATE SELECTED: %s\n", ps);
		}
	};

*/
class Flx_Calendar : public Fl_Double_Window {
public:

    Flx_Calendar(int x=0, int y=0);

	/** sets the initially selected date */
    void setDate(const my::MyDate &date);

	/** sets the initially selected date */
	void setDate( const char *date );

	/** shows this dialog at the given position */
	my::MyDate show( int x, int y );
    
    void onDayTableSelected( Flx_Table &, SelectionEvent & );

protected:
	/** handles the dragging of this MyCalendar window */
    int handle(int event);

private:
    /** called by Ok and Cancel buttons */
    static void staticOnOkCancel( Fl_Widget *pWi, void* pUserdata ) ;
	void onOkCancel( bool ok );

private:
	Flx_ReturnButton *_pBtnOk;
	Flx_Button  *_pBtnCancel;
    Flx_Choice *_pMoCho;
    Flx_Choice *_pYCho;
    int _xMouse, _yMouse;
    bool _isDragging;
    my::MyDate _selectedDate;
	DayTable *_pDayTable;
    //DayTableModel *_pDayModel;
    //ONSELECTION _callback_on_selection;
	CalendarController *_pCalCtrl;
};

} //namespace flx

#endif