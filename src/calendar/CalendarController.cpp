#include <src/calendar/CalendarController.h>
#include <src/calendar/strings.h>
#include <src/calendar/DayTable.h>

#include <flx/Flx_Choice.h>

#include <my/datetime.h>
#include <my/convert.h>
#include <my/CharBuffer.h>

#include <vector>

using namespace my;
using namespace std;

namespace flx {

CalendarController::CalendarController( Flx_Choice &m, Flx_Choice &y, DayTable &tbl ) 
	: _months( m )
	, _years( y )
	, _dayTbl( tbl )
{
	for( int i = 0; months[i] != ""; i++ ) {
		_months.add( months[i].c_str() );
	}
	_months.signalSelected.
		connect< CalendarController, &CalendarController::onMonthYearChanged >( this );

	for( int i = 0; years[i] != ""; i++ ) {
		_years.add( years[i].c_str() );
	}
	_years.signalSelected.
		connect< CalendarController, &CalendarController::onMonthYearChanged >( this );
	_dayTbl.setTableData( _daysModel );

	MyDate today( true );
	setDate( today );

}


void CalendarController::onMonthYearChanged( Flx_Choice &, ActionParm & ) {
    int month = _months.value() + 1;
    int year = _years.value() + 2010;
    MyDate date( 1, month, year );
    setDate( date );
}

void CalendarController::setDate( const MyDate &dt ) {
	setDays( dt.GetMonth(), dt.GetYear(), dt.GetDay() );
    _months.value( dt.GetMonth() - 1 );
    _years.value( dt.GetYear() - 2010 );
	//select new day:
	_dayTbl.clearSelection();
	int day = dt.GetDay();
	CharBuffer cbDay;
	if( day < 10 ) {
		cbDay.add( " " );
	}
	cbDay.addInt( day );

	int row, col;
	_daysModel.search( cbDay.get(), row, col );
	_dayTbl.select( row, col );
}


void CalendarController::setDays( int month, int year, int day ) {
    int nDays = MyDate::GetNumberOfDays( month, year );
	MyDate showDate;
    showDate.SetDay( day );
    showDate.SetMonth( month );
    showDate.SetYear( year );

	int firstWeekday;
    if ( day == 1 ) {
        firstWeekday = showDate.GetWeekday( ); //0->sunday; 6->saturday
    } else {
        MyDate firstofmonth( 1, month, year );
        firstWeekday = firstofmonth.GetWeekday( );
    }
    //we want monday to have index 0:
    firstWeekday--;
    firstWeekday = ( firstWeekday < 0 ) ? 6 : firstWeekday;

    //number of rows to display
	int rowCount;
    if ( firstWeekday > 5 || ( firstWeekday > 4 && nDays == 31) ) {
        rowCount = 6;
    } else {
        rowCount = 5;
    }

	_daysModel.clearDays();
	int startC = firstWeekday;
	char dayNr[3];
	for( int r = 0, d = 1; r < rowCount && d <= nDays; r++ ) {
		for( int c = startC; c < 7 && d <= nDays; c++, d++ ) {
			sprintf( dayNr, "%2d", d );
			_daysModel.setValue( dayNr, r, c );
		}
		startC = 0;
	}

	MyDate today( true );
	int rowCurDate, colCurDate;
    if ( showDate.GetMonth( ) == today.GetMonth( ) &&
         showDate.GetYear( ) == today.GetYear( ) ) 
	{
        //the day table of the actual month is to be shown
        //calculate position of current date:
        int idx = firstWeekday + today.GetDay( ) - 1;
        colCurDate = idx % 7;
        rowCurDate = (idx - colCurDate) / 7;
    } else {
        //show another month than the actual one
        colCurDate = rowCurDate = -1;
    }
	_dayTbl.setToday( rowCurDate, colCurDate );
}

void CalendarController::getSelectedDate( my::MyDate &date ) {
    CellPtr pCell = _dayTbl.getSelectedCells()[0];
    const char *pDayNr = _daysModel.getValue( pCell->row, pCell->col );
    int dayNr = Convert::ToInt( pDayNr );
    int month = _months.value() + 1;
    int year = _years.value() + 2010;
    date.SetDate( dayNr, month, year );
}

CalendarController::~CalendarController(void)
{
}

} //flx
