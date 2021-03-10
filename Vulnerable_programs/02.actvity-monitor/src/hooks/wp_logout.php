<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		User logout.
	@since		2014-05-01 22:01:00
**/
class wp_logout
	extends hook
{
	use categories\Users;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A user logged out.' );
	}

	public function log()
	{
		// Activity text: A user logged out.
		$s = $this->activity_monitor()->_( 'User %s logged out.', $this->log_hook->user_id_html() );
		$this->html_and_execute( $s );
	}

}
