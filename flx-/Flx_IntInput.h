#pragma once

//#include <FL/Fl_Int_Input.H>
#include <flx/Flx_NumericInput.h>

#include <my/CharBuffer.h>

//namespace my {
//    class CharBuffer;
//};

namespace flx {

class Flx_IntInput : public Flx_NumericInput { //Fl_Int_Input {
public:
	Flx_IntInput( int x, int y, int w, int h, const char *pLabel=0 );
    const my::CharBufferPtr &getInput() const;
	virtual ~Flx_IntInput(void);
};

}

