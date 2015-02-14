#include <flx/Flx_TableSearch.h>
#include <flx/Flx_Table.h>
#include <flx/Flx_TableSearchDlg.h>

#include <my/TableDataSearch.h>
#include <my/convert.h>
#include <my/StringHelper.h>

#include <cstdio>

using namespace my;

namespace flx {

Flx_TableSearch::Flx_TableSearch( Flx_Table &tbl ) 
	: _table( tbl )
	, _pDlg( NULL )
	, _dataSearch( *(new TableDataSearch( tbl.getTableData() ) ) )
{
	_dataSearch.signalHit.connect < Flx_TableSearch, &Flx_TableSearch::onHit >( this );
	_dataSearch.signalSearchTerminated.connect < Flx_TableSearch, &Flx_TableSearch::onSearchTerminated >( this );
}

void Flx_TableSearch::start() {
	if( !_pDlg ) {
		_pDlg = new Flx_TableSearchDlg( 100, 100, "Suche nach Zell-Inhalten" );
		_pDlg->signalSearch.connect< Flx_TableSearch, &Flx_TableSearch::onSearch >( this );
		_pDlg->signalCancel.connect< Flx_TableSearch, &Flx_TableSearch::onCancel >( this );
	}
    
    _pDlg->position( Fl::event_x_root(), Fl::event_y_root() );
	_pDlg->show();
}

void Flx_TableSearch::onSearch( Flx_TableSearchDlg &dlg, SearchParm &parm ) {
	if( !(parm.pSearchString) || strlen( parm.pSearchString ) == 0 ) {
		_pDlg->setMessage( "Bitte Suchbegriff eingeben." );
		return;
	}
	_dataSearch.search( parm );
}

void Flx_TableSearch::onCancel( Flx_TableSearchDlg &dlg, Dummy & ) {		
	_pDlg->hide();
	_table.unmark();
	return;
}

void Flx_TableSearch::onHit( TableDataSearch &, TableDataSearchHit &hit ) {
	_table.mark( hit.row, hit.col );
	string msg( "Zeile " );
	msg.append( *Convert::ToString( hit.row ) );
	msg.append( ", Spalte " );
	msg.append( *Convert::ToString( hit.col ) );
	msg.append( ": " );
	msg.append( hit.pSubStr );
	_pDlg->setMessage( msg.c_str() );
}

void Flx_TableSearch::onSearchTerminated( TableDataSearch &, TableDataSearchHit &hit ) {
	_table.unmark();
	string msg( "Suche beendet." );
	_pDlg->setMessage( msg.c_str() );
}

Flx_TableSearch::~Flx_TableSearch() {
	if( _pDlg ) 
		delete _pDlg;
}


}//flx
