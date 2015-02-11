#ifndef _FLX_SIGNALBOX_
#define _FLX_SIGNALBOX_

#include <flx/Flx_Box.h>
#include <flx/flx_signalparms.h>

#include <my/Signal.hpp>

namespace flx {

class Flx_SignalBox : public Flx_Box {
public:
	my::Signal < Flx_SignalBox, ClickParm > signalClick;
	my::Signal < Flx_SignalBox, MouseParm > signalMouseEnter;
	my::Signal < Flx_SignalBox, MouseParm > signalMouseLeave;
public:
	Flx_SignalBox( int x, int y, int w, int h, const char *pLabel=0 );
	virtual ~Flx_SignalBox() {}
protected:
	int handle( int evt );
};

} //ns flx

#endif