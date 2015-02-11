/* 
 * File:   Flx_Tabs.h
 * Author: max
 *
 * Created on 21. Dezember 2013, 19:30
 */

#ifndef FLX_TABS_H
#define	FLX_TABS_H

#include <FL/Fl_Tabs.H>

namespace flx {
    
class Flx_Tabs : public Fl_Tabs {
public:
    Flx_Tabs( int x, int y, int w, int h, const char *pLabel = 0 );
    virtual ~Flx_Tabs();
private:

};

} //flx
#endif	/* FLX_TABS_H */

