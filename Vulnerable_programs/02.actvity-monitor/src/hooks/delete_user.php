<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		User deletion.
	@details	Thanks to websolad for expanding this hook.
	@since		2014-05-04 20:43:14
**/
class delete_user
	extends user_register
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A user was deleted.' );
	}

  	public function log()
	{
		// We need special handling for wp_login since the user is not noted in the get_current_user_id function yet.
		$user = get_userdata( $this->parameters->get( 1 ) );
		$this->log_hook->user_id = get_current_user_id();

		// Activity text: A user was deleted. This is his login name.
		$s = $this->activity_monitor()->_( 'User deleted: %s',
			$this->em( $user->data->user_login )
		);
		$this->html()->append( $s );

		// Activity text: A user was deleted. This is his name.
		$s = $this->activity_monitor()->_( 'Name: %s',
			$this->em( $user->first_name . ' ' . $user->last_name )
		);
		$this->html()->append( $s );

		// Activity text: A user was deleted. This is his e-mail address.
		$s = $this->activity_monitor()->_( 'E-mail: %s',
			$this->em( $user->data->user_email )
		);
		$this->html()->append( $s );

		$this->html_and_execute();
	}
}
