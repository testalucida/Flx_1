#include <flx/Flx_Dialog.h>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>

namespace flx {

Flx_Dialog::Flx_Dialog( int x, int y, int w, int h, const char *pTitle ) :
	Fl_Double_Window( x, y, w, h, pTitle ),
	_labelSize( 11 ),
	_ok( false ),
    _pFocusOnShow( NULL )
{
	begin();
		createLayout();
	end();
}

void Flx_Dialog::createLayout() {
	int buttonheight = 25;
	int vmargin = 10;
	int hmargin = 5;
	int okcancelheight = buttonheight + 2*vmargin;
	
	_pGrp = new Fl_Group( 0, this->h() - okcancelheight, this->w(), okcancelheight );	
	_pGrp->begin();
		_pBtnOk = new Fl_Return_Button( hmargin, _pGrp->y() + vmargin, 75, 
                                        buttonheight, "OK" );
		_pBtnOk->labelsize( _labelSize );
		_pBtnOk->callback( staticOkCancel_cb, this );

		_pBtnCancel = new Fl_Button( _pBtnOk->x() + _pBtnOk->w() + hmargin,
										_pBtnOk->y(), 
										_pBtnOk->w(), 
										_pBtnOk->h(), 
										"Abbrechen" );
		_pBtnCancel->labelsize( _labelSize );
		_pBtnCancel->callback( staticOkCancel_cb, this );

		//keep the buttons of same width when resizing the dialog:
		Fl_Box* pStretch = new Fl_Box( _pBtnCancel->x() + _pBtnCancel->w(),
									    _pBtnCancel->y(), 
										this->w() - ( _pBtnCancel->x() + _pBtnCancel->w() ),
										1 );
	_pGrp->end();
	_pGrp->resizable( pStretch );
}

int Flx_Dialog::show( bool modally ) {
	if( modally ) {
		Fl_Double_Window::set_modal();
	} else {
		Fl_Double_Window::set_non_modal();
	}
    
    if( _pFocusOnShow ) {
        _pFocusOnShow->take_focus();
    }
    
	Fl_Double_Window::show();

	while( shown() ) {
		Fl::wait();
	}
	return _ok ? 1 : 0;
}

void Flx_Dialog::getClientArea( int& x, int& y, int& w, int& h ) {
	x = 0;
	y = 0;
	w = this->w();
	h = this->h() - _pGrp->h();
}

void Flx_Dialog::add( Fl_Widget &w ) {
//    _pGrp->add( w );
    Fl_Double_Window::add( w );
}

void Flx_Dialog::add( Fl_Widget *pW ) {
    //_pGrp->add( pW );
    Fl_Double_Window::add( pW );
}

void Flx_Dialog::setFocusOnShow( Fl_Widget &widget ) {
    _pFocusOnShow = &widget;
}

bool Flx_Dialog::validate() {
	return true;
}

void Flx_Dialog::staticOkCancel_cb( Fl_Widget* pWdgt, void* pUserData ) {
	Flx_Dialog* pDlg = (Flx_Dialog*)pUserData;
	pDlg->okCancel_cb( (Fl_Button*)pWdgt );
}

void Flx_Dialog::okCancel_cb( Fl_Button* pBtn ) {
    if( pBtn == _pBtnOk  ) {
        if( validate() ) {
            _ok = true;
            hide();
        }
    } else {
        _ok = false;
        hide();
    }
}

} //ns myfltk