<?php
/*
Author:			Edward Plainview
Author Email:	edward@plainviewplugins.com
Author URI:		https://plainviewplugins.com
Description:	Shows how to create a new hook for use with the Activity Monitor.
Plugin Name:	Activity Monitor Example 1
Plugin URI:		https://plainviewplugins.com/
Version:		1
*/

namespace PVAM_Example_1;

class Activity_Monitor_Example_1
{
	/**
		@brief		The constructor.
		@since		2015-12-29 15:03:17
	**/
	public function __construct()
	{
		// By using this action, we guarantee that PVAM exists in order for our hook to extend the PVAM hook class.
		add_action( 'plainview_activity_monitor_manifest_hooks', [ $this, 'plainview_activity_monitor_manifest_hooks' ] );
	}

	/**
		@brief		Manifest the hooks we provide: tell PVAM which hooks are available, so that they can be loaded and/or stored.
		@since		2015-12-29 15:04:35
	**/
	public function plainview_activity_monitor_manifest_hooks( $action )
	{
		// If we require once earlier, there is the risk that the PVAM hook class cannot be found.
		require_once( 'publish_to_publish.php' );
		$hook = new publish_to_publish();
		$hook->register_with( $action->hooks );
	}
}

new Activity_Monitor_Example_1();
