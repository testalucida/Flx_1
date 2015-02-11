#ifndef _FLX_OUTPUT_H_
#define _FLX_OUTPUT_H_

#include <FL/Fl_Output.H>
#include <my/CharBuffer.h>
#include <string>

namespace flx {

class Flx_Output : public Fl_Output {
public:
	Flx_Output( int x, int y, int w, int h, const char *pLabel=0 );
    void const_value( const char *pVal );
    void const_value( const std::string &val );

	~Flx_Output(void);
    
private:
    my::CharBuffer _constValue;
};

} //ns flx

#endif

