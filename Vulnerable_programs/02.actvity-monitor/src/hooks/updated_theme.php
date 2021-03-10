<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A theme was updated.
	@since		2016-01-03 14:01:14
**/
class updated_theme
	extends upgrader_post_install
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A theme was updated.' );
	}

	public function log()
	{
		$args = $this->parameters->get( 2 );

		// We only want themes.
		if ( $args[ 'type' ] != 'theme' )
			return;

		// Load theme info.
		$theme = $args[ 'theme' ];
		$info = themes_api( 'theme_information', [ 'slug' => $theme ] );

		// Theme NAME updated to VERSION
		$s = $this->activity_monitor()->_( 'Theme %s updated to %s', $this->em( $info->name ), $this->em( $info->version ) );

		$this->html_and_execute( $s );

		// Since we are a filter, return the value we got.
		return $this->parameters->get( 1 );
	}
}
