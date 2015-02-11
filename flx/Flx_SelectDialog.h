#ifndef _FLX_SELECTDIALOG_H_
#define _FLX_SELECTDIALOG_H_

#include <cstdio>
#include <vector>
#include <flx/Flx_Dialog.h>
#include <my/TableData.h>

namespace flx {

class Flx_Table;

class Flx_SelectDialog : public Flx_Dialog {
public:
	Flx_SelectDialog( int x, int y, int w, int h, const char *pLabel = NULL );
	~Flx_SelectDialog();

	/** allow user select many items. 
	Default is single selection */
	void setMultiSelection( bool canSelectMulti );
	
	/** sets a vector of string as TableData.
	To display more sophistic objects you have to provide your own
	TableData (setData(const TableData&) )*/
	void setData( const std::vector<std::string> &items, const std::string &header );
	
	/** Data to select in the Flx_Table */
	void setData( my::TableData &data );

	/** returns the selected row's index.
	Use this method if single selection is specified (default) */
	int getSelectedRow();
	
	/** returns the selected rows' indexes.
	If multi selection is enabled, you have to use this methods for 
	obtaining proper results.
	In case of single selection you might prefer using getSelectedRow() */
	const std::vector<int> &getSelectedRows();

	void onSelectionCallback();
private:
	Flx_Table *_pTable;
	my::TableData *_pData;
	bool _mustDeleteTableData;
	std::vector<int> _selectedItems;
};


} //ns myfltk

#endif