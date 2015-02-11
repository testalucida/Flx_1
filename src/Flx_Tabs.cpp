/* 
 * File:   Flx_Tabs.cpp
 * Author: max
 * 
 * Created on 21. Dezember 2013, 19:30
 */

#include <flx/Flx_Tabs.h>

#include <FL/fl_draw.H>

namespace flx {
    
Flx_Tabs::Flx_Tabs( int x, int y, int w, int h, const char *pLabel ) 
: Fl_Tabs( x, y, w, h ) 
{
    this->labelfont( fl_font() );
    this->labelsize( fl_size() );
}

Flx_Tabs::~Flx_Tabs( ) {
}

}//flx