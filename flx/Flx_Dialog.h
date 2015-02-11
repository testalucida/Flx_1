#ifndef _FLX_DIALOG_H_
#define _FLX_DIALOG_H_

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Widget.H>

namespace flx {

class Flx_Dialog : public Fl_Double_Window {
public:
	Flx_Dialog( int x, int y, int w, int h, const char* pTitle = 0x00 );
	int show( bool modally );
    /** gets position and size of the client usable group */
	void getClientArea( int& x, int& y, int& w, int& h );
    /** shadow Fl_Group::add( Fl_Widget & )*/
    void add( Fl_Widget & );
    /** shadow Fl_Group::add( Fl_Widget * )*/
    void add( Fl_Widget * );
    void setButtonLabels( const char *pOkLabel, const char *pCancelLabel ) {
        _pBtnOk->copy_label( pOkLabel );
        _pBtnCancel->copy_label( pCancelLabel );
    }
    
    /** sets the widget which is to have focus first when this
     dialog is shown */
    void setFocusOnShow( Fl_Widget & );
	
	/** Does actually do nothing.
	    Override this method in your derived dialog as necessary.
		This method is called each time the user hits OK or Enter.
		It's a great place to copy widgets' values into the model.*/
	virtual bool validate();
	//bool ok();
protected:
	/** creates a layout showing an OK and a Cancel button.
	Override this for your own button needs */
	virtual void createLayout();
private:
	static void staticOkCancel_cb( Fl_Widget* pWdgt, void* pUserData );
	void okCancel_cb( Fl_Button* pBtn );

///////DATA/////
protected:
	Fl_Return_Button* _pBtnOk;
private:
	int _labelSize;
	Fl_Group* _pGrp;
	Fl_Button* _pBtnCancel;
	bool _ok; //true, if OK pressed
    Fl_Widget *_pFocusOnShow;
};

} //ns myfltk
#endif