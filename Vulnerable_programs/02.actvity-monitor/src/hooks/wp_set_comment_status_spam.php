<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A comment was spammed.
	@since		2015-12-04 09:30:47
**/
class wp_set_comment_status_spam
	extends comment_hook
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A comment was spammed.' );
	}

	public function get_status_type()
	{
		return 'spam';
	}

	public function get_verb()
	{
		// Comment verb: A comment was XXX
		return $this->activity_monitor()->_( 'spammed' );
	}

}
