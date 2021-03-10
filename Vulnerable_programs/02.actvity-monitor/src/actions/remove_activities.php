<?php

namespace plainview\wordpress\activity_monitor\actions;

use \plainview\sdk_pvam\collections\collection;

/**
	@brief		Remove activities with the specified IDs from the database.
	@since		2014-05-11 09:57:31
**/
class remove_activities
	extends action
{
	/**
		@brief		IN: Activity IDs to remove from the database.
		@since		2014-05-04 01:07:01
	**/
	public $ids;

	public function __construct()
	{
		$this->ids = new collection;
	}
}
