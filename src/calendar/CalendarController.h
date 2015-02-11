#ifndef _CALENDARCONTROLLER_H_
#define _CALENDARCONTROLLER_H_

#include <src/calendar/DayTableModel.h>

namespace my {
	class MyDate;
}

namespace flx {

class DayTable;
class Flx_Choice;
struct ActionParm;

class CalendarController {
public:
	CalendarController( Flx_Choice &months, Flx_Choice &years, DayTable &tbl );
	~CalendarController(void);
	void onMonthYearChanged( Flx_Choice &, ActionParm & );
	void setDate( const my::MyDate & );
    void getSelectedDate( my::MyDate & );
private:
	/** updates day table by the given date (nn, yyyy, dd) */
	void setDays( int month, int year, int day );
private:
	Flx_Choice &_months;
	Flx_Choice &_years;
	DayTable &_dayTbl;
	DayTableModel _daysModel;
};

} //flx


#endif
