<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A parent class for handling posts, containing various useful post handling methods.
	@since		2014-05-04 22:44:57
**/
abstract class posts
	extends hook
{
	use categories\Posts;

	/**
		@brief		Prehandles the logging for post hooks.
		@see		log_post()
		@since		2014-05-04 22:48:09
	**/
	public function log()
	{
		// User 0 = Wordpress itself. Ignore.
		if ( $this->log_hook->user_id < 1 )
			return;

		return $this->log_post();
	}

	/**
		@brief		The actual log function for this posts type of class.
		@since		2014-05-04 22:44:34
	**/
	abstract public function log_post();

	/**
		@brief		Returns the post title and link if $this->post is set.
		@since		2014-05-04 22:02:16
	**/
	public function post_html( $post = null )
	{
		if ( $post === null )
			$post = $this->post;
		$title = esc_html( $post->post_title );
		if ( trim( $title ) == '' )
			$title = $post->ID;
		return sprintf( '<a href="%s">%s</a>',
			get_permalink( $post->ID ),
			$title
		);
	}

	/**
		@brief		Is the post valid for logging? Autosaves and the like are not interesting.
		@since		2014-05-04 22:46:45
	**/
	public function post_is_good( $post = null )
	{
		if ( $post === null )
			$post = $this->post;
		if ( ! in_array( $post->post_status, [ 'publish', 'trash' ] ) )
			return false;
		return true;
	}
}
