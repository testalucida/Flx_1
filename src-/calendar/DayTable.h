#ifndef _DAYTABLE_H_
#define _DAYTABLE_H_

#include <flx/Flx_Table.h>

namespace flx {

class DayTable : public Flx_Table {
public:
    DayTable( int x, int y, int w, int h ) ;

	/** don't select cells without values */
	bool canSelectCell( int row, int col );
	/** overrides MyTable::cellBackground(..) */
    Fl_Color getCellBackground( int row, int col, bool isSelected );
	/** for background color purposes: set today's row and col index.
	    Might be -1 if a month other than the current one is to be
		shown. */
	void setToday( int row, int col );
protected:
    bool isCellEditable(int row, int col) const;
private:
	//DayTableModel *_pDtm;
	int _rowToday, _colToday;
    int _r, _c; //remember last selected cell
	Fl_Color _weekendColor;
};

} //flx

#endif