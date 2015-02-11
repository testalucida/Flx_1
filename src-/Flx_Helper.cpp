#include <flx/Flx_Helper.h>
#include <flx/Flx_Group.h>

#include <my/CharBuffer.h>

#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Double_Window.H>

#include <cstdio>
#include <algorithm>

using namespace my;

namespace flx {

Flx_Helper *Flx_Helper::_pThis = 0;

Flx_Helper &Flx_Helper::instance() {
	if( !_pThis ) {
		_pThis = new Flx_Helper;
		atexit( destroy );
	}
	return *_pThis;
}

Flx_Helper::Flx_Helper() {
	
}

void Flx_Helper::destroy() {
	delete _pThis;
	_pThis = NULL;
}

void Flx_Helper::dump( const Fl_Group &grp, bool recursive ) const {
	CharBuffer fl_Group_Name( typeid( Fl_Group ).name() );
	CharBuffer flx_Group_Name( typeid( Flx_Group ).name() );
	CharBuffer fl_Scroll_Name( typeid( Fl_Scroll ).name() );
	CharBuffer fl_DblWin_Name( typeid( Fl_Double_Window ).name() );

	printf( "====================================================================\n" );
	printf( "Flx_Helper::dumpChildren() of parent %s (at address %p)\n", 
			typeid( grp ).name(), &grp );
	printf( "\tlabel: %s, labelfont: %d, labelsize: %d\n", 
			grp.label(), grp.labelfont(), grp.labelsize() );
	printf( "\tx: %d, y: %d, w: %d, h: %d\n", grp.x(), grp.y(), grp.w(), grp.h() );
	printf( "====================================================================\n" );
	for( int i = 0, imax = grp.children(); i < imax; i++ ) {
		Fl_Widget *pW = grp.child( i );
		printf( "\tchild %d of type %c: %s at address %p\n", 
				i, pW->type(), typeid( *pW ).name(), pW );
		printf( "\tlabel: %s, labelfont: %d, labelsize: %d\n", 
				pW->label(), pW->labelfont(), pW->labelsize() );
		printf( "\tx: %d, y: %d, w: %d, h: %d\n", 
				pW->x(), pW->y(), pW->w(), pW->h() );
		if( i < imax-1 ) {
			printf( "----------------------------\n" );
		}

		if( recursive ) {
			Fl_Group *pG = dynamic_cast< Fl_Group* >( pW );
			if( pG && pG->children() > 0 ) {
				dump( *pG, true );
			}
		}

		//CharBuffer typeName( typeid( *pW ).name() );
		//if( recursive && ( typeName == fl_Group_Name || typeName == flx_Group_Name ||
		//				   typeName == fl_Scroll_Name || typeName == fl_DblWin_Name ) ) 
		//{
		//	dump( *(Fl_Group*)pW, true );
		//}
	}
	printf( "=========end of dump==========\n\n" );
}

}