<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Comment approved, reset, deleted, etc.
	@since		2014-05-04 21:18:41
**/
class wp_set_comment_status
	extends hook
{
	use categories\Comments;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A comment was approved, reset, etc.' );
	}

	public function get_parameter_count()
	{
		return 2;
	}

	public function log()
	{
		$comment_id = $this->parameters->get( 1 );
		$comment_data = get_comment( $comment_id );

		// Wordpress is autocleaning things.
		if ( $comment_data === null )
			return;

		$post_id = $comment_data->comment_post_ID;

		switch( $this->parameters->get( 2 ) )
		{
			case '0':
				// Activity text for comments: Comment reset
				$verb = $this->activity_monitor()->_( 'reset' );
				break;
			case '1':
				// Activity text for comments: Comment reapproved
				$verb = $this->activity_monitor()->_( 'reapproved' );
				break;
			case 'approve':
				// Activity text for comments: Comment approved
				$verb = $this->activity_monitor()->_( 'approved' );
				break;
			case 'hold':
				// Activity text for comments: Comment held back
				$verb = $this->activity_monitor()->_( 'held back' );
				break;
			case 'spam':
				// Activity text for comments: Comment spammed
				$verb = $this->activity_monitor()->_( 'spammed' );
				break;
			case 'trash':
				// Activity text for comments: Comment trashed
				$verb = $this->activity_monitor()->_( 'trashed' );
				break;
		}

		$url = sprintf( '%s#comment-%s', get_permalink( $post_id ), $comment_id );

		// Activity text: Comment VERBED, with url.
		$s = $this->activity_monitor()->_( 'Comment <a href="%s">%s</a>.', $url, $verb );

		$this->html_and_execute( $s );
	}
}
