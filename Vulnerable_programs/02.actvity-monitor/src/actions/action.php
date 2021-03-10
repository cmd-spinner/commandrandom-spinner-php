<?php

namespace plainview\wordpress\activity_monitor\actions;

/**
	@brief		Base class for all activity monitor actions.
	@since		2014-05-11 09:51:04
**/
class action
	extends \plainview\sdk_pvam\wordpress\actions\action
{
	/**
		@brief		Return the instance of the activity monitor.
		@since		2014-05-06 22:42:55
	**/
	public function activity_monitor()
	{
		return \plainview\wordpress\activity_monitor\Plainview_Activity_Monitor::instance();
	}

	public function get_prefix()
	{
		return 'plainview_activity_monitor_';
	}
}
