<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A post is restored.
	@since		2014-05-04 21:18:41
**/
class trash_to_publish
	extends transition_post_status
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A post was restored.' );
	}

	public function get_verb()
	{
		// Activity text: A post was restored
		return $this->activity_monitor()->_( 'restored' );
	}
}
