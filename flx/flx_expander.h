
#ifndef _FLX_EXPANDER_H_
#define _FLX_EXPANDER_H_

#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>

#include <my/Signal.hpp>

#include <vector>
#include <cstdio>

class Fl_Box;
class Fl_Pixmap;
class Fl_Button;

namespace flx {

class Flx_ContextMenu;

struct Position {
    int row;
    int col;
};

//////////////////////////////////////////////////////////

class Flx_ExpanderContentGroup : public Fl_Group {
public:
	Flx_ExpanderContentGroup( int row, int col, int w, int h, const char *pLbl = NULL );
	void resize( int, int, int, int );
    Position getPosition() const;
	/** add a Widget to this content group.
		You must NOT add() a widget while the content group 
		is beeing shown (is expanded) */
	void add( Fl_Widget * );
	void add( Fl_Widget & );
private:
    int _row, _col;
};

//////////////////////////////////////////////////////////

class Flx_ExpanderHeader : public Fl_Group {
public:
	my::Signal< Flx_ExpanderHeader, my::NULLPARM > signalHeaderPushed;
    //my::Signal< Flx_ExpanderHeader, int > signalMenuCommand;
public:
	Flx_ExpanderHeader( int x, int y, int w, int h, const char *pLbl = NULL, Flx_ExpanderContentGroup *pContentGrp = NULL );
	void draw();
	int handle( int );
	bool isExpanded() const { return _isExpanded; }
    Flx_ExpanderContentGroup &getContentGroup() const { return *_pContentGrp; }
	void resize( int x, int y, int w, int h );
	void setActive( bool active );
	bool isActive() const {  return _isActive; }
	/** causes the header to change symbol if necessary and to 
	    send a signalHeaderPushed if a change was really done. 
		If deactivate == true, the header will be disabled */
	void collapse( bool deactivate );
	/** causes the header to change symbol if necessary and to send
	    a signalHeaderPushed if a change was really done. */
	void expand();
	/** sets the context menu that is shown after
     * right clicking on this header.
     * NULL disables the context menu. */
    //void setContextMenu( Flx_ContextMenu * );
    //Flx_ContextMenu &getContextMenu() const;
private:
	void toggleExpandClose();
    void showContextMenu();
    void handleContextMenuCommand( int ident );
private:
    Fl_Box *_pPixbox;
	Fl_Pixmap *_pOpenXpm;
	Fl_Pixmap *_pCloseXpm;
    Fl_Button *_pLabelBtn;
	bool _isExpanded;
    Flx_ExpanderContentGroup *_pContentGrp;
	bool _isActive;
    Flx_ContextMenu *_pContextMenu;
};

//////////////////////////////////////////////////////////

struct Flx_ExpandParm {
	Flx_ExpanderContentGroup &grp;
	bool beforeExpanding;
	bool beforeCollapsing;
};

/** class designed for hosting Flx_Expander instances */
class Flx_ExpanderGroup : public Fl_Group {
public:
	my::Signal< Flx_ExpanderGroup, Flx_ExpandParm > signalBeforeExpandOrCollapse;
public:
	Flx_ExpanderGroup( int x, int y, int w, int h, const char *pLbl = NULL, int columns = 1 );
	void end();
	void resize( int, int, int, int );
	void onExpandedOrCollapsed( Flx_ExpanderHeader &, my::NULLPARM & );
    /** gets the available width for a Flx_ExpanderContentGroup */
	int getColumnWidth() const;
	void setColumnCount( int );
	/** Returns the number of columns. 
	    Be aware that not every cell (row/col-position) is used so you
		might get an exception when addressing a not used position. */
	int getColumnCount() const { return _colCnt; }
	/** Returns the number of rows. 
		Be aware that not every cell (row/col-position) is used so you
		might get an exception when addressing a not used position. */
	int getRowCount() const;
    void add( Flx_ExpanderContentGroup * );
	void setActive( int row, int col, bool active );
	bool isActive( int row, int col ) const;
	Flx_ExpanderContentGroup &getContentGroup( int row, int col ) const;
	/** collapses a Flx_ExpanderContentGroup. Disables the header if
	    deactivate == true */
	void collapse( int row, int col, bool deactivate );
	/** expands a Flx_ExpanderContentGroup */
	void expand( int row, int col ) const ;
	void getExpandedContentGroups( std::vector< Flx_ExpanderContentGroup* > & ) const;
private:	
    void releaseChildren();
    void grabChildren();
	Flx_ExpanderHeader &findHeader( int row, int col ) const;
    int getNeededHeight() const;
	int getHeaderCount( int colIdx ) const; 
private:
    std::vector< Flx_ExpanderHeader* > _expanderHeaders;
	int _spacingC; //spacing between Flx_Expander columns
	int _spacingR; //spacing between Flx_Expander rows
	int _marginL; //left side margin of Flx_Expander column 0.
	int _marginR; //right side margin of last column
	int _marginT; //top margin of the topmost Flx_Expander row
	int _colCnt; //number of columns of Flx_Expander objects
	//int _colWidth; //width of each column of Flx_Expander objects
    int _headerH;
};

//////////////////////////////////////////////////////////

class Flx_ExpanderScroll : public Fl_Scroll {
public:
    Flx_ExpanderScroll( int x, int y, int w, int h );
    void setExpanderGroup( Flx_ExpanderGroup *pGrp );
    void resize( int x, int y, int w, int h );
private:
    Flx_ExpanderGroup *_pGrp;
};

} //ns flx

#endif
