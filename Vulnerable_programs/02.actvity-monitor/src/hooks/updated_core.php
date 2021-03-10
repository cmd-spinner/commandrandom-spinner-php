<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		The core was upgraded.
	@since		2016-01-03 14:01:14
**/
class updated_core
	extends hook
{
	use categories\Updates;

	/**
		@brief		We do not hook into update_core...
		@since		2016-01-03 15:18:06
	**/
	public function get_action_name()
	{
		return '_core_updated_successfully';
	}

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'The Wordpress core was updated.' );
	}

	public function log()
	{
		$version = $this->parameters->get( 1 );
		$s = $this->activity_monitor()->_( 'Wordpress updated to %s', $version );
		$this->html_and_execute( $s );
	}
}
