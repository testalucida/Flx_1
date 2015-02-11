
#include <src/calendar/DayTableModel.h>

using namespace my;

namespace flx {

DayTableModel::DayTableModel( ) {
	//create 7 columns (day's names) and 6 rows (day numbers 1-31)
	for( int c = 0; c < 7; c++ ) {
		addColumn( dayNames[c].c_str() );
	}
	addRows( 6 );
}

void DayTableModel::clearDays() {
	for( int r = 0, rmax = getRowCount(); r < rmax; r++ ) {
		for( int c = 0, cmax = getColumnCount(); c < cmax; c++ ) {
			setValue( " ", r, c );
		}
	}
}

const char* DayTableModel::getRowHeader( int row ) const {
    return "";
}

DayTableModel::~DayTableModel( ) {
    //deleteDays( );
}

} //flx