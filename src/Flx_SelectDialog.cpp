#include <flx/Flx_SelectDialog.h>
#include "flx/Flx_Table.h"


using namespace std;
using namespace my;

namespace flx {

Flx_SelectDialog::Flx_SelectDialog( int x, int y, int w, int h, const char *pLabel ) :
		Flx_Dialog( x, y, w, h, pLabel ? pLabel : "Eintrag auswählen" ),
		_pData( NULL ),
		_mustDeleteTableData( false )
{
	begin();
		int X, Y, W, H;
		getClientArea( X, Y, W, H );
		_pTable = new Flx_Table( X, Y, W, H );
		_pTable->setSelectionMode( FLX_SELECTION_SINGLEROW );
		//_pTable->setSelectionCallback( bind( &Flx_SelectDialog::onSelectionCallback, this ) );
	end();
	_pBtnOk->deactivate();
	resizable( _pTable );
}

Flx_SelectDialog::~Flx_SelectDialog() {
	if( _mustDeleteTableData ) {
		delete _pData;
	}
}

void Flx_SelectDialog::onSelectionCallback() {
	if( _pTable->getSelectedCells().empty() ) {
		_pBtnOk->deactivate();
	} else {
		_pBtnOk->activate();
	}
}

void Flx_SelectDialog::setMultiSelection( bool canSelectMulti ) {
	_pTable->setSelectionMode( canSelectMulti ?  FLX_SELECTION_MULTIROW : FLX_SELECTION_SINGLEROW );
}

void Flx_SelectDialog::setData( const vector<string> &items, const string &header ) {
	_pData = new TableData();
	_pData->addColumn( header.c_str() );
	for( int r = 0, rmax = items.size(); r < rmax; r++ ) {
		_pData->addRow();
		_pData->setValue( items[r], r, 0 );
	}
	_pTable->setTableData( *_pData );
	_mustDeleteTableData = true;
}

void Flx_SelectDialog::setData( TableData &data ) {
	_pTable->setTableData( data );
}

int Flx_SelectDialog::getSelectedRow() {
	return _pTable->getSelectedRows()[0];
}

const vector<int> &Flx_SelectDialog::getSelectedRows() {
	_selectedItems.clear();
	return _pTable->getSelectedRows();
	return _selectedItems;
}


} //ns myfltk