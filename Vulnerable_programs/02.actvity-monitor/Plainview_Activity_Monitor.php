<?php
/*
Author:			edward_plainview
Author Email:	plugins@plainview.se
Author URI:		http://plugins.plainview.se
Description:	Monitors activity on a site or blog.
Plugin Name:	Plainview Activity Monitor
Plugin URI:		https://wordpress.org/plugins/plainview-activity-monitor/
Version:		20161228
*/

require_once( 'vendor/autoload.php' );

define( 'PLAINVIEW_ACTIVITY_MONITOR_VERSION', 20161228 );

function Plainview_Activity_Monitor()
{
	return plainview\wordpress\activity_monitor\Plainview_Activity_Monitor::instance();
}

// For compatability with old SDK
if ( ! class_exists( '\\plainview\\sdk\\collections\collection' ) )
	require_once( 'src/old_sdk/collection.php' );

$Plainview_Activity_Monitor = new plainview\wordpress\activity_monitor\Plainview_Activity_Monitor();
