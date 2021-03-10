<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A plugin was updated.
	@since		2016-01-03 14:01:14
**/
class updated_plugin
	extends upgrader_post_install
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A plugin was updated.' );
	}

	public function log()
	{
		$args = $this->parameters->get( 2 );

		// We only want plugins.
		if ( $args[ 'type' ] != 'plugin' )
			return;

		// Installing means we don't know anything about the plugin.
		if ( $args[ 'action' ] == 'install' )
			return;

		// Load the plugin info.
		$info = get_plugin_data( $args[ 'plugin' ] );

		// Plugin NAME updated to VERSION
		$s = $this->activity_monitor()->_( 'Plugin %s updated to %s', $this->em( $info[ 'Name' ] ), $this->em( $info[ 'Version' ] ) );

		$this->html_and_execute( $s );

		// Since we are a filter, return the value we got.
		return $this->parameters->get( 1 );
	}
}
