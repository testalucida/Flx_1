#ifndef _Flx_Table_H_
#define _Flx_Table_H_

#include <flx/flx_signalparms.h>
#include <flx/Flx_ContextMenu.h>

#include <my/enums.h>
#include <my/TableData.h>
#include <my/Signal.hpp>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Pixmap.H>

#include <list>
#include <vector>

class Fl_Input;
class Fl_Menu_;

namespace flx {

    class Flx_TableSearch;
    struct MenuItemAction;

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

    //==============================================================

    class Flx_ColumnHeaderContextMenu : public Flx_ContextMenu {
    public:
        Flx_ColumnHeaderContextMenu();
    };

    //===========================================================

    class Flx_CellContextMenu : public Flx_ContextMenu {
    public:
        Flx_CellContextMenu();
        void setEntriesCountItem(int rows);
    private:
        int _entryItemIdx;
    };

    //===========================================================

    //#define BYTE unsigned char

    struct Area {
        int toprow, leftcol, botrow, rightcol;

        void reset() {
            toprow = leftcol = botrow = rightcol = -1;
        }

        void dump() const {
            fprintf(stderr, "topleft: %d/%d, botright: %d/%d\n",
                    toprow, leftcol, botrow, rightcol);
        }
    };

    struct Cell {

        Cell() : row(-1), col(-1) {
        }
        int row, col;
    };

    typedef std::shared_ptr<Cell> CellPtr;

    class Selection {
    public:

        /** Ctor */
        Selection(int rows, int cols);
        /** Default Ctor */
        Selection();
        /** Dtor */
        ~Selection();

        /** defines the size of this cells object.*/
        void setSize(int rows, int cols);

        /** mark cell row/col as "on" */
        void on(int row, int col);

        /** mark cell row/col as "off" */
        void off(int row, int col);

        /** toggle mark for cell row/col */
        void toggle(int row, int col);

        /** mark all cells of a given row as "on" */
        void rowOn(int row);

        /** mark all cells of a given row as "off" */
        void rowOff(int row);

        /** mark all cells of a given column as "on" */
        void colOn(int col);

        /** mark all cells of a given column as "off" */
        void colOff(int col);

        /** mark all cells of a given area as "on" */
        void areaOn(int toprow, int leftcol, int botrow, int rightcol);

        /** mark all cells of a given area as "on".
         * row/col is meant to be a corner of the area.  */
        void areaOff(int fromrow, int fromcol, int torow, int tocol);

        inline bool isOn(int row, int col) const;

        bool isRowOn(int row) const;

        bool isColOn(int col) const;

        /** get the topmost row being completely selected */
        int getMinRowOn();

        /** get a rectangle of "on" bits including the cell specified by row/col */
        //const Area &getAreaOn( int row, int col );

        /** mark all cells as "on" */
        void allOn();

        /** mark all cells as "off" */
        void allOff();

        CellPtr getFirstSelectedCell() const;

        /** get all selected cells. */
        const std::vector<CellPtr> &getSelectedCells();
        
        void dumpSelection() const;

        /** get all selected rows.
         * A row is taken for selected when
         * each column's cell in this row is selected. */
        const vector<int> &getSelectedRows();

        /** get all selected columns.
         * A column is taken for selected when
         * each rows' cell in this row is selected. */
        const vector<int> &getSelectedColumns();
    private:
        //void deleteAllocation( bool **ppArea, int rows, int cols );
        inline long getSelIndex(int row, int col) const;
        inline void getRowStartEndIndexes(int row, long &start, long &end) const;
        long getMaxIndex() const;
        void rowOnOff(int row, uchar onoff);
        void colOnOff(int col, uchar onoff);
    private:
        //bool** _ppCells;
        int _rows, _cols;
        uchar *_pSel;
        //Area _area;
        std::vector<CellPtr> _selectedCells;
        std::vector<int> _selectedRows;
        std::vector<int> _selectedCols;

    };

    //----------------------------------------------

    struct SelectionEvent {

        SelectionEvent() : doubleClicked(false) {
        }
        bool doubleClicked;
    };


    //----------------------------------------------

    class Flx_Table : public Fl_Table {
    public:
        /** a cell has been selected */
        my::Signal<Flx_Table, SelectionEvent> signalSelected;
        /** an item of the cell context menu has been selected*/
        my::Signal < Flx_ContextMenu, MenuItemAction > signalContextMenuItemSelected;
    private:

        struct {
            int fl_event;
            TableContext tableContext;
            int row; //concerned row
            int lastRow; //last concerned row
            int col; //concerned column;
            int lastCol; //last concerned column
            int key;
            bool isDblClick;
            bool isCtrl;
            bool isShift;
            bool isDragging;
            bool isButton1;
            bool isButton3;
            bool isArrowUp;
            bool isArrowDown;
            bool isArrowLeft;
            bool isArrowRight;

            void reset() {
                tableContext = CONTEXT_NONE;
                fl_event = row = col = lastRow = lastCol = key = -1;
                isDblClick = isCtrl = isShift = isDragging = isButton1 = isButton3 =
                        isArrowUp = isArrowDown = isArrowLeft = isArrowRight = false;
            }

            bool isSelectionConcerned() {
                if (fl_event != FL_PUSH && fl_event != FL_KEYDOWN && fl_event != FL_DRAG) {
                    return false;
                }
                if (tableContext != CONTEXT_CELL && tableContext != CONTEXT_ROW_HEADER &&
                        !(tableContext == CONTEXT_TABLE && (key & FL_Tab) == FL_Tab)) {
                    return false;
                }
                if (!(isMouseClick() || isArrowKey())) {
                    return false;
                }
                return true;
            }

            bool isArrowKey() {
                return ( isArrowUp || isArrowDown || isArrowLeft || isArrowRight);
            }

            bool isMouseClick() {
                return ( isButton1 || isButton3);
            }

            void dump() {
                fprintf(stderr, "eventContext:\n fl_event=%d, tableContext=%d, row=%d, col=%d, key=%d\n",
                        fl_event, tableContext, row, col, key);
                fprintf(stderr, "isCtrl=%c, isShift=%c, isButton1=%c, isButton3=%c\n",
                        isCtrl ? 'J' : 'N',
                        isShift ? 'J' : 'N',
                        isButton1 ? 'J' : 'N',
                        isButton3 ? 'J' : 'N');
                fprintf(stderr, "isArUp=%c, isArDown=%c, isArLeft=%c, isArRight=%c\n",
                        isArrowUp ? 'J' : 'N',
                        isArrowDown ? 'J' : 'N',
                        isArrowLeft ? 'J' : 'N',
                        isArrowRight ? 'J' : 'N');
            }
        } _eventContext;
    public:
        /**  Ctor  */
        Flx_Table(int x, int y, int w, int h, const char *L = 0);

        /** Dtor */
        virtual ~Flx_Table();

        void setTableData(my::TableData &data);

        my::TableData &getTableData() const;

        void enableSorting(bool isSortable);

        /** Callback whenever someone clicks on different parts of the table */
        static void static_event_callback(Fl_Widget*, void *userdata) {
            Flx_Table *pThis = (Flx_Table*) userdata;
            pThis->event_callback();
        }

        void event_callback();

        /** selects a single cell */
        void select(int row, int col);

        void selectRow(int row);

        void selectColumn(int col);

		void clearSelection();

        /** marks a cell. This means painting a blue rectangle 
            around the given cell */
        void mark(int row, int col);

        /** releases any marks */
        void unmark();

        /** where to align the given column's content.
            One of FL_ALIGN_CENTER, FL_ALIGN_LEFT, FL_ALIGN_RIGHT */
        void alignContent(int colIdx, Fl_Align alignment);

        /** if cell row/col can be selected */
        virtual bool canSelectCell(int row, int col);

        /** returns true if the given cell is selected */
        inline bool isSelected(int row, int col) const;

        bool isSortable() const;

        int getSelectionMode();

        void setSelectionMode(Flx_SelectionMode mode);

        /** gets the topmost leftmost selected cell.
            This is useful if selectionmode is FLX_SELECTION_SINGLECELL
            or FLX_SELECTION_SINGLEROW */
        CellPtr getFirstSelectedCell() const;

        /** get all selected cells. */
        const std::vector<CellPtr> &getSelectedCells();
        
        void dumpSelection() const;

        /** get all selected rows.
         * A row is taken for selected when
         * each column's cell in this row is selected. */
        const vector<int> &getSelectedRows();

        /** get all selected columns.
         * A column is taken for selected when
         * each rows' cell in this row is selected. */
        const vector<int> &getSelectedColumns();

        /** callback for TableData changes */
        //static void staticTableDataCallback( my::TableData &src, my::TableDataChange chng, int row, int col, void *userData );
        //void tableDataCallback( my::TableData &src, my::TableDataChange chng, int row, int col );
        void tableDataCallback(my::TableData &src, my::DataChangedEvent &chng);

        /** called by tableDataCallback if all values have changed.
            Clears the entire table and sets rows and cols according
            to tableData model. Arranges columns. 
            You may override this method but you should call it before
            or after your own code. */
        virtual void onTableDataChanged();

        /** sets table sortable */
        void setSortable(bool sortable);

        //Flx_CellContextMenu &getCellContextMenu();
        //Flx_ColumnHeaderContextMenu &getColumnHeaderContextMenu();
        /** returns column idx of column specified by header */
        int getColumnIdByHeader(const char *pHeader);
        
        /** gets the cell context menu */
        Flx_ContextMenu &getCellContextMenu() { return _cellMenu; }
    
        void onShortcut( Fl_Menu_ &, MenuItemAction & );
    
    protected:

        /** checks if a given cell is editable */
        virtual bool isCellEditable(int row, int col) const {
            return true;
        }
        /** gets the appropriate editor widget. Per default, a basic Fl_Input
            is returned */
        virtual Fl_Input &getEditor();
        /** override Fl_Table's resize(..) */
        void resize(int x, int y, int w, int h);

        /** Handle drawing table's cells
            Fl_Table calls this function to draw each visible cell in the table.
            It's up to us to use FLTK's drawing functions to draw the cells the way we want. */
        void draw_cell(TableContext context, int row = 0, int col = 0, int x = 0, int y = 0, int w = 0, int h = 0);

        /** gets the background color for the given cell */
        virtual Fl_Color getCellBackground(int row, int col, bool isSelected);

        int handle(int evt);

        /** opens the search dialog to allow the user to enter a search item */
        void searchTable();


    private:
        /** checks if the last pushed key may trigger edit mode.
            ('A' will do so, '->' not.) */
        bool isTriggerEditKey();
        /** checks if editing is possible; if so, starts editing */
        void checkEdit();
        /** start editing a cell */
        void startEdit(int row, int col);
        /** end editing a cell */
        void endEdit();

        /** returns true if TAB was the last hit button */
        bool hitTab() {
            return (Fl::event_key() & FL_Tab) == FL_Tab;
        }
        /** Draw the col headings */
        void drawColumnHeader(int col, const char *s, int x, int y, int w, int h);
        /** if sorted draw a small triangle according to the sort direction */
        void drawSortedFlag(int x, int y, int w, int h, my::SortDirection sortdir) const;
        /** Draw the row headings */
        void drawRowHeader(int row, const char *s, int x, int y, int w, int h);

        /** copies the content of the selected cells to clipboard */
        void copyToClipboard();
        /** pastes the clipboard's content into the table */
        void pasteFromClipboard();
        void paste(const char *pClipContent);

        /** adjusts selection depending on _selectionModel*/
        bool adjustSelection();

        /** call the TableListeners */
        void callTableListeners(Flx_TableEvent evt, int r1, int c1, int r2, int c2);

        /** checks if sth is to be done related to
         *  the table's selections.
         *  All checks based upon _eventContext.
         *  Called out of event_callback.
         *  Returns true if the selection is concerned. */
        bool checkSelection();

        /** deals with column selection */
        void checkColumnSelection();

        /** sets anchor row and column depending
         * on _eventContext */
        void checkAnchor();

        /** Switches selection on if the given row or
         * parts of it are not selected.
         * Switches selection off if
         * the given row is selected.    */
        void toggleRowSelection(int row);

        void selectMultiRow();

        /** selects one or more columns depending
         * on _eventContext  */
        void selectMultiColumn();

        /** shows columnheader context menu at the
         * given position and takes appropriate action */
        void showColumnHeaderContextMenu(int x, int y);

        /** shows cell context menu at the given position */
        void showCellContextMenu(int x, int y);

        inline Flx_TableDrawMode checkDraw(int row, int col);

        inline void setEventContext();

        const char *getNextValue(const char *pSrc, char *pVal, bool init);

        void doSelectionCallback();
        void adjustColWidth();
        /** creates a submenu containing all columns of the table */
        void createAllColumnsSubmenu();
        /** sorts columns in submenu */
        static bool alphaSort(const char *pVal1, const char *pVal2);
        /** copies all values of the specified column to clipboard */
        void copyToClipboard(int col, bool withSeparator);
        Fl_Scrollbar *getHorzScrollbar();
        /** static callback used by the cell editor */
        static void staticInputCallback(Fl_Widget *, void *);
        /** callback used by the cell editor */
        void inputCallback();
        void makeColumnsFit();
    protected:
        my::TableData *_pData;
	private:
        Flx_SelectionMode _selectionMode;
        bool _popupIsActive;
        static const int ENTRYLABELSIZE = 50;
        char _entriesLabel[ENTRYLABELSIZE];
        vector<Fl_Align> _vAlign;
        bool _isSortable;
        my::SortDirection _sortdirection;
        Selection _selected;
        Flx_ColumnHeaderContextMenu _colhdrMenu;
        Flx_CellContextMenu _cellMenu;
        int _anchorRow; // for area selections
        int _anchorCol; // for area selections

        std::vector<CellPtr> _switchedCells;
        std::vector<int> _switchedRows;
        std::vector<int> _switchedCols;

        bool _doCallback;
        int _gotoCol;
        Fl_Color _gotoColColor;
        //for editing purposes
        Fl_Input *_pIn;
        int _rowEdit, _colEdit;
        Flx_TableSearch *_pSearch;
        Cell _markedCell;
    };

}

#endif
