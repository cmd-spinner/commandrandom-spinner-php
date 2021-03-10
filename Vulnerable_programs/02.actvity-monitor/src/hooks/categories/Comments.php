<?php

namespace plainview\wordpress\activity_monitor\hooks\categories;

/**
	@brief		Category for comments.
	@since		2015-07-05 20:44:54
**/
trait Comments
{
	public function get_category()
	{
		// A hook category
		return Plainview_Activity_Monitor()->_( 'Comments' );
	}
}
