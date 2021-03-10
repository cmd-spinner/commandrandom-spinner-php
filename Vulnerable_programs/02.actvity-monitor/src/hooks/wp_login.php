<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		User login.
	@since		2014-04-27 16:28:01
**/
class wp_login
	extends hook
{
	use categories\Users;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A user logged in.' );
	}

	public function get_parameter_count()
	{
		// $user_login, $user
		return 2;
	}

	public function log()
	{
		// We need special handling for wp_login since the user is not noted in the get_current_user_id function yet.
		$user = $this->parameters->get( 2 );
		$this->log_hook->user_id = $user->ID;

		// Activity text: This user logged in.
		$s = $this->activity_monitor()->_( 'Login: %s', $this->log_hook->user_id_html() );
		$this->html()->append( $s );

		// Activity text: A user logged in with this user agent.
		$s = $this->activity_monitor()->_( 'User agent: %s', $this->em( $_SERVER[ 'HTTP_USER_AGENT' ] ) );
		$this->html()->append( $s );

		$this->html_and_execute();
	}
}
