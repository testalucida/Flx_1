#include <string>
#include <FL/Fl_Group.H>
#include <flx/flx_boxtypes.h>
#include <flx/Flx_Button.h>
#include <flx/flx_signalparms.h>

#include <my/Signal.hpp>


class Fl_Choice;

namespace flx {

class Flx_ToolButton : public Flx_Button {
public:
	Flx_ToolButton( Fl_Boxtype type = FL_FLAT_BOX, Fl_Boxtype downtype = FL_DOWN_BOX );
	Flx_ToolButton( const char *pLabel, Fl_Boxtype type = FL_FLAT_BOX, Fl_Boxtype downtype = FL_DOWN_BOX );
	Flx_ToolButton( const char **ppXpm, const char **ppXpmDeact, Fl_Boxtype type = FL_FLAT_BOX, Fl_Boxtype downtype = FL_DOWN_BOX );
	Flx_ToolButton( int x, int y, int w, int h, Fl_Boxtype type = FL_FLAT_BOX, Fl_Boxtype downtype = FL_DOWN_BOX );
	~Flx_ToolButton();
	//void setCallback( myfltk::ON_WIDGET_CALLBACK cb );
	void setIdent( int ident ) { _ident = ident; }
	int getIdent() { return _ident; }
private:
	void init( Fl_Boxtype uptype, Fl_Boxtype downtype );
	static void staticBtnCallback( Fl_Widget *pW, void *pUserdata );
	void doCallback();
	int _ident; //Workaround wegen dem Schmarrn mit Flx_Callback.
};

//////////////////////////////////////////////////////////////////

class Flx_InputChoice;
class Flx_ToolButton;

class Flx_ToolBar : public Fl_Group {
	static const int SPACING = 5;
public:
	my::Signal< Flx_ToolBar, ToolAction > signalToolButtonSelected;
public:
	Flx_ToolBar( int x, int y, int w, int h );
    virtual ~Flx_ToolBar();
	/** sets a left aligned label */
	void setLabel( const char *pLabel );
	Flx_ToolButton &addButton(  const char **ppXpm, 
								const char **ppXpmDeact = NULL,
								const char *pTooltip = NULL,
								int ident = -1,
								void *pUserdata=0 ); 
	void addLabel( const char *pLabel );
	void addSeparator();
	void resize( int x, int y, int w, int h );
	Fl_Widget *getTool( int ident ) const;
	void onToolButtonSelected( Flx_Button &, ActionParm & );
private:
	/** max X-position of rightmost tool */
	int getMaxX();

	Flx_ToolButton &createButton( int x, 
								  const char *pTooltip,								 
								  int ident,
								  void *pUserdata );
	void resizeStrut( Fl_Widget *pMostRightWidget );
private:
	const int XSPACING;
	const int YSPACING;
	const int TOOLHEIGHT;
	const int TOOLBAR_LABELSIZE;
	bool _isCloseable;
	Flx_ToolButton *_pCloseBtn;
	std::string _label;
	int _labelwidth;
	bool _labelSuppressed;
	Fl_Box *_pStrut;
};

}  //ns myfltk