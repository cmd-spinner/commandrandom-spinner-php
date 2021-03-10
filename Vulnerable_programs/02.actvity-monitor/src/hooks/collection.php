<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Container of activities.
	@since		2014-04-27 15:36:08
**/
class collection
	extends \plainview\sdk_pvam\collections\collection
{
	/**
		@brief		Return the instance of the activity monitor.
		@since		2014-05-06 22:42:55
	**/
	public function activity_monitor()
	{
		return \plainview\wordpress\activity_monitor\Plainview_Activity_Monitor::instance();
	}

	/**
		@brief		Builds the classmap for autoloading.
		@since		2014-05-01 08:42:25
	**/
	public function get_classmap()
	{
		$r = [];
		foreach( $this->items as $hook )
		{
			$classmap = $hook->get_classmap();
			$r[ $classmap ] = $classmap;
		}
		return $r;
	}

	/**
		@brief		Retrieve a hook using the class name.
		@since		2014-05-07 00:36:55
	**/
	public function get_hook( $class_name )
	{
		foreach( $this->items as $item )
			if ( get_class( $item ) == $class_name )
				return $item;
		return false;
	}

	/**
		@brief		Does this hook name occur in the collection?
		@since		2014-10-04 09:04:10
	**/
	public function has_hook( $hook )
	{
		return $this->has( $hook->get_id() );
	}

	/**
		@brief		Register a hook, adding all the necessary info to the indexes.
		@since		2014-04-27 20:56:42
	**/
	public function register( $hook )
	{
		$id = $hook->get_id();
		$this->set( $id, $hook );
	}

	/**
		@brief		Save our hooks.
		@since		2014-05-09 12:39:23
	**/
	public function save()
	{
		$this->activity_monitor->set_logged_hooks( $this->get_classmap() );
	}
}
