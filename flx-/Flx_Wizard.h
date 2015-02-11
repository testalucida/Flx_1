/* 
 * File:   Flx_Wizard.h
 * Author: max
 *
 * Created on 27. September 2013, 13:54
 */

#ifndef FLX_WIZARD_H
#define	FLX_WIZARD_H

#include <FL/Fl_Wizard.H>

class Flx_Wizard : public Fl_Wizard {
public:
    Flx_Wizard( int x, int y, int w, int h, const char *pLabel=0 );
    virtual ~Flx_Wizard();
private:

};

#endif	/* FLX_WIZARD_H */

