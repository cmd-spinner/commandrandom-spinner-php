<?php

namespace plainview\wordpress\activity_monitor\actions;

/**
	@brief		Return a list of columns that are shown in the activity table.
	@since		2015-10-19 17:15:47
**/
class get_activity_table_columns
	extends action
{
	/**
		@brief		OUT: A collection of column names ( id => visible name )
		@since		2015-10-19 17:17:11
	**/
	public $columns;

	/**
		@brief		Constructor.
		@since		2015-10-19 17:17:21
	**/
	public function _construct()
	{
		$this->columns = Plainview_Activity_Monitor()->collection();
	}
}
