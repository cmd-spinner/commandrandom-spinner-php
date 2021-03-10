<?php

namespace plainview\wordpress\activity_monitor\hooks\categories;

/**
	@brief		Miscellaneous.
	@since		2015-07-05 23:19:12
**/
trait Misc
{
	public function get_category()
	{
		// A hook category
		return Plainview_Activity_Monitor()->_( 'Miscellaneous' );
	}
}
