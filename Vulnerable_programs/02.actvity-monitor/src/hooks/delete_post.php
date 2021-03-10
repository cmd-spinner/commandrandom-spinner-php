<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A post is deleted permanently.
	@since		2014-05-04 21:18:41
**/
class delete_post
	extends posts
{
	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A post was deleted permanently.' );
	}

	public function log_post()
	{
		$this->post = get_post( $this->parameters->get( 1 ) );

		if ( ! $this->post_is_good() )
			return false;

		// Activity text: A post was deleted.
		$s = $this->activity_monitor()->_( 'Post deleted: %s', $this->post_html() );

		$this->html_and_execute( $s );
	}
}
