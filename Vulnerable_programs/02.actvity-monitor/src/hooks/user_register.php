<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		User creation.
	@since		2014-05-04 20:43:14
**/
class user_register
	extends hook
{
	use categories\Users;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A new user was registered.' );
	}

	public function log()
	{
		$user = get_userdata( $this->parameters->get( 1 ) );
		$this->log_hook->user_id = $user->ID;

		// Activity text: A new user was registered
		$s = $this->activity_monitor()->_( 'User registered: %s', $this->em( $user->data->user_login ) );
		$this->html()->append( $s );

		// Activity text: A new user was registered
		$s = $this->activity_monitor()->_( 'E-mail: %s', $this->em( $user->data->user_email ) );
		$this->html()->append( $s );

		$this->html_and_execute();
	}
}
