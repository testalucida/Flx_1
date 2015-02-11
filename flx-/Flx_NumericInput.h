/* 
 * File:   Flx_NumericInput.h
 * Author: max (concept and inital implementation by Greg Ercolano)
 *
 * Created on 20. Dezember 2013, 09:11
 */

#ifndef FLX_NUMERICINPUT_H
#define	FLX_NUMERICINPUT_H

#include <flx/Flx_Group.h>

#include <my/Signal.hpp>

class Fl_Widget;

namespace flx {
    
class Flx_Input;
class Flx_Box;
struct ActionParm;
    
class Flx_NumericInput : public Flx_Group {
public:
    my::Signal< Flx_NumericInput, ActionParm > signalChanged;
    my::Signal< Flx_NumericInput, bool > signalFocus;
public:
    Flx_NumericInput(int X,int Y,int W,int H,const char *L=0);
    void type( uchar t );
    void justify( Fl_Align val );
    void value(const char *val);
    const char *value() const;
    int handle(int e);
    Fl_Font textfont();
    void textfont( Fl_Font );
    void textcolor( Fl_Color );
    Fl_Color textcolor() const;
    Fl_Fontsize textsize();
    void textsize( Fl_Fontsize );
    Fl_Font labelfont();
	void labelfont( Fl_Font );
    Fl_Fontsize labelsize();
	void labelsize( Fl_Fontsize );
    static void staticCallback( Fl_Widget *, void * );
    void callback();
    bool hasFocus() const;
private:
    Flx_Input *_pInp;
    Flx_Box   *_pBox;
};
} //flx

#endif	/* FLX_NUMERICINPUT_H */

