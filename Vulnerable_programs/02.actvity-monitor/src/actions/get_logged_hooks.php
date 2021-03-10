<?php

namespace plainview\wordpress\activity_monitor\actions;

/**
	@brief		Return a collection of all of the hooks that the Activity Monitor logs to the database.
	@since		2014-07-06 17:41:59
**/
class get_logged_hooks
	extends action
{
	/**
		@brief		OUT: A hooks\collection of hook objects.
		@since		2014-07-06 17:41:34
	**/
	public $logged_hooks;

	public function __construct()
	{
		$this->logged_hooks = new \plainview\wordpress\activity_monitor\hooks\collection();
	}
}
