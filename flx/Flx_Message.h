#pragma once 

#include <flx/Flx_Dialog.h>
#include <images/failed48.xpm>
#include <images/stop.xpm>
#include <images/warning.xpm>
#include <images/info.xpm>
#include <images/question.xpm>

#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Output.H>

namespace flx {

class Flx_Message {
public:
	static void setColor( Fl_Color color );
	static void failed( const char *pMsg );
	static void stop( const char *pMsg );
	static void warning( const char *pMsg );
	static void info( const char *pMsg );
    static int ask( const char *pTitle, const char *pQuestion );
private:
	static flx::Flx_Dialog *createDialog();
private:
	static flx::Flx_Dialog *_pDlg;
	static Fl_Box *_pPicBox;
	static Fl_Pixmap *_pFailed_xpm;
	static Fl_Pixmap *_pStop_xpm;
	static Fl_Pixmap *_pWarning_xpm;
	static Fl_Pixmap *_pInfo_xpm;
    static Fl_Pixmap *_pQuestion_xpm;
	static Fl_Multiline_Output *_pMsg;
	static Fl_Color _color;
};

}