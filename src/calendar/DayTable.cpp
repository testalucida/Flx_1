#include <src/calendar/DayTable.h>

namespace flx {

DayTable::DayTable( int x, int y, int w, int h ) : Flx_Table( x, y, w, h ) {
    _r = _c = -1;
    setSelectionMode( FLX_SELECTION_SINGLECELL );		
	//signalSelected.connect<DayTable, &DayTable::onSelectionChanged>( this );
    _weekendColor = fl_rgb_color( 193, 211, 255 );
	this->col_resize( 0 );
}

bool DayTable::canSelectCell( int row, int col ) {
	if( row < 0 || col < 0 ) return false; //

    const char* val = _pData->getValue( row, col );
    return val[0] > ' ' || val[1] > ' ';
}

/** overrides Flx_Table::getCellBackground(..) */
Fl_Color DayTable::getCellBackground( int row, int col, bool isSelected ) {
    if ( _rowToday == row && _colToday == col ) {
        return isSelected ? FL_YELLOW : FL_GRAY;
    }

    return isSelected ? FL_YELLOW :
            (col == 5 || col == 6) ? _weekendColor : FL_WHITE;
}

void DayTable::setToday( int row, int col ) { 
	clearSelection();
	_rowToday = row; _colToday = col; 
}

bool DayTable::isCellEditable(int row, int col) const {
    return false;
}


} //flx