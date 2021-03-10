<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A comment was reset.
	@since		2015-12-04 09:30:47
**/
class wp_set_comment_status_0
	extends comment_hook
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A comment was reset.' );
	}

	public function get_status_type()
	{
		return 0;
	}

	public function get_verb()
	{
		// Comment verb: A comment was XXX
		return $this->activity_monitor()->_( 'reset' );
	}

}
