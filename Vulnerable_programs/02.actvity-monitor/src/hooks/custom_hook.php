<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		This is for hooks that don't normally exist.
	@details

	- wp_login, profile_update, created_term are existing actions and therefore supposed to use the hook class.
	- ip_too_often, bruteforce_detect, send_to_htaccess, being hooks that we created ourselves, should use custom_hook.

	@see		hook
	@since		2015-12-06 20:53:44
**/
class custom_hook
	extends hook
{

	/**
		@brief		Do not hook into the hook we are monitoring, else we get into a loop.
		@since		2015-12-06 20:53:21
	**/
	public function hook()
	{
	}

}
