<?php

namespace plainview\wordpress\activity_monitor\hooks\categories;

/**
	@brief		Category for terms.
	@since		2015-10-21 21:24:01
**/
trait Terms
{
	public function get_category()
	{
		// A hook category
		return Plainview_Activity_Monitor()->_( 'Taxonomy terms' );
	}
}
