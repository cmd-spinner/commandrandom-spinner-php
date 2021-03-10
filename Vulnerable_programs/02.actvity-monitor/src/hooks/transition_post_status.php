<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A post is transitioned.
	@since		2014-05-04 21:18:41
**/
abstract class transition_post_status
	extends posts
{
	/**
		@brief		Get the verb for this transition. "published" or "trashed" or whatever.
		@since		2014-05-04 22:47:23
	**/
	abstract public function get_verb();

	public function log_post()
	{
		$this->post = $this->parameters->get( 1 );

		// Activity text: Post "Hello": Updated
		$s = $this->activity_monitor()->_( 'Post %s: %s',
			$this->get_verb(),
			$this->post_html()
		);

		$this->html_and_execute( $s );
	}
}
