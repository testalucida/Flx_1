#ifndef FLX_BASE_H
#define FLX_BASE_H

#include <my/CharBuffer.h>

class Fl_Widget;

namespace flx {

enum Flx_Type {
    FLX_TYPE_BUTTON,
    FLX_TYPE_BOX,
    FLX_TYPE_CHECKBUTTON,
    FLX_TYPE_CHOICE,
    FLX_TYPE_EXPANDERGROUP,
    FLX_TYPE_EXPANDERHEADER,
    FLX_TYPE_EXPANDERCONTENTGROUP,
    FLX_TYPE_GROUP,
    FLX_TYPE_INPUT,
    FLX_TYPE_INTINPUT,
    FLX_TYPE_FLOATINPUT,
    FLX_TYPE_MULTILINEINPUT,
    FLX_TYPE_MULTILINEOUTPUT,
    FLX_TYPE_OUTPUT,
    FLX_TYPE_RADIOBUTTON,
    FLX_TYPE_RETURNBUTTON,
    FLX_TYPE_TABLE,
    FLX_TYPE_WINDOW,
    FLX_TYPE_WIZARD,
    FLX_TYPE_UNKNOWN
};

class Flx_Base {
public:
    Flx_Base();
    virtual ~Flx_Base() {}
    Flx_Type getFlxType() const;
    virtual const char *toString() const;
    my::CharBuffer &toCharBuffer() const;
    virtual void scaleUp();
    virtual void scaleDown();
    Fl_Widget &getWidget() const { return *_pWidget; }
protected:
    Flx_Type _type;
    Fl_Widget *_pWidget;
private:
    mutable my::CharBuffer _toString;
};

} //flx
#endif // FLX_BASE_H
