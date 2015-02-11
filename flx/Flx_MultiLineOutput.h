/* 
 * File:   Flx_MultiLineOutput.h
 * Author: max
 *
 * Created on 29. September 2013, 14:48
 */

#ifndef FLX_MULTILINEOUTPUT_H
#define	FLX_MULTILINEOUTPUT_H

#include <FL/Fl_Multiline_Output.H>

namespace flx {

class Flx_MultiLineOutput : public Fl_Multiline_Output {
public:
    Flx_MultiLineOutput( int x, int y, int w, int h, const char *pLabel=0 );
   
    virtual ~Flx_MultiLineOutput();
private:

};

} //flx
#endif	/* FLX_MULTILINEOUTPUT_H */

