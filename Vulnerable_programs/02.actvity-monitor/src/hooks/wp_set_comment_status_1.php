<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A comment was reapproved.
	@since		2015-12-04 09:30:47
**/
class wp_set_comment_status_1
	extends comment_hook
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A comment was reapproved.' );
	}

	public function get_status_type()
	{
		return 1;
	}

	public function get_verb()
	{
		// Comment verb: A comment was XXX
		return $this->activity_monitor()->_( 'reapproved' );
	}

}
