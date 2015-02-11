#pragma once

#include <flx/Flx_Dialog.h>

#include <my/odbc.h>
#include <my/RegistryHelper.h>

#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Choice.H>

#include <functional>

namespace flx {

////////////////////////////////////////////////
////////////////////////////////////////////////

class Flx_ConnectData {
public:
	std::string userId;
	std::string password;
	/** default DSN */
	std::string dsn;
	/** all available System DSN */
	my::RegValuesPtr pSystemDsnList;
};

////////////////////////////////////////////////
////////////////////////////////////////////////

class Flx_ConnectDialog : public Flx_Dialog {
public:
	Flx_ConnectDialog( int x, int y );
	void setModel( Flx_ConnectData &model );
protected:
	bool validate();
private:
	Fl_Input *_pUserId;
	Fl_Secret_Input *_pPwd;
	Fl_Choice *_pDsn;
	Flx_ConnectData *_pModel;
};

////////////////////////////////////////////////
////////////////////////////////////////////////

typedef std::function<void (const char *pDsn, const char *pUserId )> 
		CONNECTION_CALLBACK;

class Flx_Odbc : public my::Odbc {
public:
	Flx_Odbc();
	~Flx_Odbc();
	/**
	   Establishs a database connection.
	   Shows the Flx_ConnectDialog if necessary (see below)
	   @param pDsn: Name of the DataSource to connect to. 
	                If not specified the Flx_ConnectDialog will be shown.
	   @param pUserId: UserID. If pUserId isn't specified the logged on user's name
	                will be provided.
	   @param pPwd: Password. If pUserId is set and pPwd is NULL the Flx_ConnectDialog 
	                will be shown.
	*/
	void connect( const char *pDsn = NULL, const char *pUserId = NULL, const char *pPwd = NULL, bool forceDialog = true );
	void addConnectionCallback( CONNECTION_CALLBACK );


	my::DataTablePtr select( const string &sql );
	my::DataTablePtr select( const my::CharBuffer &sql );
	void select( const string &sql, my::DataTablePtr );
	void select( const string &sql, my::TableData & );
	int execute( const string &sql );


private:
	void readSystemDsn();
	void tryConnect( bool forceDialog );
	void doCallbacks();
	void reconnectOrThrow( my::MyOdbcException & );
protected:
	Flx_ConnectDialog *_pDlg;
private:
	Flx_ConnectData _model;
	my::Registry _reg;
	std::vector<CONNECTION_CALLBACK> _callbacks;
	std::vector<CONNECTION_CALLBACK>::iterator _cbItr;
};

} //nsp flx