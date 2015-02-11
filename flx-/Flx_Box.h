#ifndef _FLX_BOX_H_
#define _FLX_BOX_H_

#include <flx/flx_signalparms.h>

#include <FL/Fl_Box.H>

namespace flx {

class Flx_Box :	public Fl_Box {
public:
	Flx_Box( int x, int y, int w, int h, const char *pLabel = (char*)0 );
	virtual ~Flx_Box(void);
    /** _underline == 0: no underlining 
                      1: one...
                      2: ...two underlinings */
    void setUnderline( int nLines ) { _underline = nLines;}
protected:
	//int handle( int evt );
    void draw();
private:
	static const int _defaultSize = 12;
    int _underline; //1: one, 2: two underlinings
};

} //ns flx

#endif
