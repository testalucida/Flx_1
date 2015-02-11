/* 
 * File:   Fl_FloatInput.h
 * Author: max
 *
 * Created on 3. Oktober 2013, 13:50
 */

#ifndef FL_FLOATINPUT_H
#define	FL_FLOATINPUT_H

//#include <FL/Fl_Float_Input.H>
#include <flx/Flx_NumericInput.h>

namespace flx {

class Flx_FloatInput : public Flx_NumericInput { //Fl_Float_Input {
public:
    Flx_FloatInput( int x, int y, int w, int h, const char *pLabel = 0 );
    void value( float, const char *pFormat = "%.2f" );
    float floatValue() const;
    virtual ~Flx_FloatInput();
private:

};

} //flx

#endif	/* FL_FLOATINPUT_H */

