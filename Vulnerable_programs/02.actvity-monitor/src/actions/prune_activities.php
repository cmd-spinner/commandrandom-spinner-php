<?php

namespace plainview\wordpress\activity_monitor\actions;

/**
	@brief		Prune off old activities from the database.
	@since		2014-07-08 12:04:36
**/
class prune_activities
	extends action
{
	/**
		@brief		IN: How many activities to keep.
		@since		2014-07-08 12:04:52
	**/
	public $activities = PHP_INT_MAX;
}
