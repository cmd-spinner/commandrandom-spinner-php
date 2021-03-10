<?php

namespace plainview\wordpress\activity_monitor\actions;

/**
	@brief		Return the activity's description as HTML text.
	@since		2014-05-03 23:41:37
**/
class get_activity_description
	extends action
{
	/**
		@brief		IN: The activity whose description we are to retrieve.
		@since		2014-05-03 23:42:01
	**/
	public $activity;

	/**
		@brief		OUT: Description described in HTML.
		@since		2014-05-03 23:42:01
	**/
	public $description;
}
