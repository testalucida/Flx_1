/* 
 * File:   Flx_NumericInput.cpp
 * Author: max
 * 
 * Created on 20. Dezember 2013, 09:11
 */

#include <flx/Flx_NumericInput.h>

#include <FL/fl_draw.H>
#include <FL/Fl.H>

#include <flx/Flx_Input.h>
#include <flx/Flx_Box.h>
#include <flx/flx_signalparms.h>

namespace flx {
    
Flx_NumericInput::Flx_NumericInput(int X, int Y, int W, int H, const char *pLabel )
: Flx_Group( X, Y, W, H, pLabel ) 
{
    align( FL_ALIGN_LEFT );
    Flx_Group::box( FL_NO_BOX );
    _pBox = new Flx_Box( X,Y,W,H );
    _pBox->color( FL_WHITE );
    _pBox->box( FL_DOWN_BOX );
    _pInp = new Flx_Input( X,Y,W,H );
    _pInp->callback( Flx_NumericInput::staticCallback, this );
    _pInp->when( FL_WHEN_CHANGED );
    _pInp->hide();
    //_pInp->color( FL_CYAN );        // for debugging: comment out for normal use
    
    textfont( fl_font() );
	textsize( fl_size() );
	labelfont( fl_font() );
	labelsize( fl_size() );
    
    end();
  }    
   
void Flx_NumericInput::type( uchar t ) {
    _pInp->type( t );
}

void Flx_NumericInput::justify( Fl_Align val ) {
    _pBox->align(val|FL_ALIGN_INSIDE);
}

void Flx_NumericInput::value( const char *val ) {
    _pBox->copy_label(val);
    _pInp->value(val);
}

const char *Flx_NumericInput::value() const {
  return _pInp->value();
}

int Flx_NumericInput::handle( int e ) {
  switch (e) {
    case FL_PUSH:
    case FL_FOCUS:
      if (!_pInp->visible()) {
          _pBox->hide();
          _pInp->show();
          _pInp->value( _pBox->label() );
          redraw();
      }
      {
        bool focus = true;
        signalFocus.send( this, &focus );
      }
      break;
    case FL_UNFOCUS:
      if (_pInp->visible()) {
          _pBox->show();
          _pInp->hide();
          _pBox->label( _pInp->value() );
          redraw();
      }
      {
        bool focus = false;
        signalFocus.send( this, &focus );
      }
      break;
    default:
      break;
  }
  return( Flx_Group::handle( e ) );
}

Fl_Font Flx_NumericInput::textfont() {
    return _pInp->textfont();
}

void Flx_NumericInput::textfont( Fl_Font font ) {
    _pInp->textfont( font );
    _pBox->labelfont( font );
}

Fl_Color Flx_NumericInput::textcolor() const {
    return _pInp->textcolor();
}

void Flx_NumericInput::textcolor( Fl_Color color ) {
    _pInp->textcolor( color );
    _pBox->labelcolor( color );
}

Fl_Fontsize Flx_NumericInput::textsize() {
    return _pInp->textsize();
}

void Flx_NumericInput::textsize( Fl_Fontsize size ) {
    _pInp->textsize( size );
    _pBox->labelsize( size );
}

Fl_Font Flx_NumericInput::labelfont() {
    return /*_pInp->*/Flx_Group::labelfont();
}

void Flx_NumericInput::labelfont( Fl_Font font ) {
    /*_pInp->*/Flx_Group::labelfont( font );
    //_pBox->labelfont( font );
}

Fl_Fontsize Flx_NumericInput::labelsize() {
    return /*_pInp->*/Flx_Group::labelsize();
}

void Flx_NumericInput::labelsize( Fl_Fontsize size ) {
    /*_pInp->*/Flx_Group::labelsize( size );
    //_pBox->labelsize( size );
}
    
void Flx_NumericInput::staticCallback( Fl_Widget *, void *pUserdata ) {
    ((Flx_NumericInput*)pUserdata)->callback();
}

void Flx_NumericInput::callback() {
    ActionParm parm;
    signalChanged.send( this, &parm );
}

bool Flx_NumericInput::hasFocus() const {
    Fl_Widget *pW = Fl::focus();
    return ( pW == _pInp || pW == _pBox );
}

} //flx