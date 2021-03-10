<?php

namespace plainview\wordpress\activity_monitor\actions;

/**
	@brief		Tell all plugins to report in which hooks are available for monitoring.
	@since		2014-04-27 15:37:13
**/
class manifest_hooks
	extends action
{
	/**
		@brief		The hooks container.
		@since		2014-04-27 15:38:07
	**/
	public $hooks;

	/**
		@brief		Constructor.
		@since		2014-04-27 15:37:37
	**/
	public function _construct()
	{
		$this->hooks = new \plainview\wordpress\activity_monitor\hooks\collection;
	}

	/**
		@brief		Return a collection of hooks grouped by category.
		@since		2015-07-05 20:46:24
	**/
	public function by_category()
	{
		return $this->hooks->sort_by( function( $hook )
		{
			$r = $hook->get_category() . $hook->get_hook() . $hook->get_vendor() . $hook->get_description();
			$r = strtolower( $r );
			return $r;
		});
	}

	/**
		@brief		Override the execute in order to return the cached result.
		@details	When manifesting, a class will usually recreate the hook.

					This is bad because one always wants to refer to the primary instance of the hook, the one that actually hooked in to the action or filter.

					So we check for the cached value and, if not found, does a real manifest.

		@since		2016-02-01 14:15:29
	**/
	public function execute()
	{
		if ( ! isset( Plainview_Activity_Monitor()->__manifested_hooks ) )
			return parent::execute();

		$this->hooks = Plainview_Activity_Monitor()->__manifested_hooks;
	}

	/**
		@brief		Sort the hooks by name and vendor.
		@since		2014-05-03 09:22:04
	**/
	public function sort()
	{
		$this->hooks->sort_by( function( $hook )
		{
			return $hook->get_hook() . $hook->get_vendor() . $hook->get_description();
		});
	}
}
