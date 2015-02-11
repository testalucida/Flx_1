
#include <flx/Flx_Calendar.h>
#include <stdio.h>

#include <my/datetime.h>
#include <my/compat.h>
#include <my/exceptions.h>

using namespace my;
using namespace std;
using namespace std::placeholders;

namespace flx {

    DayTableModel::DayTableModel( ) {
        _selIdx = -1;
        _onCellsChangedCallback = NULL;
        allocDays( );
        //we start by showing current date:
        _today.SetCurrentDate( );
        month( _today.GetMonth( ), _today.GetYear( ), _today.GetDay( ) );
    }

    /** returns given month */
    int DayTableModel::month( ) {
        return _showDate.GetMonth( );
    }

    /** returns given year*/
    int DayTableModel::year( ) {
        return _showDate.GetYear( );
    }

    /** returns selected day */
    int DayTableModel::day( ) {
        int d;
        sscanf( _days[_selIdx], "%d", &d );
        return d;
    }

    /** returns today's month */
    int DayTableModel::monthToday( ) {
        return _today.GetMonth( );
    }

    /** returns today's year */
    int DayTableModel::yearToday( ) {
        return _today.GetMonth( );
    }

    /** called by DayTable whenever a cell is selected */
    void DayTableModel::day( int row, int col ) {
        _selIdx = row * 7 + col;
    }

    /** returns first weekday of the given month/year.
            monday will have index 0 */
    int DayTableModel::firstWeekday( ) {
        return _firstWeekday;
    }

    /** selects the month to be shown 
                if day is omitted and month == current month and 
                year == current year then current date will be preselected */
    void DayTableModel::month( int month, int year, int day ) {
        _nDays = MyDate::GetNumberOfDays( month, year );
        _showDate.SetDay( day );
        _showDate.SetMonth( month );
        _showDate.SetYear( year );

        if ( day == 1 ) {
            _firstWeekday = _showDate.GetWeekday( ); //0->sunday; 6->saturday
        } else {
            MyDate firstofmonth( 1, month, year );
            _firstWeekday = firstofmonth.GetWeekday( );
        }
        //we want monday to have index 0:
        _firstWeekday--;
        _firstWeekday = (_firstWeekday < 0) ? 6 : _firstWeekday;

        //number of rows to display
        if ( _firstWeekday > 5 || (_firstWeekday > 4 && _nDays == 31) ) {
            _rowCount = 6;
        } else {
            _rowCount = 5;
        }

        if ( _showDate.GetMonth( ) == _today.GetMonth( ) &&
                _showDate.GetYear( ) == _today.GetYear( ) ) {
            //the day table of the actual month is to be shown
            //calculate position of current date:
            int idx = _firstWeekday + _today.GetDay( ) - 1;
            _colCurDate = idx % 7;
            _rowCurDate = (idx - _colCurDate) / 7;
            //_selIdx = _rowCurDate * 7 + _colCurDate;
        } else {
            //show another month than the actual one
            _colCurDate = _rowCurDate = -1;
            //_selIdx = -1;
        }

        _selIdx = _firstWeekday + _showDate.GetDay( ) - 1;
        //fprintf( stderr, "month(..): _selIdx=%d\n", _selIdx );

        clearDays( );

        for ( int d = 0; d < _nDays; d++ ) {
            int idx = d + _firstWeekday;
            Compat::intToCString( _days[idx], 3, "%2d", d + 1 );
            //fprintf(stderr, "_days[%d]=%s\n", idx, _days[idx]);
        }

        /** tell the listeners that the day table has been changed */
        if ( _onCellsChangedCallback ) {
            _onCellsChangedCallback( );
        }
    }

    void DayTableModel::setOnCellsChangedCallback( ON_CELLS_CHANGED_CALLBACK cb ) {
        _onCellsChangedCallback = cb;
    }

    /** provides today's coordinates.
        If you call this without the actual month be shown, -1/-1 will be returned. */
    void DayTableModel::getTodayCoord( int &row, int &column ) {
        row = _rowCurDate;
        column = _colCurDate;
    }

    /** provides the selected day's coordinates.
        There's always a day selected.*/
    void DayTableModel::getSelectedDayCoord( int &row, int &column ) {
        column = _selIdx % 7;
        row = (_selIdx - column) / 7;
        //fprintf( stderr, "selectedDayCoord returns: row=%d, col=%d\t_selIdx=%d\n", row, column, _selIdx );
    }

    int DayTableModel::getColumnCount( ) const {
        return 7;
    }

    int DayTableModel::getRowCount( ) const {
        return _rowCount;
    }

    const char* DayTableModel::getValue( int row, int col ) const {
        int idx = row * 7 + col;
        //fprintf( stderr, "row=%d, col=%d, day=%s\n", row, col, _days[idx] );
        return _days[idx];
    }

    const char* DayTableModel::getColumnHeader( int col ) const {
        switch ( col ) {
            case 0:
                return "Mo";
            case 1:
                return "Di";
            case 2:
                return "Mi";
            case 3:
                return "Do";
            case 4:
                return "Fr";
            case 5:
                return "Sa";
            case 6:
                return "So";
            default:
                return "";
        }
    }

    const char* DayTableModel::getRowHeader( int row ) const {
        return "";
    }

    void DayTableModel::allocDays( ) {
        for ( int i = 0; i < MAX_CELLS; i++ ) {
            _days[i] = new char[3];
        }
    }

    void DayTableModel::clearDays( ) {
        for ( int i = 0; i < MAX_CELLS; i++ ) {
            Compat::strcpy( _days[i], 3, "  " );
        }
    }

    void DayTableModel::deleteDays( ) {
        for ( int i = 0; i < MAX_CELLS; i++ ) {
            fprintf( stderr, "delete _days[%d]\n", i );
            delete[] _days[i];
        }
    }

    DayTableModel::~DayTableModel( ) {
        deleteDays( );
    }


    //==============================================================

	static const int ROWHEIGHT = 22;
	static const int COLWIDTH = 22;

	//===============================================================

    DayTable::DayTable( int x, int y, int w, int h ) : Flx_Table( x, y, w, h ) {
        _pDtm = NULL;
        _r = _c = -1;
        setSelectionMode( FLX_SELECTION_SINGLECELL );
		//setSelectionCallback( bind( &DayTable::onSelectionChanged, this, _1 ) );
		signalSelected.connect<DayTable, &DayTable::onSelectionChanged>( this );
        _weekendColor = fl_rgb_color( 193, 211, 255 );
    }

    /** registers a callback function that is called when a 
        cell is doubleclicked */
    void DayTable::daySelected_callback( DAY_SELECTED cb_fnc ) {
        _daySelectedCallback = cb_fnc;
    }

    /** called when a selection is made */
    //void DayTable::onSelectionChanged( bool doubleClicked ) {
	void DayTable::onSelectionChanged( Flx_Table &tbl, SelectionEvent &evt ) {
		const vector<CellPtr> cells = getSelectedCells();
		_pDtm->day( cells[0]->row, cells[0]->col );
		if( evt.doubleClicked ) {
			if ( _daySelectedCallback ) {
				_daySelectedCallback( _pDtm->day( ) );
			}
		}
    }

    bool DayTable::canSelectCell( int row, int col ) {
        const char* val = _pDtm->getValue( row, col );
        return val[0] > ' ' || val[1] > ' ';
    }

    void DayTable::tableModel( DayTableModel &model ) {
        _pDtm = &model;
		model.setOnCellsChangedCallback( bind( &DayTable::onTableDataChanged, this ) );
        Flx_Table::setTableData( model );
        int row, col;
        model.getTodayCoord( row, col );
		select( row, col );
        _r = row;
        _c = col;
    }

    void DayTable::onTableDataChanged( ) {
        int r, c; //_pDtm->firstWeekday();
        _pDtm->getSelectedDayCoord( r, c );
        Flx_Table::onTableDataChanged( );
        select( r, c );
		row_height_all( ROWHEIGHT );
		col_width_all( COLWIDTH );
    }

    /** overrides MyTable::cellBackground(..) */
    Fl_Color DayTable::getCellBackground( int row, int col, bool isSelected ) {
        int r, c;
        _pDtm->getTodayCoord( r, c );
        if ( r == row && c == col ) {
            return isSelected ? FL_YELLOW : FL_GRAY;
        }

        return isSelected ? FL_YELLOW :
                (col == 5 || col == 6) ? _weekendColor : FL_WHITE;
    }

    /** we override handle(int) because we don't want
        cells without value to be selected */
    int DayTable::handle( int evt ) {
        if ( current_row > -1 && current_col > -1 ) {
            const char *val = _pDtm->getValue( current_row, current_col );
            if ( strcmp( val, "  " ) == 0 ) {
                select( _r, _c );
                _pDtm->day( _r, _c );
            }

            //get_selection( _r, _c, _r, _c );
            const vector<CellPtr> &selCells = getSelectedCells();
			if( selCells.size() > 0 ) {
				_r = selCells[0]->row;
				_c = selCells[0]->col;
			} else {
				fprintf( stderr, "DayTable::handle(): no cell selected. Select %d, %d\n",
					_r, _c );
				select( _r, _c );
				redraw();
			}
            
        }

        return Flx_Table::handle( evt );
    }

//================================================================

	static const int MARGIN = 2;
	static const int BAR_H = 25;
	static const int TABLE_H = 7*ROWHEIGHT;
	static const int TABLE_W = 8*COLWIDTH + 2*MARGIN;
	static const int BUTTON_H = 20;
	static const int BUTTONGROUP_H = 24;
	

    Flx_Calendar::Flx_Calendar( int x, int y ) : 
		Fl_Double_Window( x, y, 
			              TABLE_W + 2*MARGIN, 
						  BAR_H + TABLE_H + BUTTONGROUP_H + 2*MARGIN )
	
		, _isDragging(false) 
	{
        //fl_register_images();
        box(FL_DOWN_BOX);
        clear_border();

        //make a gray bar as a replacement for the title bar,
        //containing 2 comboboxes for entering year and month
        Fl_Group *pBarGrp = new Fl_Group( MARGIN, MARGIN, TABLE_W, BAR_H );
        pBarGrp->box(FL_FLAT_BOX);
        pBarGrp->begin();
        {
            _pMoCho = new Fl_Choice(45, 4, 55, BAR_H - 5);
            _pMoCho->box(FL_FLAT_BOX);
            _pMoCho->textsize(10);
            _pMoCho->callback(month_year_cb, this);
            _pMoCho->add("Jan");
            _pMoCho->add("Feb");
            _pMoCho->add("Mar");
            _pMoCho->add("Apr");
            _pMoCho->add("May");
            _pMoCho->add("Jun");
            _pMoCho->add("Jul");
            _pMoCho->add("Aug");
            _pMoCho->add("Sep");
            _pMoCho->add("Oct");
            _pMoCho->add("Nov");
            _pMoCho->add("Dec");

            _pYCho = new Fl_Choice(45 + 2 + _pMoCho->w(), 4, 55, BAR_H - 5);
            _pYCho->textsize(10);
            _pYCho->callback(month_year_cb, this);
            _pYCho->add("2009");
            _pYCho->add("2010");
            _pYCho->add("2011");
            _pYCho->add("2012");
            _pYCho->add("2013");
            _pYCho->add("2014");
            _pYCho->add("2015");
            _pYCho->add("2016");
            _pYCho->add("2017");
            _pYCho->add("2018");
            _pYCho->add("2019");
        }
        pBarGrp->color(FL_DARK3);
        pBarGrp->end();

        DayTable *pDayTbl = new DayTable( MARGIN, MARGIN + BAR_H, TABLE_W, TABLE_H );
		pDayTbl->setSelectionMode( FLX_SELECTION_SINGLECELL );
		pDayTbl->daySelected_callback( std::bind( &Flx_Calendar::onDaySelected, this, _1 ) );
        _pDayModel = new DayTableModel();
        pDayTbl->tableModel(*_pDayModel);
        pDayTbl->col_width_all( COLWIDTH );
        pDayTbl->row_header_width( COLWIDTH );
		pDayTbl->row_height_all( ROWHEIGHT );
        int m = _pDayModel->month();
        _pMoCho->value( m-1 );
        int yr = _pDayModel->year();
        _pYCho->value( yr - 2009 );
        pDayTbl->end();

        _pBtnOk = new Fl_Return_Button( MARGIN + COLWIDTH, 
			                            this->h() - 2*MARGIN - BUTTON_H, 
										70, 
										BUTTON_H, "OK" );
		_pBtnOk->labelsize( 10 );
		_pBtnOk->callback( staticOnOkCancel, this );
		_pBtnCancel = new Fl_Button( _pBtnOk->x() + _pBtnOk->w() + MARGIN,
			                         _pBtnOk->y(), _pBtnOk->w(), _pBtnOk->h(),
									 "Abbrechen" );
		_pBtnCancel->labelsize( _pBtnOk->labelsize() );
		_pBtnCancel->callback( staticOnOkCancel, this );
        end();
		resizable( this );
    }

	/** registers a member function to be called back
        when the user hits OK or Cancel  */
    void Flx_Calendar::callbackOnSelection( ONSELECTION callback) {
		_callback_on_selection = callback;
    }

	 /** returns the selected date  */
    MyDate& Flx_Calendar::date() {
        return _selectedDate;
    }

	/** sets the initially selected date */
    void Flx_Calendar::date(const MyDate &date) {
        _selectedDate = date;
		_pDayModel->month( date.GetMonth(), date.GetYear(), date.GetDay() );
        _pMoCho->value(date.GetMonth() - 1);
        _pYCho->value(date.GetYear() - 2009);
    }

	/** sets the initially selected date */
	void Flx_Calendar::date( const char *date ) {
		MyDate dt;
		MyDate::FromString( dt, date );
		this->date( dt );
	}

	/** shows this dialog at the given position */
	void Flx_Calendar::show( int x, int y ) {
		position( x, y );
		Fl_Double_Window::show();
	}


	/** called from static month_year_cb */
    void Flx_Calendar::onMonthYearChanged(Fl_Widget *w) {
        _pDayModel->month( _pMoCho->value() + 1, _pYCho->value() + 2009, 1 );
    }

	/** called from month and year combobox */
    void Flx_Calendar::month_year_cb(Fl_Widget *w, void *userdata) {
        Flx_Calendar *me = (Flx_Calendar*) userdata;
        me->onMonthYearChanged(w);
    }

	/** called when a cell is doubleclicked */
	void Flx_Calendar::onDaySelected( int day ) {
		selectionDone();
		hide();
	}

	void Flx_Calendar::staticOnOkCancel( Fl_Widget *pWi, void *pUserdata ) {
		Flx_Calendar *pThis = (Flx_Calendar*)pUserdata;
		Fl_Button *pBtn = (Fl_Button*) pWi;
		pThis->onOkCancel( ( strcmp( pBtn->label(), "OK" ) == 0 ) ? true : false );
	}

    /** called by OkCancelGroup */
    void Flx_Calendar::onOkCancel( bool ok ) {
		if ( ok ) {
			selectionDone();
        }
		hide();
    }

	/**  calls the callback when the selection is done
		either by pushing OK or by doubleclicking a 
		day in the day table */
	void Flx_Calendar::selectionDone() {
		_selectedDate.SetDay( _pDayModel->day() );
		_selectedDate.SetMonth( _pDayModel->month() );
		_selectedDate.SetYear( _pDayModel->year() );

		if ( _callback_on_selection ) {
			_callback_on_selection( _selectedDate );
        }
	}



	/** handles the dragging of this Flx_Calendar window */
    int Flx_Calendar::handle(int event) {
        switch (event) {
            case FL_PUSH:
                _xMouse = Fl::event_x(), _yMouse = Fl::event_y();
                if (_yMouse < BAR_H) {
                    _isDragging = true;
                }
                break;
            case FL_RELEASE:
                _isDragging = false;
                break;
            case FL_DRAG:
            {
                if (_isDragging) {
                    int dx = Fl::event_x() - _xMouse;
                    int dy = Fl::event_y() - _yMouse;
                    this->position(x() + dx, y() + dy);
                }
                //return 1;
            }
                break;
            default:
                break;
        }
        return Fl_Double_Window::handle(event);
    }

} //namespace flx
