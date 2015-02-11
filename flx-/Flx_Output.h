#ifndef _FLX_OUTPUT_H_
#define _FLX_OUTPUT_H_

#include <FL/Fl_Output.H>

namespace flx {

class Flx_Output : public Fl_Output {
public:
	Flx_Output( int x, int y, int w, int h, const char *pLabel=0 );
	~Flx_Output(void);
};

} //ns flx

#endif

