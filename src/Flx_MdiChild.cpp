//#include <flx/Flx_MdiChild.h>
#include <src/Flx_MdiChild.h>
#include <flx/flx_boxtypes.h>
#include <flx/Flx_Button.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Scroll.H>
#include <FL/Enumerations.H>

namespace flx {

Flx_TitleBar::Flx_TitleBar( int x, int y, int w, const char *pTitle ) 
	: Flx_Group( x, y, w, _titleHeight )
	, _titleSize( 11 )
{
	box( FLX_GRADIENT_BOX );
	color( fl_darker( FL_BLUE ) );
    int systemBoxWidth = 69;
	begin();
		_pImageBox = new Fl_Box( x+3, y+3, 19, 19 );
		_pImageBox->box( FL_FLAT_BOX );
		_pImageBox->color( fl_lighter( FL_BLUE ) );
        
//        _pTitleGroup = new Flx_Group( _pImageBox->x() + _pImageBox->w() + 3,
//                                      _pImageBox->y(),
//                                      this->w() - _pImageBox->w() - systemBoxWidth,
//                                      _pImageBox->h(),
//                                      pTitle );  
//        _pTitleGroup->align( FL_ALIGN_CENTER | FL_ALIGN_INSIDE );
//        _pTitleGroup->labelcolor( FL_BLACK );
//        _pTitleGroup->box( FL_FLAT_BOX /*box()*/ );
//        _pTitleGroup->color( FL_WHITE /*color()*/ );
//        _pTitleGroup->end();
	
		if( pTitle ) _title.add( pTitle );

		int btnX = x + w - systemBoxWidth;
        _pMinBtn = new Flx_Button( btnX, y+2, 20, 20, "_" );
        _pMinBtn->box( FLX_GRADIENT_BOX );
        _pMinBtn->color( fl_darker( FL_BLUE ) );
		_pMinBtn->labelcolor( FL_WHITE );
		_pMinBtn->visible_focus( false );
        _pMinBtn->tooltip( "Fenster minimieren" );
        _pMinBtn->signalSelected.
                connect< Flx_TitleBar, &Flx_TitleBar::onControlButton >( this );
        
        btnX = _pMinBtn->x() + _pMinBtn->w();
        _pMaxBtn = new Flx_Button( btnX, y+2, 20, 20, "+" );
        _pMaxBtn->box( FLX_GRADIENT_BOX );
        _pMaxBtn->color( fl_darker( FL_BLUE ) );
		_pMaxBtn->labelcolor( FL_WHITE );
		_pMaxBtn->visible_focus( false );
        _pMaxBtn->tooltip( "Fenster maximieren" );
        _pMaxBtn->signalSelected.
                connect< Flx_TitleBar, &Flx_TitleBar::onControlButton >( this );
        
        btnX = _pMaxBtn->x() + _pMaxBtn->w();
		_pCloseBtn = new Flx_Button( btnX, y + 2, 20, 20, "x" );
		_pCloseBtn->box( FLX_GRADIENT_BOX );
		_pCloseBtn->color( fl_darker( FL_BLUE ) );
		_pCloseBtn->labelcolor( FL_WHITE );
		_pCloseBtn->visible_focus( false );
        _pCloseBtn->tooltip( "Fenster schließen" );
		_pCloseBtn->signalSelected.
                connect< Flx_TitleBar, &Flx_TitleBar::onControlButton >( this );
	
	end();
    resizable( _pTitleGroup );
}

void Flx_TitleBar::onControlButton( Flx_Button &btn, ActionParm & ) {
	ControlBoxAction action;
	if( &btn == _pCloseBtn ) {
		action.action = CONTROLBOXACTION_CLOSE;
	} else if( &btn == _pMinBtn ) {
        action.action = CONTROLBOXACTION_MINI;
    } else {
        action.action = CONTROLBOXACTION_MAXI;
    }
    signalControl.send( this, &action );
}

void Flx_TitleBar::draw() {
	Fl_Group::draw();
	fl_color( FL_BLUE );
	fl_draw( _title.get(), _pImageBox->x() + _pImageBox->w() + 6, y() + 18 );
}

bool Flx_TitleBar::isMouseOverControlBox() const {
	Fl_Widget *pW = Fl::belowmouse();
	if( pW == _pMinBtn || pW == _pMaxBtn || pW == _pCloseBtn ) {
		return true;
	}
	return false;
}

bool Flx_TitleBar::isMouseOverTitle() const {
//	Fl_Widget *pW = Fl::belowmouse();
//	if( pW == _pTitleGroup ) {
		return true;
//	}
//	return false;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Flx_MdiChild::Flx_MdiChild( int x, int y, int w, int h, const char *pLabel ) 
	: Fl_Group( x, y, w, h, pLabel )
	, _mousePos( POS_ANY )
	, _dragging( false )
	, _x( -1 )
	, _y( -1 )
{
    align( FL_ALIGN_INSIDE | FL_ALIGN_CENTER );
	begin();
        box( FL_DOWN_BOX );
        color( FL_WHITE );
		_pTitleBar = new Flx_TitleBar( x, y, w, pLabel );
		_pTitleBar->signalControl.connect< Flx_MdiChild, &Flx_MdiChild::onControl >( this );
//        _pClientScroll = new Fl_Scroll( x, y + _pTitleBar->h(), w, h - _pTitleBar->h() );
//		
//		_pClientScroll->box( FL_FLAT_BOX );
//		_pClientArea = new Fl_Group( _pClientScroll->x(), 
//                                     _pClientScroll->y(), 
//                                     _pClientScroll->w(),
//                                     _pClientScroll->h() );
        _pClientArea = new Fl_Group(  x+3, y + _pTitleBar->h(), w-6, h-3 - _pTitleBar->h() );
        _pClientArea->box( FL_FLAT_BOX );
		_pClientArea->end();
//        _pClientScroll->end();
//        _pClientScroll->box( FL_DOWN_BOX );
//        _pClientScroll->color( FL_WHITE );
	end();
	//resizable( _pClientScroll );
    resizable( _pClientArea );
}

Fl_Group &Flx_MdiChild::getClientArea() const {
	return *_pClientArea;
}

Fl_Group &Flx_MdiChild::getClientAreaContent() const {
	return  *(dynamic_cast< Fl_Group* > (_pClientArea->child( 0 )));
}

void Flx_MdiChild::onControl( Flx_TitleBar &, ControlBoxAction &action ) {
	if( action.action == CONTROLBOXACTION_CLOSE ) {
		signalControlBoxAction.send( this, &action );
	} else if( action.action == CONTROLBOXACTION_MINI ) {
        hide();
        signalControlBoxAction.send( this, &action );
    } else {
        //maximize
        Fl_Group::resize( parent()->x(), parent()->y(), parent()->w(), parent()->h() );
        redraw();
    }
}

int Flx_MdiChild::handle( int evt ) {
	switch( evt ) {
	case FL_MOVE:
		_mousePos = checkResizeCursor();
		break;
	case FL_LEAVE:
		fl_cursor( FL_CURSOR_DEFAULT );
		break;
    case FL_PUSH:
		{
			_x = Fl::event_x();
			_y = Fl::event_y();
            
            if( isMouseOverMeOrOneOfMyChildren() ) {
                bringMeOnTop();
                damage( FL_DAMAGE_ALL );
                
            } else {
                return 1;
            }

			if( _mousePos == POS_ANY ) {
                if( _pTitleBar->isMouseOverTitle() ) {
					_dragging = true;
					fl_cursor( FL_CURSOR_HAND );
                    return 1;
				}
			} else return 1; //resizing
           
        }
		break;
	case FL_RELEASE:
		_dragging = false;
		_mousePos = POS_ANY;
		break;
	case FL_DRAG:
		if( _dragging ) {
			drag();
		} else if( _mousePos != POS_ANY ) {
			resize();
		}
		break;
	default:
		break;
	}
	
	return Fl_Group::handle( evt );
}

bool Flx_MdiChild::bringMeOnTop() {
    Fl_Group *pParent = parent();
    int n = pParent->find( this );
    int nChildren = pParent->children();
    if( n == nChildren - 1 ) return false;
    
    pParent->remove( this );
    pParent->add( this );
    pParent->redraw();
    return true;
}

bool Flx_MdiChild::isMouseOverMeOrOneOfMyChildren() const {
    Fl_Widget *p = Fl::belowmouse();
    return isMouseOverMeOrOneOfMyChildren( p, this );
}

bool Flx_MdiChild::isMouseOverMeOrOneOfMyChildren( const Fl_Widget *pBelowMouse, 
                                                   const Fl_Group *pCheck ) const 
{   
    for( int i = 0, imax = pCheck->children(); i < imax; i++ ) {
        Fl_Widget *pChild = pCheck->child( i );
        if( pChild == pBelowMouse ) {
            fprintf( stderr, "this: %s, belowMouse: %s\n", this->label(), typeid( *pBelowMouse).name() );
            return true;
        } else {
            Fl_Group *pG = dynamic_cast< Fl_Group* >( pChild );
            if( pG ) {
                if( isMouseOverMeOrOneOfMyChildren( pBelowMouse, pG ) ) return true;
            }
        }
    }
    return false;
}

MousePosition Flx_MdiChild::checkResizeCursor() {
	int x = Fl::event_x(), y = Fl::event_y();
	int leftX = this->x();
	int rightX = leftX + this->w();
	int topY = this->y();
	int botY = topY + this->h();
	
	unsigned int pos = POS_ANY;

	if( x > ( rightX - 6 ) ) {
		pos = POS_E;
	} else 	if( x < ( leftX + 6 ) ) {
		pos = POS_W;
	}
	if( y > ( botY - 6 ) ) {
		pos += POS_S;
	} else if( y < ( topY + 6 ) ) {
		pos += POS_N;
	}
	
	fl_cursor( cursors[pos] ) ;

	return (MousePosition)pos;
}

void Flx_MdiChild::drag() {
	int x = Fl::event_x(), y = Fl::event_y();
	int thisX = this->x(), thisY = this->y();

	int dx = x - _x, dy = y - _y;
	int newX = thisX + dx;
	int newY = thisY + dy;

	//check if dragging is possible
	int remVis = 15; //min. remains visible
	int parentX = parent()->x(), parentY = parent()->y();
	if( newX < (parentX - w() + remVis ) ) return;
	if( newX > (parentX + parent()->w() - remVis ) ) return;
	if( newY < parentY ) return;
	if( newY > ( parentY + parent()->h() - remVis ) ) return;
	
	position( newX, newY );
	window()->redraw();
	_x = x;
	_y = y;
}

void Flx_MdiChild::resize() {
	int x = Fl::event_x(), y = Fl::event_y();
	int dx = x - _x, dy = y - _y;

	switch( _mousePos ) {
	case POS_E:
	case POS_W:
		resizeHorz( dx );
		break;
	case POS_N:
	case POS_S:
		resizeVert( dy );
		break;
	default:
		resizeHorz( dx );
		resizeVert( dy );
		break;
	}
	
	window()->redraw();

	_x = Fl::event_x();
	_y = Fl::event_y();
}

void Flx_MdiChild::resizeHorz( int dx ) {
	int thisX = this->x();
	if( canResizeHorz( dx ) ) {
		if( ( _mousePos & POS_E )  == POS_E ) {
			Fl_Group::resize( thisX, this->y(), this->w() + dx, this->h() );
		} else {
			Fl_Group::resize( this->x() + dx, this->y(), this->w() - dx, this->h() );
		}
	}
}

void Flx_MdiChild::resizeVert( int dy ) {
	int thisY = this->y();
	if( canResizeVert( dy ) ) {
		if( ( _mousePos & POS_N ) == POS_N ) {	
			Fl_Group::resize( this->x(), thisY + dy, this->w(), this->h() - dy );
		} else {
			Fl_Group::resize( this->x(), thisY, this->w(), this->h() + dy );
		}
	}
}

bool Flx_MdiChild::canResizeVert( int dy ) const {
	if( dy > 0 ) {
		//resizing downwards
		if( ( _mousePos & POS_N ) == POS_N ) { //grip on title bar; child getting smaller
			if( this->y() > parent()->y()/*keep title bar visible*/ &&
				this->h() > ( _pTitleBar->h() + 25 /*approx. scrollbar height*/ ) ) 
			{
				return true;
			}
		} else {
			//grip on lower border; child getting higher
			return true;
		}
	} else {  //dy < 0; resizing upwards
		if( ( _mousePos & POS_N ) == POS_N ) { //grip on title bar; child getting higher
			if( this->y() > parent()->y() ) {
				return true;
			}
		} else {
			//grip on lower border; child getting smaller
			if( this->h() > ( _pTitleBar->h() + 25 /*approx. scrollbar height*/ ) ) {
				return true;
			}
		}
	}
	return false;
}

bool Flx_MdiChild::canResizeHorz( int dx ) const {
	if( dx > 0 ) { //resizing to the right
		if( ( _mousePos & POS_W ) == POS_W ) { //grip on left border; child getting smaller
			if( this->w() > 25 /* scrollbar width */ ) {
				return true;
			}
		} else {
			//grip on right border; child growing
			return true;
		}
	} else { //resizing to the left
		if( ( _mousePos & POS_W ) == POS_W ) { //grip on left border; child getting smaller		
			return true;		
		} else {
			//grip on right border; child growing
			if( this->w() > 25 /* scrollbar width */ ) {
				return true;
			}
		}
	}
	return false;
}

void Flx_MdiChild::draw() {
    Fl_Group::draw();
//    int H = parent()->h() - y();
//    fl_push_clip( x(), y(), w(), H );
//    Fl_Group::draw();
//    fl_pop_clip();
}

Flx_MdiChild::~Flx_MdiChild() {
}

} //ns flx