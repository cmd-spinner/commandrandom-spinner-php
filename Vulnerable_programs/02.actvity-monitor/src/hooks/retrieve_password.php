<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Request a new password.
	@since		2014-05-04 12:38:03
**/
class retrieve_password
	extends hook
{
	use categories\Users;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'The user requested a password reset link.' );
	}

	public function log()
	{
		$username = $this->parameters->get( 1 );
		$user = get_user_by( 'login', $username );
		$this->log_hook->user_id = $user->ID;
		// Activity text: The user requested a password reset link.
		$s = $this->activity_monitor()->_( 'Password reset requested.' );
		$this->html_and_execute( $s );
	}
}
