#ifndef _FLX_TABLESEARCHDLG_
#define _FLX_TABLESEARCHDLG_

#include <flx/Flx_Window.h>
#include <flx/flx_signalparms.h>

#include <my/TableDataSearch.h>
#include <my/Signal.hpp>

namespace flx {

class Flx_Input;
class Flx_Button;
class Flx_CheckButton;
class Flx_Output;

struct Dummy {
};

class Flx_TableSearchDlg : public Flx_Window {
public:
	my::Signal< Flx_TableSearchDlg, my::SearchParm > signalSearch;
	my::Signal< Flx_TableSearchDlg, Dummy > signalCancel;
public:
	Flx_TableSearchDlg( int x, int y, const char *pTitle = 0 );
	~Flx_TableSearchDlg(void);
	void onButtonClick( Flx_Button &btn, ActionParm &parm );
	void onSearchStringEnter( Flx_Input &, ActionParm & );
	void setMessage( const char *pMessage );
private:
	Flx_Input *_pSearchString;
	Flx_CheckButton *_pOptionMatchCase;
	Flx_CheckButton *_pOptionMatchWord;
	Flx_Button *_pBtnPrev;
	Flx_Button *_pBtnNext;
	Flx_Button *_pBtnCancel;
	Flx_Output *_pMessageOut;
};

} //flx;

#endif

