<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A package was upgraded.
	@since		2016-01-03 14:01:14
**/
class upgrader_post_install
	extends hook
{
	use categories\Updates;

	public function get_parameter_count()
	{
		// apply_filters( 'upgrader_post_install', true, $args['hook_extra'], $this->result );
		return 3;
	}

	public function hook()
	{
		if ( $this->is_disabled() )
			return;

		add_filter( 'upgrader_post_install', [ $this, 'do_log' ], $this->get_priority(), $this->get_parameter_count() );
	}
}
