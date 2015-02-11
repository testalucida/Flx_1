#ifndef _FLX_WINDOW_H_
#define _FLX_WINDOW_H_

#include <FL/Fl_Double_Window.H>

namespace flx {

class Flx_Window : public Fl_Double_Window {
public:
	Flx_Window( int x, int y, int w, int h, const char *pLabel=(const char*)0 );
	~Flx_Window() {}
protected:
	int handle( int evt );
private:
	bool _shortCutComing;
};

} //ns flx

#endif