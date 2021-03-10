<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A comment was approved.
	@since		2015-12-04 09:30:47
**/
class wp_set_comment_status_approve
	extends comment_hook
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A comment was approved.' );
	}

	public function get_status_type()
	{
		return 'approve';
	}

	public function get_verb()
	{
		// Comment verb: A comment was XXX
		return $this->activity_monitor()->_( 'approved' );
	}

}
