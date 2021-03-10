<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A comment was deleted.
	@since		2015-12-04 09:30:47
**/
class delete_comment
	extends comment_hook
{
	public function get_action_name()
	{
		return $this->get_hook();
	}

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A comment was deleted.' );
	}

	public function get_status_type()
	{
		return false;
	}

	public function get_verb()
	{
		// Comment verb: A comment was XXX
		return $this->activity_monitor()->_( 'deleted' );
	}

}
