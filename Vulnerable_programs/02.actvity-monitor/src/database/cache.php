<?php

namespace plainview\wordpress\activity_monitor\database;

use \plainview\sdk_pvam\collections\collection;

/**
	@brief		Blog and user caching class.
	@since		2014-05-11 09:59:19
**/
class cache
	extends \plainview\sdk_pvam\collections\collection
{
	/**
		@brief		Return the AM instance.
		@since		2014-05-10 23:20:53
	**/
	public function am()
	{
		return \plainview\wordpress\activity_monitor\Plainview_Activity_Monitor::instance();
	}

	/**
		@brief		Convert a blog ID to a linked blog name, if possible.
		@since		2014-05-10 23:15:09
	**/
	public function blog_html( $blog_id )
	{
		if ( ! $this->has( 'blogs' ) )
		{
			$blogs = new collection;
			$this->set( 'blogs', $blogs );
		}
		else
			$blogs = $this->get( 'blogs' );

		if ( ! $blogs->has( $blog_id ) )
		{
			if ( function_exists( 'switch_to_blog' ) )
				switch_to_blog( $blog_id );
			$blogname = get_bloginfo( 'blogname' );
			if ( ! $blogname )
				$blogname = $this->am()->_( 'Deleted blog %s', $blog_id );
			else
			{
				$blogname = sprintf( '<a href="%s">%s</a>', get_bloginfo( 'url' ), $blogname );
				if ( function_exists( 'switch_to_blog' ) )
					restore_current_blog();
			}
			$blogs->set( $blog_id, $blogname );
		}

		return $blogs->get( $blog_id );
	}

	/**
		@brief		Convert a user ID to a username, if possible.
		@since		2014-05-10 23:18:27
	**/
	public function user_html( $user_id)
	{
		if ( ! $this->has( 'users' ) )
		{
			$users = new collection;
			$this->set( 'users', $users );
		}
		else
			$users = $this->get( 'users' );

		if ( ! $users->has( $user_id ) )
		{
			if( $user_id == 0 )
				$username = 'Guest';
			else
			{
				$userdata = get_userdata( $user_id );
				if ( $userdata )
				{
					$username = $this->am()->_( '<span title="User ID %s">%s</span>', $userdata->ID, $userdata->data->user_login );
				}
				else
				{
					$username = $this->am()->_( 'Deleted user %s', $user_id );
				}
			}
			$users->set( $user_id, $username );
		}

		return $users->get( $user_id );
	}
}