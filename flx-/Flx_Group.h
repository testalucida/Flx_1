#ifndef _FLX_GROUP_
#define _FLX_GROUP_

#include <FL/Fl_Group.H>

namespace flx {

class Flx_Group : public Fl_Group {
public:
	Flx_Group( int x, int y, int w, int h, const char *pLabel = 0 );
	virtual ~Flx_Group(void);
};

} //flx

#endif