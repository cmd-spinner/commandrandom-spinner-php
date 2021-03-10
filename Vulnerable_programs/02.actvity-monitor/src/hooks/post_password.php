<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Post password entered.
	@details	Hooks into the post_password_expires filter because that's the only thing available to us.
	@since		2015-11-26 15:50:19
**/
class post_password
	extends hook
{
	use categories\Posts;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'Post password entered.' );
	}

	public function hook()
	{
		if ( $this->is_disabled() )
			return;

		add_filter( 'post_password_expires', [ $this, 'do_log' ], $this->get_priority(), $this->get_parameter_count() );
	}

	public function log()
	{
		if ( ! isset( $_POST[ 'post_password' ] ) )
			return $this->parameters->get( 1 );

		// Try to get the nice name for the post.
		$url = wp_get_referer();
		$post_id = url_to_postid( $url );
		$post = get_post( $post_id );

		$title = esc_html( $post->post_title );
		if ( trim( $title ) == '' )
			$title = $post->ID;
		$post = sprintf( '<a href="%s">%s</a>',
			get_permalink( $post->ID ),
			$title
		);
		// There!

		// Activity text: A user's password was reset.
		$s = $this->activity_monitor()->_( 'Post password entered: %s for %s',
			wp_unslash( $_POST[ 'post_password' ] ),
			$post
		);

		$this->html_and_execute( $s );

		// Since we are a filter, return the value we got.
		return $this->parameters->get( 1 );
	}
}
