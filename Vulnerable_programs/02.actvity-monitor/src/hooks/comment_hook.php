<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		General comment class.
	@since		2015-12-04 09:32:44
**/
class comment_hook
	extends hook
{
	use categories\Comments;

	/**
		@brief		We hook into the wp_set_comment_status action.
		@since		2015-12-04 09:43:58
	**/
	public function get_action_name()
	{
		return 'wp_set_comment_status';
	}

	/**
		@brief		Return the comment status we are looking for.
		@since		2015-12-04 09:33:43
	**/
	public function get_status_type()
	{
		return '';
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

		if( $this->parameters->get( 2 ) != $this->get_status_type() )
			return;

		$post_id = $comment_data->comment_post_ID;
		$url = sprintf( '%s#comment-%s', get_permalink( $post_id ), $comment_id );

		// Activity text: Comment VERBED, with url.
		$s = $this->activity_monitor()->_( 'Comment <a href="%s">%s</a>.',
			$url,
			$this->get_verb()
		);

		$this->html_and_execute( $s );
	}

	/**
		@brief		Return the verb that happened to this comment.
		@details	"reset", "approved", etc.
		@since		2015-12-04 09:55:59
	**/
	public function get_verb()
	{
	}

}
