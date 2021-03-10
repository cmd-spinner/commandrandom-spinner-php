<?php

namespace plainview\wordpress\activity_monitor\actions;

/**
	@brief		Convert the data of an activity into a column of the activity table.
	@since		2015-10-19 17:46:01
**/
class display_activity_table_column
	extends action
{
	/**
		@brief		IN: The activity from which to extract the data.
		@since		2015-10-19 17:54:50
	**/
	public $activity;

	/**
		@brief		IN: The ID of the column to display.
		@since		2015-10-19 17:54:37
	**/
	public $column;

	/**
		@brief		IN / OUT: The Plainview table row object.
		@since		2015-10-19 17:46:27
	**/
	public $row;
}
