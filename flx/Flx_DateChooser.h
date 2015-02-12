#ifndef FLX_DATECHOOSER_H_
#define FLX_DATECHOOSER_H_

#include <flx/Flx_Group.h>
#include <my/datetime.h>
#include <my/Signal.hpp>

namespace flx {

class Flx_Output;
class Flx_Button;
struct ActionParm;

/**	Eine Flx_Group, die ein Ausgabefeld f�r das gew�hlte Datum enth�lt 
	und rechts daneben einen Button, mit dem das Flx_Calendar-Control ge�ffnet wird.
	Das Label wird links vom Eingabefeld angezeigt. */
class Flx_DateChooser : public Flx_Group {
public:
    my::Signal< Flx_DateChooser, my::MyDate > signalDateSelected;
public:
	Flx_DateChooser( int x, int y, int h, const char *pLabel=0 );
	void setDate( const my::MyDate &dt );
    
    /**
     * sets the inital date
     * @param date "dd.mm.yyyy" oder "yyyy-mm-dd"
     */
    void setDate( const char *date );
	const my::MyDate &getDate() const;
	void onButtonHit( Flx_Button &, ActionParm & );
private:
	Flx_Output *_pOutputDate;
	Flx_Button *_pBtnCalendar;
	my::MyDate _date;
};

} //flx

#endif