#include <flx/Flx_Table.h>
#include <flx/flx_boxtypes.h>
#include <flx/Flx_TableSearch.h>
#include <flx/Flx_MenuItemFactory.h>
#include <flx/Flx_Input.h>

#include <my/compat.h>
#include <my/convert.h>

#include <FL/fl_ask.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Menu_.H>

#include <cstdio>
#include <iostream>
#include <algorithm>

#include "flx/flx_signalparms.h"

using namespace std;
using namespace my;

namespace flx {

const char *COPY = "Kopieren";
const char *PASTE = "Einfügen";
const char *SEARCH = "Suchen...";
const char *ENTRIES = "Einträge: ";

const char *SORT_ASC = "Sortiere aufsteigend";
const char *SORT_DESC = "Sortiere absteigend";
const char *MARK = "Markiere alle Spaltenwerte";
const char *COPY_COL_VALUES = "Kopiere alle Spaltenwerte";
const char *COPY_WITH_SEP = "Kopiere alle Spaltenwerte/durch Kommas getrennt";
const char *COPY_WITHOUT_SEP = "Kopiere alle Spaltenwerte/ohne Trennzeichen";
const char *GOTO_COL = "Gehe zu Spalte";
const char *REMOVE_COL = "Entferne Spalte";

enum ContextMenuIdent {
	IDENT_SORT_ASC,
	IDENT_SORT_DESC,
	IDENT_MARK,
    IDENT_COPY,
	IDENT_COPY_WITH_SEP,
	IDENT_COPY_WITHOUT_SEP,
    IDENT_PASTE,
	IDENT_GOTO_COL,
	IDENT_REMOVE_COL,
    IDENT_SEARCH
};

const char *NEXTROW = "\n"; //separator for clipboard actions
const char *NEXTCOL = "\t"; 

//===============================================

Flx_ColumnHeaderContextMenu::Flx_ColumnHeaderContextMenu()
	: Flx_ContextMenu()
{
    this->textsize( 12 );
    //add(const char* label, int shortcut, Fl_Callback*, void *user_data=0, int flags=0)
	add( SORT_ASC, 0, NULL, (void*)IDENT_SORT_ASC, FL_MENU_INACTIVE );
    add( SORT_DESC, 0, NULL, (void*)IDENT_SORT_DESC, FL_MENU_DIVIDER | FL_MENU_INACTIVE );
    //add( MARK, FL_CTRL+'m', NULL, NULL, 0 );
    add( MARK, 0, NULL, (void*)IDENT_MARK,  FL_MENU_INACTIVE );
	add( COPY_COL_VALUES, 0, NULL, NULL, FL_MENU_DIVIDER | FL_SUBMENU );
	add( COPY_WITH_SEP, 0, NULL, (void*)IDENT_COPY_WITH_SEP, 0 );
	add( COPY_WITHOUT_SEP, 0, NULL, (void*)IDENT_COPY_WITHOUT_SEP, 0 );
	add( GOTO_COL, 0, NULL, (void*)IDENT_GOTO_COL, FL_SUBMENU );
}

//===============================================

Flx_CellContextMenu::Flx_CellContextMenu()
	: Flx_ContextMenu(), _entryItemIdx( -1 )
{
//    add( COPY, FL_CTRL+'c', NULL, NULL /*, FL_MENU_INACTIVE*/ );
//    add( PASTE, FL_CTRL+'v', NULL, NULL, FL_MENU_DIVIDER | FL_MENU_INACTIVE );
//    add( SEARCH, FL_CTRL+'f', NULL, NULL, FL_MENU_DIVIDER );
    addMenuItem( COPY, FL_CTRL+'c', IDENT_COPY );
    addMenuItem( PASTE, FL_CTRL+'v', IDENT_PASTE, -1 /*pos*/, 
                 NULL /*ppXpm*/, NULL /*pUserdata*/,
                 FL_MENU_DIVIDER | FL_MENU_INACTIVE );
    addMenuItem( SEARCH, FL_CTRL+'f', IDENT_SEARCH, -1, NULL, NULL, FL_MENU_DIVIDER );
}

void Flx_CellContextMenu::setEntriesCountItem( int rows ) {
	char label[30];
	strcpy( label, ENTRIES );
	char intbuf[10];
	sprintf( intbuf, "%d", rows );
	strcat( label, intbuf );
	if( _entryItemIdx < 0 ) {
		//_entryItemIdx = add( label, 0, NULL, NULL, FL_MENU_INACTIVE );
        addMenuItem( label, (int)0, (int)0, -1, 
                    (const char**)NULL, (void*)NULL, FL_MENU_INACTIVE );
        _entryItemIdx = 0;
	} else {
		//replaceLabel(  );
	}
}

//============================================

Selection::Selection() 
	: _rows( 0 ) 
	, _cols( 0 ) 
	, _pSel( NULL )
{
}

Selection::Selection( int rows, int cols ) {
	setSize( rows, cols );
}

Selection::~Selection() {
}


void Selection::setSize( int rows, int cols ) {
	if( _pSel ) {
		delete[] _pSel;
		_pSel = NULL;
	}

	long n = rows*cols;
	_pSel = new uchar[n];

	memset( _pSel, 0, sizeof( uchar ) * n );

	_rows = rows;
	_cols = cols;
}

inline long Selection::getSelIndex( int row, int col ) const {
	return row * _cols + col;
}

long Selection::getMaxIndex() const {
	return _rows * _cols - 1;
}

inline void Selection::getRowStartEndIndexes( int row, long &start, long &end ) const {
	start = getSelIndex( row, 0 );
	end = getSelIndex( row, _cols - 1 );
}

/** mark cell row/col as "on" */
void Selection::on( int row, int col ) {
	_pSel[getSelIndex( row, col )] = 1;
}

/** mark cell row/col as "off" */
void Selection::off( int row, int col ) {
	_pSel[getSelIndex( row, col )] = 0;
}

/** toggle mark for cell row/col */
void Selection::toggle( int row, int col ) {
	if( isOn( row, col ) ) {
		off( row, col );
	} else {
		on( row, col );
	}
}

/** mark all cells of a given row as "on" */
void Selection::rowOn( int row ) {
	rowOnOff( row, 1 );
}

/** mark all cells of a given row as "off" */
void Selection::rowOff( int row ) {
	rowOnOff( row, 0 );
}

inline void Selection::rowOnOff( int row, uchar onoff ) {
	long s, e;
	getRowStartEndIndexes( row, s, e );
	for( uchar *p = _pSel + s, *pMax = _pSel + e; p <= pMax; p++ ) {
		*p = onoff;
	}
}

/** mark all cells of a given column as "on" */
void Selection::colOn( int col ) {
	colOnOff( col, 1 );
}

/** mark all cells of a given column as "off" */
void Selection::colOff( int col ) {
	colOnOff( col, 0 );
}

void Selection::colOnOff( int col, uchar onoff ) {
	for( uchar *p = _pSel + col, *pMax = _pSel + getMaxIndex(); p < pMax; p += _cols ) {
		*p = onoff;
	}
}

void Selection::areaOn( int toprow, int leftcol, int botrow, int rightcol ) {
	int minrow, maxrow;
	if( toprow < botrow ) {
		minrow = toprow;
		maxrow = botrow;
	} else {
		minrow = botrow;
		maxrow = toprow;
	}
	int mincol, maxcol;
	if( leftcol < rightcol ) {
		mincol = leftcol;
		maxcol = rightcol;
	} else {
		mincol = rightcol;
		maxcol = leftcol;
	}
	for( int r = minrow; r <= maxrow; r++ ) {
		for( int c = mincol; c <= maxcol; c++ ) {
			on( r, c );
		}
	}

}

void Selection::areaOff( int fromrow, int fromcol, int torow, int tocol ) {
	int minrow, maxrow, mincol, maxcol;
	if( fromrow < torow ) {
		minrow = fromrow;
		maxrow = torow;
	} else {
		minrow = torow;
		maxrow = fromrow;
	}
	if( fromcol < tocol ) {
		mincol = fromcol;
		maxcol = tocol;
	} else {
		mincol = tocol;
		maxcol = fromcol;
	}
	for( int r = minrow; r <= maxrow; r++ ) {
		for( int c = mincol; c <= maxcol; c++ ) {
			off( r, c );
		}
	}

}

/** mark all cells as "on" */
void Selection::allOn() {
	memset( _pSel, 1, sizeof( uchar ) * _rows * _cols );
}

/** mark all cells as "off" */
void Selection::allOff() {
	memset( _pSel, 0, sizeof( uchar ) * _rows * _cols );
}

CellPtr Selection::getFirstSelectedCell() const {
	CellPtr pCell( new Cell );

	const uchar *p = _pSel;
	for( int r = 0; r < _rows; r++ ) {
		for( int c = 0; c < _cols; c++, p++ ) {
			if( *p == 1 ) {
				pCell->row = r;
				pCell->col = c;
				return pCell;
			}
		}
	}

	return pCell;
}


const std::vector<CellPtr>& Selection::getSelectedCells() {
	_selectedCells.clear();
	const uchar *p = _pSel;
	for( int r = 0; r < _rows; r++ ) {
		for( int c = 0; c < _cols; c++, p++ ) {
            uchar sel = *p;
			if( !( sel == 0 ) ) {
				CellPtr pCell( new Cell );
				pCell->row = r;
				pCell->col = c;
				_selectedCells.push_back( pCell );
			}
		}
	}
	return _selectedCells;
}

void Selection::dumpSelection() const {
    printf( "---------Selection---------\n" );
    printf( "this: %p\n", this );
    printf( "selectable Cells. Rows = %d, Cols = %d\n", _rows, _cols );
    
    printf( "selected (0/1):\n" );
    uchar *pSel = _pSel;
    for( int i = 0, imax = _rows*_cols; i < imax; i++, pSel++ ) {
        uchar sel = *pSel;
        printf( "\tcell index: %d --> %c\n", i, (sel == 0) ? '0' : '1' );
    }
    
    printf( "---------------------------\n" );
}

const vector<int>& Selection::getSelectedRows() {
	_selectedRows.clear();
	const uchar *p = _pSel;
	for( int r = 0; r < _rows; r++, p += _cols ) {
		int c = 0;
		for( ; c < _cols; c++ ) {
			if( ! *( p + c ) == 1 ) {
				break;
			}
		}
		if( c == _cols ) {
			_selectedRows.push_back( r );
		}
	}
	return _selectedRows;
}

const vector<int>& Selection::getSelectedColumns() {
	_selectedCols.clear();
	const uchar *p = _pSel;
	for( int c = 0; c < _cols; c++, p += _rows ) {
		int r = 0;
		for( ; r < _rows; r++ ) {
			if( ! *( p + r ) == 1 ) {
				break;
			}
		}
		if( r == _rows ) {
			_selectedCols.push_back( c );
		}
	}
	return _selectedCols;
}


inline bool Selection::isOn( int row, int col ) const {
	return ( _pSel[getSelIndex( row, col )] == 1 );
}

bool Selection::isRowOn( int row ) const {
	long s, e;
	getRowStartEndIndexes( row, s, e );
	for( uchar *p = _pSel + s, *pMax = _pSel + e; p <= pMax; p++ ) {
		if( *p == 0 ) return false;
	}

	return true;
}

bool Selection::isColOn( int col ) const {
	for( uchar *p = _pSel + col, *pMax = _pSel + getMaxIndex(); p < pMax; p += _cols ) {
		if( *p == 0 ) return false;
	}

	return true;
}

int Selection::getMinRowOn() {

	const uchar *p = _pSel;
	for( int r = 0; r < _rows; r++, p += _cols ) {
		int c = 0;
		for( ; c < _cols; c++ ) {
			if( *( p + c ) == 0 ) {
				break;
			}
		}
		if( c == _cols ) {
			return r;
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////  F L X _ T A B L E ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/** Ctor */
Flx_Table::Flx_Table( int x, int y, int w, int h, const char* L ) :
		Fl_Table( x, y, w, h, L ),
        Flx_Base(),
		//_model( NULL ),
		_pData( NULL ),
		_selectionMode(	FLX_SELECTION_SINGLECELL ),
		_isSortable( false ),
		_sortdirection(	SORTDIRECTION_NONE ),
		_anchorRow( -1 ),
		_anchorCol( -1 ),
		_gotoCol( -1 ),
		_gotoColColor( fl_lighter( fl_lighter( FL_YELLOW ) ) ),
		_pIn( NULL ),
		_rowEdit( -1 ),
		_colEdit( -1 ),
		_pSearch( NULL ),
        _isAlternatingColumnColor( false ),
        _isAlternatingRowColor( false ),
        _backgroundColor( FL_WHITE ),
        _alternatingColumnColor( FL_WHITE ),
        _alternatingRowColor( FL_WHITE ),
        _drawCellBorders( true )
{
    _pWidget = this; //MANDATORY due to Flx_Base
    
	//font default
	labelfont( 0 );
	labelsize( 12 );
    
	// Rows
	rows( 0 );
	row_header( 1 ); // enable row headers (along left)
    row_header_color( FL_LIGHT1 | FL_GRAY );
	row_resize( 0 ); // disable row resizing
    //row_height( 10 ); --> doesn't effect anything

	// Cols
	cols( 0 );
	col_header( 1 ); // enable column headers (along top)
    col_header_color( FL_LIGHT1 | FL_GRAY );
	// default width of columns
	col_resize( 1 ); // enable column resizing
	//callback( &static_event_callback, (void*) ((this)) );
	_eventContext.reset(); //initialize event context struct
    
	//for editing cell values:
	_pIn = &getEditor(); 
	_pIn->textfont( labelfont() );
	_pIn->textsize( Fl_Table::labelsize() );
	_pIn->hide();
    _pIn->signalEnterPressed.connect < Flx_Table, &Flx_Table::onCellEditorEnterKey >( this );
    _pIn->signalTabPressed.connect < Flx_Table, &Flx_Table::onCellEditorTabKey >( this );
	
	end();
    
    _pCellMenu = new Flx_CellContextMenu();
    _pCellMenu->signalShortcut.
            connect< Flx_Table, &Flx_Table::onShortcut >( this );
}

Flx_Table::~Flx_Table() {
    if( _pSearch ) delete _pSearch;
    delete _pCellMenu;
}

void Flx_Table::setNiceDefaults() {
    row_header_color( fl_rgb_color( 221, 221, 221 ) );
    col_header_color( row_header_color() );
    
    setAlternatingRowColor();
    setSelectionMode( FLX_SELECTION_FREE );
    drawCellBorders( false );
    setSortable( true );
}

const char *Flx_Table::toString() const {
    return Flx_Base::toString();
}

void Flx_Table::scaleDown() {
    Flx_Base::scaleDown();
    if( rows() == 0 || cols() == 0 ) return;
    
	makeColumnsFit();

    int rh = row_height( 0 ) < 11 ? row_height( 0 ) : row_height( 0 ) - 2;
    row_height_all( rh );
	col_header_height( rh );
    redraw();
}

void Flx_Table::scaleUp() {
    Flx_Base::scaleUp();
    if( rows() == 0 || cols() == 0 ) return;
    
    makeColumnsFit();

    int rh = row_height( 0 ) + 2;
    row_height_all( rh );
	col_header_height( rh );
    redraw();
}

Flx_Input &Flx_Table::getEditor() {
	return *(new Flx_Input( 1, 1, 0, 0 ));
}

void Flx_Table::onCellEditorEnterKey( Flx_Input &in, ActionParm &p ) {
    editNextRow();
}

void Flx_Table::onCellEditorTabKey( Flx_Input &in, ActionParm &p ) {
    editNextColumn();
}

void Flx_Table::editNextColumn() {
    int r = -1, c;
    
    c = ( _colEdit < this->cols() - 1 ) ? _colEdit + 1 : 0;
    
    if( c == 0 ) {
        //do we have a next row?
        if( _rowEdit < this->rows() - 1 ) {
            r = _rowEdit + 1;
        }
    } else {
        //stay on this row
        r = _rowEdit;
    }
    
    endEdit();
    
    if( r > -1 ) {
        startEdit( r, c );
    }
    if( Fl::focus() != _pIn ) {
        fprintf( stderr, "hab keinen Fokus!\n" );
    }
}

void Flx_Table::editNextRow() {
    int r = -1, c = _colEdit;
    if( _rowEdit < this->rows() - 1 ) {
        r = _rowEdit + 1;
    }
    
    endEdit();
    if( r > -1 ) {
        startEdit( r, c );
    }
}


/** Draw the col headings */
void Flx_Table::drawColumnHeader( int col, const char* s, int x, int y, int w, int h ) {
	fl_push_clip( x-1, y, w, h );
	Fl_Color colorMemo = color(); //dont't use fl_color(): won't compile in release config.
	fl_draw_box( FL_FLAT_BOX, x, y, w, h, _gotoCol == col ? _gotoColColor : col_header_color() );
	//fl_draw_box( FLX_GRADIENT_BORDER_BOX, x, y, w, h, _gotoCol == col ? _gotoColColor : col_header_color() );
	
    //draw a vertical line between columns and a downsided horizontal one
    fl_color ( FL_GRAY );
    fl_line( x-1, y, x-1, y+h-1, x+w, y+h-1 );
    
    if( _pData->getSortInfo().isSorted && _pData->getSortInfo().col == col ) {
		fl_color( fl_darker( col_header_color() ) );
		drawSortedFlag( x, y, w, h, _pData->getSortInfo().sortDirection );
		fl_pop_clip();
		fl_push_clip( x, y, w-13, h );
	}
	fl_color (FL_BLACK);
	fl_draw( s, x, y, w, h, FL_ALIGN_CENTER );
	fl_color( colorMemo );
	fl_pop_clip();
    if( _rowEdit != -1 ) {
        //we're in editing mode; resize cell editor:
        makeCellEditorFit();
    }
}

void Flx_Table::drawSortedFlag( int x, int y, int w, int h, SortDirection sortdir ) const {
	int r, x1, x2, x3, y1, y2, y3, s = 8; //side length
	r = h - s;
	x1 = x+w-s-4;
	x2 = x1+s;
	x3 = (x1+x2)/2;
	y1 = y+h-r/2;
	y2 = y1;	
	y3 = y+r/2;
	if( sortdir == SORTDIRECTION_ASC ) {
		fl_polygon( x1, y1, x2, y2, x3, y3 );
	} else {
		fl_polygon( x1, y3, x2, y3, x3, y1 );
	}
}

/** Draw the row headings */
void Flx_Table::drawRowHeader( int row, const char* s, int x, int y, int w,	int h ) {
	fl_push_clip( x, y-1, w+1, h );
	Fl_Color colorMemo = color(); //dont't use fl_color(): won't compile in release config.
    
	fl_draw_box( FL_FLAT_BOX, x, y, w, h, row_header_color() );
    
    //draw horizontal line between rows and a vertical right sided line
    fl_color ( FL_GRAY );
    fl_line( x, y-1, x+w-1, y-1, x+w-1, y+h );
    
    //draw row number
	fl_color (FL_BLACK);
	fl_draw( s, x, y, w, h, FL_ALIGN_CENTER );
	fl_color( colorMemo );
	fl_pop_clip();
}

/** Handles drawing this table's cells
 Fl_Table calls this function to draw each visible cell in the table.
 It's up to us to use FLTK's drawing functions
 to draw the cells the way we want. */
void Flx_Table::draw_cell( TableContext context, int row, int col, int x, int y, int w, int h ) {

	switch ( context ) {
	case CONTEXT_STARTPAGE: // before page is drawn..
		fl_font( labelfont(), Fl_Table::labelsize() ); // set the font for our drawing operations
		return;
	case CONTEXT_COL_HEADER: // Draw column headers
		drawColumnHeader( col, _pData->getColumnHeader( col ), x, y, w, h );
		return;
	case CONTEXT_ROW_HEADER: // Draw row headers
		if( row_header_width() > 0 ) {
			drawRowHeader( row, _pData->getRowHeader( row ), x, y, w, h );
		}
		return;
	case CONTEXT_CELL: // Draw data in cells
		if( _pIn->visible() && row == _rowEdit &&  col == _colEdit ) {
            return;
        }
        
		{
			Flx_TableDrawMode drawMode = checkDraw( row, col );
			if( drawMode == FLX_DRAW_NONE ) {
				return;
			}
			fl_push_clip( x-1, y-1, w, h );
			fl_rectf( x-1, y-1, w+2, h+2,
					getCellBackground( row, col, ((drawMode == FLX_DRAW_SELECTED) ? true : false) ) );
            
			if( _pData ) { /* we have a model */
				// draw text:
				Fl_Color colorMemo = color();
				fl_color (FL_BLACK);
				fl_draw( _pData->getValue( row, col ), x + 2, y, w - 4, h, _vAlign[col], NULL, 0 );
				//fl_draw( "martin.kendel@@mail.com", x+1, y, w, h, _vAlign[col] );
				fl_color( colorMemo );
			}
            
			// Draw box border
			if( row == _markedCell.row && col == _markedCell.col ) {
				fl_rect( x, y, w, h, FL_BLUE );
				fl_rect( x+1, y+1, w-2, h-2, FL_BLUE );
			} else {
				fl_color( color() );
				//fl_rect( x, y, w, h );
                if( _drawCellBorders ) {
                    fl_rect( x-1, y-1, w+2, h+2 );
                } else {
                    fl_line( x-1, y-1, x-1, y+h );
                }
			}
			fl_pop_clip();
			return;
		}
	default:
		return;
	}
}

Flx_TableDrawMode Flx_Table::checkDraw( int row, int col ) {

	if( damage() != FL_DAMAGE_USER1 ) {
		return isSelected( row, col ) ? FLX_DRAW_SELECTED : FLX_DRAW_UNSELECTED;
	}

	//row in the row vector?
	auto rit = _switchedRows.begin();
	for( ; rit != _switchedRows.end(); rit++ ) {
		if( *rit == row ) {
			return isSelected( row, col ) ? FLX_DRAW_SELECTED : FLX_DRAW_UNSELECTED;
		}
	}

	auto cit = _switchedCols.begin();
	for( ; cit != _switchedCols.end(); cit++ ) {
		if( *cit == col ) {
			return isSelected( row, col ) ? FLX_DRAW_SELECTED : FLX_DRAW_UNSELECTED;
		}
	}

	auto it = _switchedCells.begin();
	for( ; it != _switchedCells.end(); it++ ) {
		if( (*it)->row == row && (*it)->col == col ) {
			return isSelected( row, col ) ? FLX_DRAW_SELECTED : FLX_DRAW_UNSELECTED;
		}
	}
	return FLX_DRAW_NONE;
}

Fl_Color Flx_Table::getCellBackground( int row, int col, bool isSelected ) {

	if( isSelected ) return FL_YELLOW;
    
    Fl_Color backColor = _backgroundColor;
    
    if( _isAlternatingColumnColor || _isAlternatingRowColor ) {        
        if( _isAlternatingColumnColor ) {
            int rem = col % 2;
            if( rem != 0 ) backColor = _alternatingColumnColor;
        }
        if( _isAlternatingRowColor ) {
            int rem = row % 2;
            if( rem != 0 ) backColor = _alternatingRowColor;
        }
    }
    return backColor;
}

void Flx_Table::alignContent( int colIdx, Fl_Align alignment ) {
	_vAlign[colIdx] = alignment;
}


void Flx_Table::copyToClipboard() {
    const vector<CellPtr> &cells = _selected.getSelectedCells();
    string cp;
    int r = -1, c = -1;
    for_each( cells.begin(), cells.end(), [&]( CellPtr pCell ) {
        const char *pVal = _pData->getValue( pCell->row, pCell->col );
        if( r > -1 ) {
            for( ; r < pCell->row; r++ ) {
                cp.append( NEXTROW );
            }
        } 
		if( c > -1 ) {
			c = ( pCell->col < c ) ? pCell->col : c; 
			for( ; c < pCell->col; c++ ) {
				cp.append( NEXTCOL );
			}
		}
		
        cp.append( pVal );
        r = pCell->row;
        c = pCell->col;
        
    });
    Fl::copy( cp.c_str(), cp.size(), 1 );
}

const char *Flx_Table::getNextValue( const char *pSrc, char *pVal, bool init ) {
    static const char *pPos;
    if( init ) {
        //delete[] pPos;
        pPos = pSrc;
    }
    const char *pStart = pPos;
    for( ; pPos && strcmp( pPos, NEXTROW ) && strcmp( pPos, NEXTCOL ); pPos++ );
    if( pPos ) {
        return ( ( strcmp( pPos, NEXTROW ) ) ? NEXTCOL : NEXTROW ); 
    }
    return NULL;
}

void Flx_Table::paste( const char *pClipContent ) {
    const vector<CellPtr> &cells = _selected.getSelectedCells();
    //where to start pasting:
    int r = cells[0]->row;
    int c = cells[0]->col;
    //split clip content into its parts being separated by
    //NEXTCOL or NEXTROW
    const char *pStart = pClipContent;
    const char *pEnd = pStart + strlen( pClipContent );
    const char *pPos = pClipContent;
    //fprintf( stderr, "ClipContent: %s\n", pClipContent );
    for( ; pPos < pEnd; ) {
        for( ; ( pPos < pEnd ) && *pPos != NEXTROW[0] && *pPos != NEXTCOL[0]; pPos++ );
        if( !pPos ) pPos--;
        if( pPos <= pStart ) return;
        
        //we got our string part...
        int n = pPos-pStart;
        char *pVal = new char[n+1];
        strncpy( pVal, pStart, n );
        pVal[n] = 0x0;
        //fprintf( stderr, "Paste value: %s into row %d, col %d\n", pVal, r, c );
        //...paste it:
        if( r < _pData->getRowCount() && c < _pData->getColumnCount() ) {
            _pData->setValue( pVal, r, c );
        }
        delete[] pVal;

        //find next cell to paste into:
        for( ; *pPos == NEXTROW[0] || *pPos == NEXTCOL[0]; pPos++ ) {
            if( *pPos == NEXTROW[0] ) r++;
            if( *pPos == NEXTCOL[0] ) c++;
        }
        pStart = pPos;
    }
}

void Flx_Table::pasteFromClipboard() {
	Fl_Text_Buffer buf;
	Fl_Text_Editor edi( 0, 0, 0, 0 );
	edi.buffer( buf );
	Fl::paste( edi, 1 );
	char *pClip = edi.buffer()->text();
    paste( pClip );
	redraw();
//    const char *pVal;
//    int r, c;
//    bool init = true;
//    while( ( pVal = getNextValue( pClip, r, c, init ) ) ) {
//        _pData->setValue( pVal, r, c );
//        init = false;
//    }
    
//    
//    
//    string delims( NEXTROW );
//    delims.append( NEXTCOL );
//    char *pc = strtok( txt, delims.c_str() );
//    const vector<CellPtr> &cells = _selected.getSelectedCells();
//    if( cells.size() != 1 ) {
//        fl_message( "Clipboard-Inhalt kann nur eingefügt werden,\nwenn genau 1 Zelle selektiert ist." );
//        return;
//    }
//    int r = cells[0]->row;
//    int c = cells[0]->col;
//    while( pc ) {
//        _pData->setValue( pc, r, c );
//        pc = strtok( NULL, delims.c_str() );
//    }
}

void Flx_Table::setTableData( TableData &data ) {
	_pData = &data;
	_pData->signalDataChanged.disconnect < Flx_Table, &Flx_Table::tableDataCallback >( this );
	_pData->signalDataChanged.connect < Flx_Table, &Flx_Table::tableDataCallback >( this );
	_selected.setSize( _pData->getRowCount(), _pData->getColumnCount() );
	_vAlign.clear();
	for( int c = 0, cmax = data.getColumnCount(); c < cmax; c++ ) {
		_vAlign.push_back( FL_ALIGN_LEFT );
	}
	rows( _pData->getRowCount() );
	cols( _pData->getColumnCount() );
	//adjustColWidth();
	makeColumnsFit();
//    this->labelsize( 11 );
//    row_height_all( 17 );
	redraw();
}

void Flx_Table::setTableData( DataTablePtr dataTablePtr ) {
	_dataTablePtr = dataTablePtr;
	setTableData( *_dataTablePtr );
}

void Flx_Table::labelsize( int size ) {
	Fl_Table::labelsize( size );
	fl_font( fl_font(), size );
	makeColumnsFit();	
}

void Flx_Table::makeColumnsFit() {
	for( int c = 0, cmax = cols(); c < cmax; c++ ) {
		int maxw = 0, w = 0, h = 0;
		//const char *pHeader = _pData->getColumnHeader( c );
		const char *pHeader = _pData->getColumnHeader( c );
		fl_measure( _pData->getColumnHeader( c ), maxw, h, false );
		if( maxw == 0 ) {
			fl_font( 0, 12 );
		}
		maxw += 4; //some extra space for the column header
		for( int r = 0, rmax = rows(); r < rmax; r++ ) {
			w = 0;
			//const char *pVal = _pData->getValue( r, c );
			fl_measure( _pData->getValue( r, c ), w, h, false );
			maxw = ( w > maxw ) ? w : maxw;
		}
		col_width( c, maxw+6 );
	}
}

void Flx_Table::adjustColWidth() {
	if( cols() == 0 ) return;

	int rhw = row_header() ? row_header_width() : 0;
	int kumW = rhw;
	for( int n = 0, nmax = this->cols(); n < nmax; n++ ) {
		kumW += this->col_width( n );
	}
	if( kumW < w() ) {
		int colW = (w() - rhw) / cols();
		col_width_all( colW );
	}
}

void Flx_Table::resize( int x, int y, int w, int h ) {
	
	//int cw = 0, cmax = cols();
	//for( int c = 0; c < cmax; c++ ) {
	//	cw += col_width( c );
	//}
	//int dw = w - cw;
	////printf( "n:%d w=%d, w()=%d, cw=%d, dw=%d\n", n++, w, this->w(), cw, dw );
	//if( dw > 0 ) {
	//	col_width( cmax-1, col_width( cmax-1 ) + dw );
	//}

	Fl_Table::resize( x, y, w, h );
}

TableData &Flx_Table::getTableData() const {
    return *_pData;
}

bool Flx_Table::canSelectCell( int row, int col ) {
	if( _selectionMode == FLX_SELECTION_FREE ||
		_selectionMode == FLX_SELECTION_SINGLECELL ) {
			return true;
	}
	return false;
}

bool Flx_Table::isSelected( int row, int col ) const {
	return _selected.isOn( row, col );
}

bool Flx_Table::isSortable() const {
	return _isSortable;
}


void Flx_Table::handleEvents() {
	_doCallback = false;

	if( _eventContext.tableContext == CONTEXT_COL_HEADER ) {
		if( ( Fl::event() == FL_PUSH ) && _eventContext.isButton3 ) {	
			redraw();
			showColumnHeaderContextMenu( Fl::event_x(), Fl::event_y() );
		}
	} else {
        //CONTEXT_CELL
		if( _eventContext.fl_event == FL_PUSH && _pIn->visible() ) {
			//in editing mode; stop it
			endEdit();
		} else if( _eventContext.fl_event == FL_KEYDOWN ) {
            
			if( isTriggerEditKey() ) {
				checkEdit();
			}
		}
		
		if( _eventContext.fl_event != FL_RELEASE ) {
            int n = Fl::event_clicks();
            if( n != 0 ) {
                doSelectionCallback();
            } else if( checkSelection() ) {
				redraw();
      
				if( _eventContext.isButton3 ) {
					showCellContextMenu( Fl::event_x(), Fl::event_y() );
				} else if( _eventContext.isSelectionConcerned() || _doCallback ) {
					if( Fl::event_clicks() > 0 && !hitTab() ) { //double clicked				
						checkEdit();
					} else {
						doSelectionCallback();
					}
				}
			}
		}
	}
}

bool Flx_Table::isTriggerEditKey() {
	int key = Fl::event_key();
	return ( key > 31 && key < 127 );
}

void Flx_Table::checkEdit() {
	if( _selectionMode == FLX_SELECTION_SINGLECELL ) {						
		CellPtr sel = getFirstSelectedCell();
		if( sel->row > -1 && sel->col > -1 &&
			isCellEditable( sel->row, sel->col ) ) 
		{
			startEdit( sel->row, sel->col );
		}
	}
}

void Flx_Table::startEdit( int row, int col ) {
	_rowEdit = row;
	_colEdit = col;
    makeCellEditorFit();
//	int x, y, w, h;
//	this->find_cell( CONTEXT_CELL, row, col, x, y, w, h );
//	_pIn->resize( x, y, w, h );
	string val( getTableData().getValue( row, col ) );
	_pIn->value( val.c_str() );
	_pIn->position( 0, strlen( _pIn->value() ) );
	_pIn->show();
	_pIn->take_focus();
}

void Flx_Table::makeCellEditorFit() {
    int x, y, w, h;
    this->find_cell( CONTEXT_CELL, _rowEdit, _colEdit, x, y, w, h );
	_pIn->resize( x, y, w, h );
}

void Flx_Table::endEdit() {
	getTableData().setValue( _pIn->value(), _rowEdit, _colEdit );
	_pIn->hide();
	window()->cursor( FL_CURSOR_DEFAULT );
	_eventContext.row = _rowEdit;
	_eventContext.col = _colEdit;
	_rowEdit = _colEdit = -1;
}

void Flx_Table::setEventContext() {

	int draggingRow = -1, draggingCol = -1;
	if( Fl::event() == FL_DRAG ) {
		ResizeFlag flag;
		TableContext tableContext = cursor2rowcol( draggingRow, draggingCol, flag );
		if( draggingRow >= _pData->getRowCount() ) {
			return;
		}
		if( draggingRow == _eventContext.row && draggingCol == _eventContext.col ) {
			return;
		}
	}

	int lastRow = _eventContext.row;
	int lastCol = _eventContext.col;
	_eventContext.reset();
	_eventContext.fl_event = Fl::event();
	_eventContext.tableContext = callback_context();
	if( draggingRow > -1 || draggingCol > -1 ) {
		_eventContext.row = draggingRow;
		_eventContext.col = draggingCol;
		_eventContext.isDragging = true;
	} else {
		_eventContext.row = callback_row();
		_eventContext.col = callback_col();
	}
	_eventContext.lastRow = (lastRow == -1) ? _eventContext.row : lastRow;
	_eventContext.lastCol = (lastCol == -1) ? _eventContext.col : lastCol;
	_eventContext.isDblClick = (Fl::event_clicks() > 0);
	_eventContext.isCtrl = Fl::event_ctrl() > 0 ? true : false;
	_eventContext.isShift = Fl::event_shift() > 0 ? true : false;
	_eventContext.isButton1 = Fl::event_button1() > 0 ? true : false;
	_eventContext.isButton3 = Fl::event_button3() > 0 ? true : false;
	_eventContext.key = Fl::event_key();
	_eventContext.isArrowUp = ((_eventContext.key & FL_Up) == FL_Up);
	_eventContext.isArrowDown = ((_eventContext.key & FL_Down) == FL_Down);
	_eventContext.isArrowLeft = ((_eventContext.key & FL_Left) == FL_Left);
	_eventContext.isArrowRight = ((_eventContext.key & FL_Right) == FL_Right );
	_eventContext.isTab = ((_eventContext.key & FL_Tab) == FL_Tab );

	//_eventContext.dump();
}

void Flx_Table::showColumnHeaderContextMenu( int x, int y ) {
    Flx_ColumnHeaderContextMenu colhdrMenu;
	colhdrMenu.position( Fl::event_x(), Fl::event_y() );
	colhdrMenu.setActive( MARK,  (_selectionMode == FLX_SELECTION_FREE) );
	colhdrMenu.setActive( SORT_ASC, _isSortable );
	colhdrMenu.setActive( SORT_DESC, _isSortable );
    
	createAllColumnsSubmenu( colhdrMenu );
	const Fl_Menu_Item* pItem = colhdrMenu.popup();
	
    if( pItem ) {
		ContextMenuIdent ident = (ContextMenuIdent)fl_intptr_t( pItem->user_data() );
		switch( ident ) {
		case IDENT_MARK:
			//mark column
            checkColumnSelection();
			break;
		case IDENT_COPY_WITH_SEP:
		case IDENT_COPY_WITHOUT_SEP:
			copyToClipboard( _eventContext.col, 
							 ident == IDENT_COPY_WITH_SEP ? true : false );
			break;
		case IDENT_SORT_ASC:
		case IDENT_SORT_DESC:
			if( _isSortable ) {
                _pData->sort( _eventContext.col,
                    ident == IDENT_SORT_ASC ? SORTDIRECTION_ASC : SORTDIRECTION_DESC );
            }
			break;
		case IDENT_REMOVE_COL:
			break;
		default:  //Goto Column submenu
			_gotoCol = getColumnIdByHeader( (const char *)pItem->user_data() );
			if( _gotoCol > -1 ) {
				col_position( _gotoCol );
			}
			redraw();
			break;
		}
	}
}

void Flx_Table::createAllColumnsSubmenu( Flx_ColumnHeaderContextMenu &colhdrMenu ) {
	char buf[256];
	TableData &data = this->getTableData();
	vector<const char *> headers;
	for( int c = 0, cmax = cols(); c < cmax; c++ ) {
		const char *pHeader = data.getColumnHeader( c );
		headers.push_back( pHeader );
	}

	sort( headers.begin(), headers.end(), Flx_Table::alphaSort );
	vector<const char *>::iterator itr;
	for( itr = headers.begin(); itr != headers.end(); itr++ ) {
		const char *pHeader = *itr;
		strcpy( buf, GOTO_COL );
		strcat( buf, "/" );
		strcat( buf, pHeader );
		colhdrMenu.add( buf, 0, NULL, (void*)pHeader, 0 );
	}
}

bool Flx_Table::alphaSort( const char *pVal1, const char *pVal2 ) {
	return ( strcmp( pVal1, pVal2 ) < 0 );
}

void Flx_Table::copyToClipboard( int col, bool withSeparator ) {
	string s;
	for( int r = 0, rmax = rows(); r < rmax; r++ ) {
		s.append( _pData->getValue( r, col, "" ) );
		if( withSeparator && r < rmax - 1 ) {
			s.append( ", " );
		}
		s.append( NEXTROW );
	}
	Fl::copy( s.c_str(), s.length(), 1 );
}

void Flx_Table::checkColumnSelection() {
	checkAnchor();
	if( _eventContext.isCtrl ) {
		_selected.colOn( _eventContext.col );
	} else if( _eventContext.isShift ) {
		selectMultiColumn();
	} else {
		_selected.allOff();
		_selected.colOn( _eventContext.col );
	}
	redraw();
	doSelectionCallback();
}

void Flx_Table::checkAnchor() {
	if( ! ( _eventContext.isShift || _eventContext.isDragging ) ||  _anchorRow == -1 ) {
		_anchorRow = _eventContext.row;
		_anchorCol = _eventContext.col;
	}
}

void Flx_Table::showCellContextMenu( int x, int y ) {
	_pCellMenu->setEntriesCountItem( _pData->getRowCount() );
    _pCellMenu->setActive( PASTE, true );
	_pCellMenu->position( Fl::event_x(), Fl::event_y() );
	const Fl_Menu_Item* pItem = _pCellMenu->popup();
	if( pItem ) {
        if( strcmp( pItem->label(), COPY ) == 0 ) {
            copyToClipboard();
        } else if( strcmp( pItem->label(), PASTE ) == 0 ) {
            pasteFromClipboard();
        } else if( strcmp( pItem->label(), SEARCH ) == 0 ) {
			//search table
			if( !_pSearch ) {
				_pSearch = new Flx_TableSearch( *this );
			}
			_pSearch->start();
		} 
        MenuItemAction miaction;
        miaction.ident = (int)pItem->user_data();
        //miaction.pSelectedItem = const_cast<Fl_Menu_Item*>(pItem);
        miaction.label = pItem->label();
        signalContextMenuItemSelected.send( *_pCellMenu, miaction );
	}
}

int Flx_Table::getSelectionMode() {
	return (int)_selectionMode;
}

void Flx_Table::setSelectionMode( Flx_SelectionMode mode ) {
	_selectionMode = (Flx_SelectionMode)( (mode));
	_selected.allOff();
	redraw();
}

void Flx_Table::select( int row, int col ) {
    if( _selectionMode == FLX_SELECTION_SINGLECELL || 
        _selectionMode == FLX_SELECTION_FREE )
    {
        _selected.on( row, col );
    }
}

void Flx_Table::selectRow( int row ) {
	if( _selectionMode == FLX_SELECTION_SINGLEROW ) {
		_selected.allOff();
	}
	if( _selectionMode == FLX_SELECTION_SINGLEROW || 
		_selectionMode == FLX_SELECTION_MULTIROW || 
        _selectionMode == FLX_SELECTION_FREE )
    {
		_selected.rowOn( row );
    }
}

void Flx_Table::selectColumn( int col ) {
	if( _selectionMode == FLX_SELECTION_FREE ) {
		_selected.colOn( col );
	}
}

void Flx_Table::clearSelection() {
	_selected.allOff();
	doSelectionCallback();
	redraw();
}

void Flx_Table::setAlternatingColumnColor( Fl_Color color ) {
    _alternatingColumnColor = color;
    _isAlternatingColumnColor = color == _backgroundColor ? false : true;
}

void Flx_Table::setAlternatingRowColor( Fl_Color color ) {
    _alternatingRowColor = color;
    _isAlternatingRowColor = color == _backgroundColor ? false : true;
}

void Flx_Table::drawCellBorders( bool draw ) {
    _drawCellBorders = draw;
}

void Flx_Table::mark( int row, int col ) {
	unmark();
	int r1, r2, c1, c2;
	visible_cells( r1, r2, c1, c2 );
	if( row >= r2 ) {
		int d = row - r2 + 1;
		row_position( r1 + d );
	} else if( row <= r1 ) {
		int d = r1 - row;
		row_position( r1 - d );
	}
	if( col <= c1 ) {
		int d = c1 - col;
		col_position( c1 - d );
	} else if( col >= c2 ) {
		int d = col - c2;
		col_position( c1 + d );
	}
	_markedCell.row = row;
	_markedCell.col = col;
	redraw_range( row, row, col, col );
}

void Flx_Table::unmark() {
	if( _markedCell.row > -1 ) {
		int r = _markedCell.row;
		int c = _markedCell.col;
		_markedCell.row = -1;
		_markedCell.col = -1;
		redraw_range( r, r, c, c );
	}
}

void Flx_Table::setColumnWidthRelative( int col, int percent ) {
    int w = tiw * ( (double)percent/100 );
    //int wRem = tiw - w;
    col_width( col, w );
}

void Flx_Table::tableDataCallback( my::TableData &src, my::DataChangedEvent &evt )
{
	switch( evt.dataChange ) {
    case TABLEDATA_VALUE_CHANGED:
        
        break;
    case TABLEDATA_ROW_ADDED:
		rows( ++evt.row );
		_selected.setSize( rows(), cols() );
        break;
    case TABLEDATA_COLUMN_ADDED:
        cols( ++ evt.col );
        _vAlign.push_back( FL_ALIGN_LEFT );
		_selected.setSize( rows(), cols() );
        break;
    case TABLEDATA_ROW_REMOVED:
        {
            int r = rows();
            rows( --r );
			_selected.setSize( rows(), cols() );
            break;
        }
	case TABLEDATA_ALL_ROWS_REMOVED:
		rows( 0 );
		_selected.setSize( 0, cols() );
		break;
    case TABLEDATA_COLUMN_REMOVED:
        {
            int c = cols();
            cols( --c );
			_selected.setSize( rows(), cols() );
            _vAlign.erase( _vAlign.begin() + evt.col );
            break;
        }
    case TABLEDATA_CLEARED:
        rows( 0 );
        cols( 0 );
		_selected.setSize( rows(), cols() );
        _vAlign.clear();
        break;
    case TABLEDATA_CHANGED:
        onTableDataChanged();
        break;
    case TABLEDATA_SORTED:
        break;
    default:
        throw( "Flx_Table::tableDataCallback: no such TableDataChange" );
    }

    redraw();
}

void Flx_Table::clear() {	
	rows( 0 );
	cols( 0 );
	_selected.allOff();
	_selected.setSize( 0, 0 );
	_vAlign.clear();
    //_isAlternatingColumnColor = false;
    //_isAlternatingRowColor = false;
}

void Flx_Table::onTableDataChanged() {
    clear();
	//_selected.allOff();
    rows( _pData->getRowCount( ) );
    cols( _pData->getColumnCount( ) );
	_selected.setSize( rows(), cols() );
    //row_height_all( 18 ); don't use this method, it's *very* slow
    //int cw = ( w( ) - row_header_width( ) ) / _pData->getColumnCount( );
    //col_width_all( cw );
	makeColumnsFit();
}

void Flx_Table::setSortable(bool sortable) {
    _isSortable = sortable;
}

//int Flx_Table::handle( int evt ) {
//    int rc = Fl_Table::handle( evt );
//	if( Fl::event_key() == FL_Escape ) return 1;
//    int rowTop, colLeft, rowBot, colRight;
//    get_selection( rowTop, colLeft, rowBot, colRight );
//    string name = "<unknown>";
//    Fl_Widget *w = Fl::focus();
//    if( w ) {
//        name = typeid( *w ).name();
//    } 
//    //fprintf( stderr, "Focus: %s\tselection: von %d/%d bis %d/%d\n", name.c_str(), rowTop, colLeft, rowBot, colRight );
//    return rc;
//}


int Flx_Table::handle( int evt ) {
    int rc = Fl_Table::handle( evt );
	if( Fl::event_key() == FL_Escape ) return 1;
    
    if( evt == FL_UNFOCUS ) {
        string name = "<unknown>";
        Fl_Widget *w = Fl::focus();
        if( w ) {
            name = typeid( *w ).name();
        } 
        //fprintf( stderr, "Focus has %s\n", name.c_str() );
        
        //Fl::focus( this );
        return 0;
    }
    if( Fl::focus() == _pIn && 
        (evt == FL_KEYDOWN || evt == FL_RELEASE) && 
        (Fl::event_key() == FL_Tab || Fl::event_key() == FL_Enter ) ) 
    {
        return 1;
    }

    //if( evt != FL_MOVE && evt != FL_NO_EVENT )  fprintf( stderr, "event = %d\n", evt );
    
	if( evt == FL_DRAG || evt == FL_KEYDOWN || evt == FL_PUSH || evt == FL_RELEASE ) {
        if( _gotoCol > -1 ) _gotoCol = -1; //reset column headers color
        
        setEventContext();
		handleEvents();
        CellPtr pCell = getFirstSelectedCell();
        //fprintf( stderr, "event was: %d, selected cell: r/c = %d, %d\n", evt, pCell->row, pCell->col );
        
		return 1;
	}
	
    return rc;
}

/** Cares about de-selecting and selecting.
    Selection after hitting TAB and ENTER is not handled here. 
    See handleTab and handleEnter.
 */
bool Flx_Table::checkSelection() {
	if( ( _selectionMode == FLX_SELECTION_SINGLECELL ||
		  _selectionMode == FLX_SELECTION_FREE ) &&
		  !canSelectCell( _eventContext.row, _eventContext.col ) ||
		  _selectionMode == FLX_SELECTION_NONE )
	{
		return false;
	}

	Fl_Damage dmg = FL_DAMAGE_USER1;
	if( _selectionMode == FLX_SELECTION_SINGLECELL 	|| 
        _selectionMode == FLX_SELECTION_SINGLEROW || 
        (! ( _eventContext.isCtrl || _eventContext.isShift || _eventContext.isDragging ) ) ) 
	{
		//if it was a right click into a selected cell selection will not be affected:
		if( _eventContext.isButton3 && 
			_selected.isOn( _eventContext.row, _eventContext.col ) ) 
		{
			;
		} else {
			_selected.allOff();
			_doCallback = true;
			dmg = FL_DAMAGE_ALL;
		}
	}

	if( !_eventContext.isSelectionConcerned() ) {
		//_selected.allOff();
		return false;
	}

	checkAnchor();

	if( _eventContext.tableContext == CONTEXT_CELL ) {
		if( _eventContext.isMouseClick() || 
            _eventContext.isArrowKey() ||
            _eventContext.isTab ||
            _eventContext.isDragging ) 
        {
			if( _eventContext.isShift || _eventContext.isDragging ) {
				if( _selectionMode == FLX_SELECTION_SINGLEROW ) {
					toggleRowSelection( _eventContext.row );
				} else if( _selectionMode == FLX_SELECTION_MULTIROW ) {
					selectMultiRow();
				} else if( _selectionMode == FLX_SELECTION_FREE ) {
					//CONTEXT_CELL; click || arrowkey; shift
					_selected.areaOff( _anchorRow, _anchorCol,
							           _eventContext.lastRow, _eventContext.lastCol );
					_selected.areaOn( _anchorRow, _anchorCol,
							          _eventContext.row, _eventContext.col );
				} else {
					// neither FLX_SELECTION_SINGLEROW nor FLX_SELECTION_MULTIROW
					_selected.on( _eventContext.row, _eventContext.col );
				}
			} else if( _eventContext.isCtrl ) {
				if( _selectionMode == FLX_SELECTION_SINGLEROW || 
                    _selectionMode == FLX_SELECTION_MULTIROW ) 
                {
					toggleRowSelection( _eventContext.row );
				} else if( _selectionMode == FLX_SELECTION_FREE ) {
					_selected.toggle( _eventContext.row, _eventContext.col );
				} else {
					_selected.on( _eventContext.row, _eventContext.col );
				}
			} else {
				//a simple mouse click into a cell
				if( _selectionMode == FLX_SELECTION_SINGLEROW || 
                    _selectionMode == FLX_SELECTION_MULTIROW ) 
                {
					_selected.rowOn( _eventContext.row );
				} else  {
					_selected.on( _eventContext.row, _eventContext.col );
				}
			}
		} //if( _eventContext.isMouseClick()  || _eventContext.isArrowKey() )
	} else if( _eventContext.tableContext == CONTEXT_ROW_HEADER ) {
		if( _eventContext.isMouseClick() ) {
			if( _eventContext.isShift || _eventContext.isDragging ) {
				if( _selectionMode == FLX_SELECTION_MULTIROW ||
					_selectionMode == FLX_SELECTION_FREE )
				{
					selectMultiRow();
				} else if( _selectionMode != FLX_SELECTION_SINGLECELL ) {
					_selected.rowOn( _eventContext.row );
				}
			} else if( _eventContext.isCtrl ) {
				_selected.rowOn( _eventContext.row );
			} else /*if( _selectionMode != FLX_SELECTION_SINGLECELL )*/ {
				//a simple mouse click into a row header
				_selected.rowOn( _eventContext.row );
			}
		}

//	} else if( _eventContext.tableContext == CONTEXT_TABLE &&
//			  (_eventContext.key & FL_Tab) == FL_Tab  ) 
//	{
//		//just having ended edit mode by hitting TAB
//		_selected.on( _eventContext.row, _eventContext.col );
	} else {
		throw ( "FLx_Table::checkSelection(): don't expect you to be here" );
	}

	return true;
}

void Flx_Table::toggleRowSelection( int row ) {
	if( _selected.isRowOn( row ) ) {
		_selected.rowOff( row );
	} else {
		_selected.rowOn( row );
	}
}

void Flx_Table::selectMultiRow() {
	int r1 = _selected.getMinRowOn();
	r1 = ( r1 == -1 ) ? _eventContext.row : r1;
	int r2 = _eventContext.row;
	if( r1 > r2 ) {
		int tmp = r1;
		r1 = r2;
		r2 = tmp;
	}
	for( ; r1 <= r2; r1++ ) {
		_selected.rowOn( r1 );
	}
}

void Flx_Table::selectMultiColumn() {
	int mincol = _eventContext.col < _anchorCol ? _eventContext.col : _anchorCol;
	int maxcol = mincol == _anchorCol ? _eventContext.col : _anchorCol;
	for( int c = mincol; c <= maxcol; c++ ) {
		_selected.colOn( c );
	}
}

/** open the search dialog */
void Flx_Table::searchTable() {
}

CellPtr Flx_Table::getFirstSelectedCell() const {
	return _selected.getFirstSelectedCell();
}

const std::vector<CellPtr>& Flx_Table::getSelectedCells() {
	return _selected.getSelectedCells();
}

void Flx_Table::dumpSelection() const {
    _selected.dumpSelection();
}

const vector<int>& Flx_Table::getSelectedRows() {
	return _selected.getSelectedRows();
}

const vector<int>& Flx_Table::getSelectedColumns() {
	return _selected.getSelectedColumns();
}


//Flx_CellContextMenu &Flx_Table::getCellContextMenu() {
//	return _cellMenu;
//}
//
//Flx_ColumnHeaderContextMenu &Flx_Table::getColumnHeaderContextMenu() {
//	return _colhdrMenu;
//}

int Flx_Table::getColumnIdByHeader( const char *pHeader ) {
	for( int c = 0, cmax = cols(); c < cmax; c++ ) {
		if( !strcmp( _pData->getColumnHeader( c ), pHeader ) ) {
			return c;
		}
	}
	return -1;
}

void Flx_Table::onShortcut( Fl_Menu_ &, MenuItemAction &action ) {
    switch( action.ident ) {
        case IDENT_COPY:
            copyToClipboard();
            break;
        case IDENT_PASTE:
            break;
        case IDENT_SEARCH:
            break;
        default:
            break;
    }
}

Fl_Scrollbar *Flx_Table::getHorzScrollbar() {
	for( int i = 0, imax = children(); i < imax; i++ ) {
		Fl_Widget *pW = child( i );
		if( typeid( *(pW) ).name() == typeid( Fl_Scrollbar ).name() ) {
			return (Fl_Scrollbar*)pW;
		}
	}
	return NULL;
}

void Flx_Table::doSelectionCallback() {
	SelectionEvent se;
	se.doubleClicked = ( Fl::event_clicks() == 0 ) ? false : true;
	signalSelected.send( this, &se );
}

} //namespace myfltk
