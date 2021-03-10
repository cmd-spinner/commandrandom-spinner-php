<?php

namespace plainview\wordpress\activity_monitor\hooks\categories;

/**
	@brief		Category for theme actions.
	@since		2015-10-21 23:34:39
**/
trait Theme
{
	public function get_category()
	{
		// A hook category
		return Plainview_Activity_Monitor()->_( 'Theme' );
	}
}
