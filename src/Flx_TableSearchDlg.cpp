#include <flx/Flx_TableSearchDlg.h>

#include <flx/Flx_Input.h>
#include <flx/flx_boxtypes.h>
#include <flx/Flx_CheckButton.h>
#include <flx/Flx_Button.h>
#include <flx/Flx_Output.h>
#include <flx/flx_colors.h>

using namespace my;

namespace flx {

Flx_TableSearchDlg::Flx_TableSearchDlg( int x, int y, const char *pTitle )
	: Flx_Window( x, y, 310, 195, pTitle )
{
	const int H = 23;
	const int vmargin = 10;
//	box( FLX_GRADIENT_BOX );
//	color( FLX_COLOR_LIGHT_GREEN );
    box( FL_FLAT_BOX );
    color( fl_rgb_color( 240, 240, 240 ) );
	begin();
	
	_pSearchString = new Flx_Input( 85, vmargin, 210, H, "Suchbegriff: " );
	_pSearchString->align( FL_ALIGN_LEFT );
	_pSearchString->signalEnterPressed.connect < Flx_TableSearchDlg, &Flx_TableSearchDlg::onSearchStringEnter >( this );

	_pOptionMatchCase = new Flx_CheckButton( _pSearchString->x(), 
										_pSearchString->y() + _pSearchString->h() + vmargin,
										200, _pSearchString->h(),
										"Groß-/Kleinschreibung beachten" );
	_pOptionMatchWord = new Flx_CheckButton( _pSearchString->x(),
										_pOptionMatchCase->y() + _pOptionMatchCase->h() + vmargin,
										200, _pSearchString->h(),
										"Nur ganze Wörter vergleichen" );

	_pBtnPrev = new Flx_Button( _pSearchString->x(), _pOptionMatchWord->y() + _pOptionMatchWord->h() + 2*vmargin, 25, 25, "@<" );
	_pBtnPrev->color( fl_rgb_color( 210, 210, 210 ) );
	_pBtnPrev->tooltip(  "Suche rückwärts" );
	_pBtnPrev->signalSelected.connect< Flx_TableSearchDlg, &Flx_TableSearchDlg::onButtonClick >( this );
	
	_pBtnNext = new Flx_Button( _pBtnPrev->x()+30, _pBtnPrev->y(), _pBtnPrev->w(), _pBtnPrev->h(), "@>" );
	_pBtnNext->color( fl_rgb_color( 210, 210, 210 ) );
	_pBtnNext->tooltip(  "Suche vorwärts" );
	_pBtnNext->signalSelected.connect< Flx_TableSearchDlg, &Flx_TableSearchDlg::onButtonClick >( this );

	_pBtnCancel = new Flx_Button( 220, _pBtnPrev->y(), 75, _pBtnPrev->h(), "Abbrechen" ); 
	_pBtnCancel->color( fl_rgb_color( 210, 210, 210 ) );
	_pBtnCancel->tooltip(  "Suche abbrechen" );
	_pBtnCancel->signalSelected.connect< Flx_TableSearchDlg, &Flx_TableSearchDlg::onButtonClick >( this );

	_pMessageOut = new Flx_Output( 0, h() - H, w(), H );
	_pMessageOut->align( FL_ALIGN_LEFT | FL_ALIGN_CENTER );
	//_pMessageOut->box( FLX_GRADIENT_BORDER_DOWN_BOX );
	//_pMessageOut->color( FLX_COLOR_GREEN );
    _pMessageOut->box( FL_THIN_DOWN_BOX );
	_pMessageOut->color( fl_rgb_color( 210, 210, 210 ) );
	_pMessageOut->value( "Bitte Suchbegriff eingeben und Suche starten." );
	end();
}

void Flx_TableSearchDlg::onButtonClick( Flx_Button &btn, ActionParm &parm ) {
	SearchParm searchparm( false, 
						   _pSearchString->value(), 
						   (_pOptionMatchCase->value() == 1) ? true : false, 
						   (_pOptionMatchWord->value() == 1) ? true : false );
	if( &btn == _pBtnPrev ) {
		signalSearch.send( *this, searchparm );
	} else if( &btn == _pBtnNext ) {
		searchparm.searchForward = true;
		signalSearch.send( *this, searchparm );
	} else {
		Dummy dummy;
		signalCancel.send( *this, dummy );
	}
}

void Flx_TableSearchDlg::onSearchStringEnter( Flx_Input &, ActionParm &parm ) {
	//onButtonClick( *_pBtnNext, parm );
	_pBtnNext->simulateEnter();
}

void Flx_TableSearchDlg::setMessage( const char *pMessage ) {
	_pMessageOut->value( pMessage );
}

Flx_TableSearchDlg::~Flx_TableSearchDlg(void) {
}

} //flx
