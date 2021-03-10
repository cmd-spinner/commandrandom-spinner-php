<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		User deletion.
	@since		2014-05-04 20:43:14
**/
class wpmu_delete_user
	extends delete_user
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A user was deleted from the network.' );
	}
}
