<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A comment was held back.
	@since		2015-12-04 09:30:47
**/
class wp_set_comment_status_hold
	extends comment_hook
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A comment was held back.' );
	}

	public function get_status_type()
	{
		return 'hold';
	}

	public function get_verb()
	{
		// Comment verb: A comment was XXX
		return $this->activity_monitor()->_( 'held back' );
	}

}
