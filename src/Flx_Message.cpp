#include <flx/Flx_Message.h>
#include <flx/flx_colors.h>
#include <flx/flx_boxtypes.h>

#include <FL/Fl.H>

namespace flx {

Fl_Pixmap  *Flx_Message::_pFailed_xpm = NULL;
Fl_Pixmap  *Flx_Message::_pStop_xpm = NULL;
Fl_Pixmap  *Flx_Message::_pWarning_xpm = NULL;
Fl_Pixmap  *Flx_Message::_pInfo_xpm = NULL;
Fl_Pixmap  *Flx_Message::_pQuestion_xpm = NULL;
Fl_Box *Flx_Message::_pPicBox = NULL;
Fl_Multiline_Output *Flx_Message::_pMsg = NULL;
Fl_Color Flx_Message::_color = fl_lighter( FL_GRAY ); //FLX_COLOR_GREEN;

Flx_Dialog *Flx_Message::_pDlg = Flx_Message::createDialog(); //new Flx_Dialog( 10, 10, 300, 200 );

Flx_Dialog *Flx_Message::createDialog() {
	_pFailed_xpm = new Fl_Pixmap( failed48_xpm );
	_pStop_xpm = new Fl_Pixmap( stop_xpm );
	_pWarning_xpm = new Fl_Pixmap( warning_xpm );
	_pInfo_xpm = new Fl_Pixmap( info_xpm );
    _pQuestion_xpm = new Fl_Pixmap( question_xpm );
	
	_pDlg = new Flx_Dialog( 0, 0, 350, 160 );
	_pDlg->box( FLX_GRADIENT_BOX );
	_pDlg->color( _color );
	int x, y, w, h;
	_pDlg->getClientArea( x, y, w, h );
	_pPicBox = new Fl_Box( x+10, y+10, 50, 50 );
	_pDlg->add( _pPicBox );
	x = _pPicBox->x() + _pPicBox->w() + 10;
	y = _pPicBox->y();
	_pMsg = new Fl_Multiline_Output( x, y, w - x - 5, h - y );
	_pMsg->box( FL_DOWN_BOX );
	//_pMsg->color( FL_BACKGROUND_COLOR );
	_pMsg->color( _color );
	_pMsg->wrap( 1 );
	_pDlg->add( _pMsg );
	_pDlg->resizable( _pMsg );
	return _pDlg;
}

void Flx_Message::failed( const char *pMsg ) {
	_pDlg->label( "Fehler!" );
	_pPicBox->image( _pFailed_xpm );
	_pMsg->value( pMsg );
	_pDlg->position( Fl::event_x_root(), Fl::event_y_root() );
	_pDlg->show( true );
}

void Flx_Message::stop( const char *pMsg ) {
	_pDlg->label( "STOPP!" );
	_pPicBox->image( _pStop_xpm );
	_pMsg->value( pMsg );
	_pDlg->position( Fl::event_x_root(), Fl::event_y_root() );
	_pDlg->show( true );
}

void Flx_Message::warning( const char *pMsg ) {
	_pDlg->label( "Warnung" );
	_pPicBox->image( _pWarning_xpm );
	_pMsg->value( pMsg );
	_pDlg->position( Fl::event_x_root(), Fl::event_y_root() );
	_pDlg->show( true );
}

void Flx_Message::info( const char *pMsg ) {
	_pDlg->label( "Info" );
	_pPicBox->image( _pInfo_xpm );
	_pMsg->value( pMsg );
	_pDlg->position( Fl::event_x_root(), Fl::event_y_root() );
	_pDlg->show( true );
}

int Flx_Message::ask( const char *pTitle, const char *pQuestion ) {
    _pDlg->label( pTitle );
    _pDlg->setButtonLabels( "Ja", "Nein" );
    _pPicBox->image( _pQuestion_xpm );
    _pMsg->value( pQuestion );
    _pDlg->position( Fl::event_x_root(), Fl::event_y_root() );
	return _pDlg->show( true );
    
}

void Flx_Message::setColor( Fl_Color color ) {
	_pDlg->color( color );
}

}