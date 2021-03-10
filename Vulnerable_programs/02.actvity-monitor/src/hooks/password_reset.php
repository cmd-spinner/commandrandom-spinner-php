<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Reset the password.
	@since		2014-05-04 12:38:03
**/
class password_reset
	extends hook
{
	use categories\Users;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A user password was reset.' );
	}

	public function log()
	{
		$this->log_hook->user_id = $this->parameters->get( 1 )->ID;

		// Activity text: A user's password was reset.
		$s = $this->activity_monitor()->_( 'Password reset.' );

		$this->html_and_execute( $s );
	}
}
