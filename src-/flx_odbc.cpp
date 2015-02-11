#include "flx/flx_odbc.h"
#include "flx/Flx_Message.h"

#include <my/odbcexception.hpp>
#include <my/CharBuffer.h>

#include <FL/fl_ask.H>

using namespace std;
using namespace my;

namespace flx {

Flx_ConnectDialog::Flx_ConnectDialog( int x, int y ) :
	Flx_Dialog( x, y, 220, 140, "Datenbankverbindung aufbauen" )
{
	int X, Y, w, h;
	int xmargin = 93;
	int wiwi = 120;
	getClientArea( X, Y, w, h );
	_pUserId = new Fl_Input( X+xmargin, Y+5, wiwi, 23, "UserId: " );
	add( _pUserId );
	_pPwd = new Fl_Secret_Input( X+xmargin, _pUserId->y() + _pUserId->h() + 5, wiwi, 23, "Passwort: " );
	add( _pPwd );
	_pDsn = new Fl_Choice( X+xmargin, _pPwd->y() + _pPwd->h() + 5, wiwi, 23, "Datenquelle: " );
	add( _pDsn );
	end();
	_pPwd->take_focus();
}

void Flx_ConnectDialog::setModel( Flx_ConnectData &model ) {
	_pModel = &model;
	_pUserId->value( _pModel->userId.c_str() );
	_pPwd->value( _pModel->password.c_str() );
	//provide choice-list:
	if( !_pModel->pSystemDsnList || _pModel->pSystemDsnList->size() == 0 ) return;

	const char *pDsnPreset =_pModel->dsn.c_str();
	int idxPreset = -1, i = 0;
	for_each( _pModel->pSystemDsnList->begin(), _pModel->pSystemDsnList->end(), 
			  [&]( RegValuePtr pRegValue ) { 
				  const char *pDsn = pRegValue->valueName.c_str();
				  _pDsn->add( pDsn );
				  if( pDsnPreset && idxPreset < 0 && !strcmp( pDsn, pDsnPreset ) ) {
					  idxPreset = i;
				  }
				  i++;
			  } );
	_pDsn->value( idxPreset < 0 ? 0 : idxPreset );
}

bool Flx_ConnectDialog::validate() {
	if( _pDsn->text() && strlen( _pDsn->text() ) > 0 ) {
		_pModel->userId = _pUserId->value();
		_pModel->password = _pPwd->value();
		_pModel->dsn = _pDsn->text();
		return true;
	} else {
		//fl_alert( "DSN muss angegeben werden." );
		Flx_Message::failed( "DSN muss angegeben werden." );
		return false;
	}
}

///////////////////////////////////////////////////////////////

Flx_Odbc::Flx_Odbc() : Odbc(), _pDlg( NULL ) {}
Flx_Odbc::~Flx_Odbc() {
	if( _pDlg ) {
		delete _pDlg;
	}
}

void Flx_Odbc::connect( const char *pDsn, const char *pUserId, const char *pPwd, bool forceDialog ) {
	/** get all System DSN from the Registry */
	//_model.pSystemDsnList = _reg.getSystemDSN();
	_model.pSystemDsnList = _reg.getSystemIBM_DSN();
	
	if( pDsn ) {
		_model.dsn.clear();
		_model.dsn.append( pDsn );
	}
	if( !pUserId ) {
		pUserId = getenv( "USERNAME" );
	}
	if( pUserId ) {
		_model.userId.clear();
		_model.userId.append( pUserId );
	}
	_model.password.clear();
	if( pPwd ) {
		_model.password.append( pPwd );
	}

	tryConnect( forceDialog );
}

void Flx_Odbc::readSystemDsn() {
	RegValuesPtr pValues = _reg.getSystemDSN();
}

void Flx_Odbc::tryConnect( bool forceDialog ) {
	if( forceDialog || _model.dsn.empty() ||
		( !_model.userId.empty() && _model.password.empty() ) )
	{
		if( !_pDlg ) {
			_pDlg = new Flx_ConnectDialog( 100, 100 );
			_pDlg->setModel( _model );
		}
		
		if( _pDlg->show( true ) ) {
			try {
				Odbc::connect( _model.dsn.c_str(), _model.userId.c_str(), _model.password.c_str() );
				doCallbacks();
			} catch( MyOdbcException ex ) {
				if( fl_ask( "%s\n\nNochmal versuchen?\n", ex.what() ) ) {
					tryConnect( true );
				}
			}
		}
	} else {
		//all parms are given, connect directly:
		Odbc::connect( _model.dsn.c_str(), _model.userId.c_str(), _model.password.c_str() );
		doCallbacks();
	}
}

void Flx_Odbc::doCallbacks() {
	if( _callbacks.size() > 0 ) {
		for( _cbItr = _callbacks.begin(); _cbItr != _callbacks.end(); _cbItr++ ) {
			(*_cbItr)( _model.dsn.c_str(), _model.userId.c_str() );
		}
	}
}

void Flx_Odbc::addConnectionCallback( CONNECTION_CALLBACK cb ) {
	_callbacks.push_back( cb );
}

DataTablePtr Flx_Odbc::select( const string &sql ) {
	try {
		return Odbc::select( sql );
	} catch( MyOdbcException ex ) {
		reconnectOrThrow( ex );
		return Odbc::select( sql );
	}
}

DataTablePtr Flx_Odbc::select( const CharBuffer &sql ) {
	try {
		return Odbc::select( sql );
	} catch( MyOdbcException ex ) {
		reconnectOrThrow( ex );
		return Odbc::select( sql );
	}
}

void Flx_Odbc::select( const string &sql, DataTablePtr pData ) {
	try {
		Odbc::select( sql, pData );
	} catch( MyOdbcException ex ) {
		reconnectOrThrow( ex );
		Odbc::select( sql, pData );
	}
}

void Flx_Odbc::select( const string &sql, TableData &data ) {
	try {
		Odbc::select( sql, data );
	} catch( MyOdbcException ex ) {
		reconnectOrThrow( ex );
		Odbc::select( sql, data );
	}
}

int Flx_Odbc::execute( const string &sql ) {
	try {
		return Odbc::execute( sql );
	} catch( MyOdbcException ex ) {
		reconnectOrThrow( ex );
		return Odbc::execute( sql );
	}
}

void Flx_Odbc::reconnectOrThrow( my::MyOdbcException &ex ) {
	CharBuffer msg( ex.what() );
	if( msg.contains( "30081" ) ) {
		//connection de-established due to exceeding idle time 
		reconnect();
	} else {
		throw( ex );
	}
}

} //nsp flx