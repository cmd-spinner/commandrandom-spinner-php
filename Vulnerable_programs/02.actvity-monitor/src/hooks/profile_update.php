<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Profile update.
	@since		2014-05-04 21:18:41
**/
class profile_update
	extends hook
{
	use categories\Users;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A user profile was updated.' );
	}

	public function get_parameter_count()
	{
		return 2;
	}

	public function log()
	{
		$new_userdata = get_userdata( $this->parameters->get( 1 ) )->data;
		$old_userdata = $this->parameters->get( 2 );

		if ( $new_userdata->display_name != $old_userdata->display_name )
		{
			// Activity text: User profile updated
			$s = $this->activity_monitor()->_( '<em>Display name</em> changed from %s to %s.',
				$this->em( $old_userdata->display_name ),
				$this->em( $new_userdata->display_name )
			);
			$this->html()->append( $s );
		}

		if ( $new_userdata->user_email != $old_userdata->user_email )
		{
			// Activity text: User profile updated
			$s = $this->activity_monitor()->_( '<em>E-mail</em> changed from %s to %s.',
				$this->em(  $old_userdata->user_email ),
				$this->em( $new_userdata->user_email )
			);
			$this->html()->append( $s );
		}

		if ( $new_userdata->user_pass != $old_userdata->user_pass )
		{
			// Activity text: User profile updated
			$s = $this->activity_monitor()->_( '<em>Password</em> changed.' );
			$this->html()->append( $s );
		}

		if ( $this->html()->count() < 1 )
			return;

		$this->html_and_execute();
	}
}
