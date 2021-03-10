<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Theme switched.
	@since		2015-10-21 23:32:44
**/
class switch_theme
	extends hook
{
	use categories\Theme;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'The theme was switched.' );
	}

	public function get_parameter_count()
	{
		// do_action( 'switch_theme', $new_name, $new_theme );
		return 2;
	}

	public function log()
	{
		$name = $this->parameters->get( 1 );

		// Activity text: The the was switched to NEW
		$s = $this->activity_monitor()->_( 'Theme switched to %s', $this->em( $name ) );
		$this->html()->append( $s );

		$this->html_and_execute();
	}
}