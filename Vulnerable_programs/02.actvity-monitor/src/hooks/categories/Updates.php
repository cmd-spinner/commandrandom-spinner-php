<?php

namespace plainview\wordpress\activity_monitor\hooks\categories;

/**
	@brief		Updates to the core, plugins and themes.
	@since		2016-01-03 14:57:18
**/
trait Updates
{
	public function get_category()
	{
		// A hook category
		return Plainview_Activity_Monitor()->_( 'Updates' );
	}
}
