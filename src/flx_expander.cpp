

#include <flx/flx_expander.h>
#include <images/expander_open.xpm>
#include <images/expander_close.xpm>
#include <flx/Flx_Button.h>
#include <flx/Flx_ContextMenu.h>
#include <flx/Flx_Table.h>
#include <flx/Flx_Dialog.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Button.H>

#include <algorithm>
#include <cstring>

using namespace my;
using namespace std;

namespace flx {

Flx_ExpanderContentGroup::Flx_ExpanderContentGroup( int row, int col, int w, int h, const char *pLbl ) 
    : Fl_Group( 0, 0, w, h, pLbl )
    , _row( row )
    , _col( col )
{
    box( FL_BORDER_BOX );
    color( FL_WHITE );
    align( FL_ALIGN_INSIDE | FL_ALIGN_CENTER );
    begin();
}
void Flx_ExpanderContentGroup::resize( int x, int y, int w, int h ) {
	Fl_Group::resize( x, y, w, h );
    //printf( "Flx_ExpanderContentGroup::resize: w=%d\n", w );
}
Position Flx_ExpanderContentGroup::getPosition() const {
    Position pos = {_row, _col};
    return pos;
}
void Flx_ExpanderContentGroup::add( Fl_Widget *pW ) {
	add( *pW );
}
void Flx_ExpanderContentGroup::add( Fl_Widget &w ) {
	if( visible() ) throw "An Flx_ExpanderContentGroup cannot be changed while beeing shown.";
	w.size( this->w(), w.h() );
    size( this->w(), w.h() );   
	Fl_Group::add( w );
}


////////////////////////////////////////

Flx_ExpanderHeader::Flx_ExpanderHeader(int x, int y, int w, int h, const char* pLbl, 
        Flx_ExpanderContentGroup *pContentGrp ) 
    : Fl_Group( x, y, w, h, pLbl )
    , _isExpanded( false )
    , _pContentGrp( pContentGrp )
	, _isActive( true )
{
    box( FL_FLAT_BOX );
    color( fl_lighter( FL_GRAY ) );
    align( FL_ALIGN_INSIDE | FL_ALIGN_CENTER );
    _pPixbox = new Fl_Box( x, y, h, h );
    _pOpenXpm = new Fl_Pixmap( expander_open_xpm );
	_pCloseXpm = new Fl_Pixmap( expander_close_xpm );
	_pPixbox->image( _pOpenXpm );
    int X = _pPixbox->x() + _pPixbox->w();
    Fl_Group *pDummy = new Fl_Group( X, y, w - _pPixbox->w(), 1 );
    
    //int X = _pPixbox->x() + _pPixbox->w();
    //_pLabelBtn = new Fl_Button( X, y, w - x, h, pLbl );
    //_pLabelBtn->box( FL_FLAT_BOX );
    end();
    resizable( pDummy );

    _pContextMenu = new Flx_ContextMenu();
    _pContextMenu->addMenuItem( "In Dialog anzeigen...", 0, 1, -1, NULL, NULL, FL_MENU_DIVIDER );
    _pContextMenu->addMenuItem( "Schrift größer", 0, 2 );
    _pContextMenu->addMenuItem( "Schrift kleiner", 0, 3 );
}
int Flx_ExpanderHeader::handle( int evt ) {
	switch( evt ) {
	case FL_ENTER:
		if( _isActive ) {
			fl_cursor( FL_CURSOR_HAND );
			labelcolor( FL_RED );
			damage( FL_DAMAGE_ALL );
		} 
		break;
	case FL_LEAVE:
		if( _isActive ) {
			fl_cursor( FL_CURSOR_DEFAULT );
			labelcolor( FL_BLACK );
			damage( FL_DAMAGE_ALL );
		}
		break;
	case FL_PUSH:
        if( _isActive ) {
            if( Fl::event_button1() ) {
                NULLPARM pp;
                signalHeaderPushed.send( this, &pp );
				toggleExpandClose();
             /*   _isExpanded = !_isExpanded;
                _pPixbox->image( _isExpanded ? _pCloseXpm : _pOpenXpm );
                redraw();*/
            } else if( Fl::event_button3() && _isExpanded ) {
                showContextMenu();
            }
			return 1;
		}
		break;
	default:
		break;
	}
    int rc = Fl_Group::handle( evt );
	return rc;
}
void Flx_ExpanderHeader::toggleExpandClose() {
	_isExpanded = !_isExpanded;
    _pPixbox->image( _isExpanded ? _pCloseXpm : _pOpenXpm );
    redraw();
}
void Flx_ExpanderHeader::collapse( bool deactivate ) {
	if( _isExpanded ) {
		NULLPARM pp;
        signalHeaderPushed.send( this, &pp );
		_isExpanded = false;
		_pPixbox->image( _pCloseXpm );
		redraw();
	}
	setActive( false );
}
void Flx_ExpanderHeader::expand() {
	if( !_isExpanded ) {
		NULLPARM pp;
        signalHeaderPushed.send( this, &pp );
		_isExpanded = true;
		_pPixbox->image( _pOpenXpm );
		redraw();
	}
	setActive( false );
}
void Flx_ExpanderHeader::draw() {
	Fl_Group::draw();
	int X = x(), Y = y(), W = w(), H = h();
	int X2 = X + W - 1;
	int Y2 = Y + H - 1;
	fl_rect( X, Y, W, H, FL_GRAY );
}
void Flx_ExpanderHeader::resize( int x, int y, int w, int h ) {
	Fl_Group::resize( x, y, w, this->h() );
}

void Flx_ExpanderHeader::setActive( bool active ) { 
	_isActive = active; 
	labelcolor( !active ? fl_darker( FL_GRAY ) : FL_BLACK );
	damage( FL_DAMAGE_ALL );
}

//void Flx_ExpanderHeader::setContextMenu( Flx_ContextMenu *pContextMenu ) {
//    _pContextMenu = pContextMenu;
//}

//Flx_ContextMenu &Flx_ExpanderHeader::getContextMenu() const {
//    return *_pContextMenu;
//}

void Flx_ExpanderHeader::showContextMenu() {
    _pContextMenu->position( Fl::event_x(), Fl::event_y() );
    const Fl_Menu_Item *pItem = _pContextMenu->popup();
    if( pItem ) {
        MenuItemAction &action = _pContextMenu->getAction( pItem );
        int id = action.ident;
        //signalMenuCommand.send( this, &id );
        handleContextMenuCommand( action.ident );
    }
}
void Flx_ExpanderHeader::handleContextMenuCommand(int ident) {
    switch ( ident ) {
        case 1:
			{
				//show in dialog
				Flx_Dialog dlg( 100, 100, 600, 800 );
				int x, y, w, h;
				dlg.getClientArea( x, y, w, h );
				Fl_Widget *pW = this->getContentGroup().child( 0 );
				if( !pW ) break;

				remove( pW );
				pW->resize( x, y, w, h );
				dlg.add( pW );
				dlg.show( true );
			}
            break;
        case 2:
        case 3:
            {
                Fl_Widget *pW = getContentGroup().child(0);
				if( !pW ) break;

                //enlarge/shrink font
                Flx_Base *pBase = dynamic_cast< Flx_Base* >( pW );
                if( pBase ) {
                    if( ident == 2 ) {
                        pBase->scaleUp();
                    } else {
                        pBase->scaleDown();
                    }
                } else {
                    int f = (ident == 2 ) ? 1 : -1;
                    pW->labelsize( pW->labelsize() + 2 * f );
                }
                pW->redraw();
            }
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////

Flx_ExpanderGroup::Flx_ExpanderGroup( int x, int y, int w, int h, const char *pLbl, int columns ) 
	: Fl_Group( x, y, w, h, pLbl ) 
	, _spacingC( 35 )
	, _spacingR( 25 )
	, _marginL( 15 )
	, _marginR( 15 )
	, _marginT( 15 )
	, _colCnt( columns )
	//, _colWidth( w/columns )
    , _headerH( 30 )
{
	box( FL_FLAT_BOX );
	color( FL_WHITE );
	align( FL_ALIGN_INSIDE | FL_ALIGN_CENTER );	
}
void Flx_ExpanderGroup::end() {
	Fl_Group::end();
}
void Flx_ExpanderGroup::resize( int x, int y, int w, int h ) {
	Fl_Group::resize( x, y, w, h );
    //printf( "Flx_ExpanderGroup::resize: h=%d\n", h );
}

void Flx_ExpanderGroup::onExpandedOrCollapsed( Flx_ExpanderHeader &hdr, NULLPARM & ) {
	Flx_ExpanderContentGroup &grp = hdr.getContentGroup();
	Flx_ExpandParm expandParm = {grp, false, false};
    releaseChildren();
    if( hdr.isExpanded() ) {
		//going to collapse
		expandParm.beforeCollapsing = true;
		signalBeforeExpandOrCollapse.send( this, &expandParm );
        hdr.getContentGroup().hide();        
    } else {
		//going to expand     
		expandParm.beforeExpanding = true;
		signalBeforeExpandOrCollapse.send( this, &expandParm );
        grp.show();        
    }

	int needH = getNeededHeight();               
	size( this->w(), needH );        
	
    grabChildren();
	
	int x = hdr.x(), y = hdr.y(), h = hdr.h(), h2 = grp.h();
	for( int i = 0, imax = children(); i < imax; i++ ) {
		Flx_ExpanderHeader *pHdr = dynamic_cast< Flx_ExpanderHeader* >( child( i ) );
		if( pHdr && pHdr->x() == x && pHdr->y() > y ) {
			//needs re-positioning
            int newY = hdr.isExpanded() ? pHdr->y() - h2 : pHdr->y() + h2;
            pHdr->position( x, newY );
            Flx_ExpanderContentGroup &contgrp = pHdr->getContentGroup();			
			contgrp.position( contgrp.x(), newY + _headerH );      			
		}
	}
	parent()->damage( FL_DAMAGE_ALL );
}

int Flx_ExpanderGroup::getNeededHeight() const {
	if( _colCnt == 0 ) return 0;

	int *pHeight = new int[_colCnt];
	int initH = /*y() +*/ _marginT;
	memset( pHeight, 0, _colCnt * sizeof( int ) );
    vector< Flx_ExpanderHeader* >::const_iterator itr = _expanderHeaders.begin();
    for( ; itr != _expanderHeaders.end(); itr++ ) {		
		const Flx_ExpanderContentGroup &grp = (*itr)->getContentGroup();
		int col = grp.getPosition().col;
		//header needed space:
		if( pHeight[col] == 0 ) {
			//found first header in column
			pHeight[col] = initH;
		}
		pHeight[col] += ( _headerH  + _spacingR );
		if( grp.visible() ) {			
			pHeight[col] += grp.h();
		} 
    }

	//which column needs most height:
	int maxH = 0;
	for( int c = 0; c < _colCnt; c++ ) {
		maxH = pHeight[c] > maxH ? pHeight[c] : maxH;
	}

	delete[] pHeight;
	return maxH;
}

//void Flx_ExpanderGroup::setColumnWidth( int w ) {
//    _colWidth = w;
//}

int Flx_ExpanderGroup::getColumnWidth() const {
	return ( this->w() - _marginL - _marginR - ((_colCnt-1) * _spacingC) ) / _colCnt;
}

void Flx_ExpanderGroup::setColumnCount( int cnt ) {
	if( _expanderHeaders.size() > 0 ) 
		throw "Change of column count not allowed after having added content groups.";

	_colCnt = cnt;
	//_colWidth = w() / _colCnt;
}

void Flx_ExpanderGroup::add( Flx_ExpanderContentGroup *pGrp ) {
    Position pos = pGrp->getPosition();
	int colWidth = getColumnWidth();
    int X = x() + _marginL + pos.col * colWidth +  pos.col * _spacingC;
    int Y = y() + _marginT + pos.row * _headerH + pos.row * _spacingR;
    //create a Flx_ExpanderHeader for this content group:
    Flx_ExpanderHeader *pHeader = new Flx_ExpanderHeader( X, Y, colWidth /*pGrp->w()*/, 
												          _headerH, pGrp->label(), pGrp );
    pHeader->signalHeaderPushed.
        connect< Flx_ExpanderGroup, &Flx_ExpanderGroup::onExpandedOrCollapsed >( this );
    Fl_Group::add( pHeader );
    Fl_Group::add( pGrp );
    //pGrp->position( X, Y + _headerH );
	pGrp->resize( X, Y + _headerH, colWidth, pGrp->h() );
    pGrp->hide();
    
    _expanderHeaders.push_back( pHeader );
    
    //adjust height of this Flx_ExpanderGroup
    int h = _marginT + getRowCount() * ( _headerH + _spacingR );
    releaseChildren();
    size( this->w(), h );
    grabChildren();
}


int Flx_ExpanderGroup::getRowCount() const {
    int maxi = -1;
    for( int i = 0, imax = children(); i < imax; i++ ) {
        Flx_ExpanderHeader *pHeader = dynamic_cast< Flx_ExpanderHeader* >( child( i ) );
        if( pHeader ) {
            int r = pHeader->getContentGroup().getPosition().row;
            maxi = r > maxi ? r : maxi;
        }
    }
    return ++maxi;
}
void Flx_ExpanderGroup::releaseChildren() {
    for( int i = 0, imax = children(); i < imax; i++ ) {
        Flx_ExpanderHeader *pHeader = dynamic_cast< Flx_ExpanderHeader* >( child( i ) );
        if( pHeader ) {
            remove( pHeader );
            --i; --imax;
        } else {
            Flx_ExpanderContentGroup *pGrp = dynamic_cast< Flx_ExpanderContentGroup* >( child( i ) );
            if( pGrp ) {
                remove( pGrp );
                --i; --imax;
            }
        }
    }
}
void Flx_ExpanderGroup::grabChildren() {
    vector< Flx_ExpanderHeader* >::iterator itr = _expanderHeaders.begin();
    for( ; itr != _expanderHeaders.end(); itr++ ) {
        Flx_ExpanderHeader *pHeader = *itr;
        Fl_Group::add( pHeader );
        Fl_Group::add( pHeader->getContentGroup() );
    }
}

void Flx_ExpanderGroup::setActive( int row, int col, bool active ) {
	Flx_ExpanderHeader &hdr = findHeader( row, col );
	hdr.setActive( active );
	damage( FL_DAMAGE_CHILD );
	window()->redraw();
}

bool Flx_ExpanderGroup::isActive( int row, int col ) const {
	const Flx_ExpanderHeader &hdr = findHeader( row, col );
	return hdr.isActive();
}

Flx_ExpanderContentGroup &Flx_ExpanderGroup::getContentGroup( int row, int col ) const {
	Flx_ExpanderHeader &hdr = findHeader( row, col );
	return hdr.getContentGroup();
}

void Flx_ExpanderGroup::collapse( int row, int col, bool deactivate ) {
	Flx_ExpanderHeader &hdr = findHeader( row, col );
	hdr.collapse( deactivate );
}

void Flx_ExpanderGroup::expand( int row, int col ) const {
	Flx_ExpanderHeader &hdr = findHeader( row, col );
	hdr.expand();
}

void Flx_ExpanderGroup::
	getExpandedContentGroups( vector< Flx_ExpanderContentGroup* > &grps ) const 
{
	for_each( _expanderHeaders.begin(), _expanderHeaders.end(), [&grps]( Flx_ExpanderHeader *pHeader ) {
		Flx_ExpanderContentGroup &grp = pHeader->getContentGroup();
		if( grp.visible() ) {
			grps.push_back( &grp );
		}
	} );
}

Flx_ExpanderHeader &Flx_ExpanderGroup::findHeader( int row, int col ) const {
	vector< Flx_ExpanderHeader* >::const_iterator itr = _expanderHeaders.begin();
	for( ; itr != _expanderHeaders.end(); itr++ ) {
		Flx_ExpanderContentGroup &contgrp = (*itr)->getContentGroup();
		Position pos = contgrp.getPosition();
		if( pos.row == row && pos.col == col ) {
			return **itr;
		}
	}
	throw( "Flx_ExpanderGroup::findHeader(): not found." );
}


/////////////////////////////////////////////////////

   Flx_ExpanderScroll::Flx_ExpanderScroll( int x, int y, int w, int h )
            : Fl_Scroll( x, y, w, h )
            , _pGrp( NULL )
    {
             
    }

    void Flx_ExpanderScroll::setExpanderGroup( Flx_ExpanderGroup *pGrp ) {
        _pGrp = pGrp;
    }

    void Flx_ExpanderScroll::resize( int x, int y, int w, int h ) {
        int dw = w - this->w();
        if( dw != 0 ) {
//            fprintf( stderr, "Flx_ExpanderScroll::resize: dw=%d, new w=%d, new h=%d\n", 
//                     dw, _pGrp->w() + dw, _pGrp->h() );
            _pGrp->size( _pGrp->w() + dw, _pGrp->h() );
        }
        Fl_Scroll::resize( x, y, w, h );        
    }


} //ns flx
