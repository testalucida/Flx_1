#ifndef _FLX_TABLESEARCH_
#define _FLX_TABLESEARCH_

#include <my/Signal.hpp>

namespace my {
	class TableDataSearch;
	struct TableDataSearchHit;
	struct SearchParm;
}

namespace flx {

class Flx_Table;
class Flx_TableSearchDlg;
struct Dummy;

class Flx_TableSearch {
public:
	my::Signal< Flx_TableSearch, my::TableDataSearchHit > signalHit;
public:
	Flx_TableSearch( Flx_Table &tbl );
	~Flx_TableSearch();
	/** starts a new search by opening the Flx_TableSearchDlg */
	void start();
	/** callback used by control buttons of Flx_TableSearchDlg */
	void onSearch( Flx_TableSearchDlg &dlg, my::SearchParm &parm );
	/** callback used by cancel button of Flx_TableSearchDlg */
	void onCancel( Flx_TableSearchDlg &dlg, Dummy & );
	/** callback used by TableDataSearch */
	void onHit( my::TableDataSearch &, my::TableDataSearchHit & );
	/** callback used by TableDataSearch when search is finished*/
	void onSearchTerminated( my::TableDataSearch &, my::TableDataSearchHit & );
private:
	void search( const my::SearchParm &parm );
private:
	Flx_Table &_table;
	Flx_TableSearchDlg *_pDlg;
	my::TableDataSearch &_dataSearch;
};

}//flx


#endif