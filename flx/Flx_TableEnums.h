/* 
 * File:   Flx_TableEnums.h
 * Author: max
 *
 * Created on 19. Oktober 2014, 11:17
 */

#ifndef FLX_TABLEENUMS_H
#define	FLX_TABLEENUMS_H

namespace flx {
   
   enum Flx_TableEvent {
        FLX_SEL_CHNG,
        FLX_DBL_CLICK,
        FLX_TABLEEVENT_SORT
    };

    enum Flx_SelectionMode {
        FLX_SELECTION_NONE,
        FLX_SELECTION_SINGLECELL,
        FLX_SELECTION_SINGLEROW,
        FLX_SELECTION_MULTIROW,
        FLX_SELECTION_FREE
    };

    enum Flx_TableDrawMode {
        FLX_DRAW_NONE,
        FLX_DRAW_SELECTED,
        FLX_DRAW_UNSELECTED
    };
}

#endif	/* FLX_TABLEENUMS_H */

