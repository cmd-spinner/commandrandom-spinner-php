<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Activated a plugin.
	@since		2014-11-23 10:28:44
**/
class activated_plugin
	extends hook
{
	use categories\Plugins;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A plugin was activated.' );
	}

	public function log()
	{
		// Activity text: A plugin was activated
		$s = $this->activity_monitor()->_( 'Activated plugin %s', $this->parameters->get( 1 ) );
		$this->html_and_execute( $s );
	}
}
