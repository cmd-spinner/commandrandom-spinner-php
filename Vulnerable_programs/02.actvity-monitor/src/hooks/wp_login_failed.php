<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Login failure.
	@since		2014-05-04 12:38:03
**/
class wp_login_failed
	extends hook
{
	use categories\Users;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'Failed to log in due to non-existent user or bad password.' );
	}

	public function log()
	{
		if ( ! isset( $this->__password ) )
			$this->__password = esc_html( $_POST['pwd'] );
		if ( ! isset( $this->__username ) )
			$this->__username = esc_html( $_POST['log'] );

		$username = $this->__username;
		$user = get_user_by( 'login', $username );

		if ( ! $user )
			// Activity text: An unknown user failed to log in.
			$s = $this->activity_monitor()->_( 'Login failed: %s', $this->em( $username ) );
		else
		{
			$this->log_hook->user_id = $user->ID;
			// Login failed for user admin
			$s = $this->activity_monitor()->_( 'Login failed for %s',
				$this->log_hook->user_id_html()
			);
		}

		$this->html()->append( $s );

		if ( $this->activity_monitor()->get_site_option( 'wp_login_failed_password' ) )
		{
			// Activity text: A user failed to log in.
			$s = $this->activity_monitor()->_( 'Password: %s', $this->em( $this->__password ) );
			$this->html()->append( $s );
		}

		// Activity text: A user failed to log in.
		$s = $this->activity_monitor()->_( 'User agent: %s', $this->em( $_SERVER[ 'HTTP_USER_AGENT' ] ) );
		$this->html()->append( $s );

		$this->html_and_execute();
	}
}
