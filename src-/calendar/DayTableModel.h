#ifndef _DAYTABLEMODEL_H_
#define _DAYTABLEMODEL_H_

#include <my/TableData.h>

namespace flx {

static string dayNames[] = {
	"Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
};

class DayTableModel : public my::TableData {
public:

    DayTableModel();
	~DayTableModel();

	void clearDays();
    virtual const char* getRowHeader( int row ) const;

private:
 
};

} //flx

#endif