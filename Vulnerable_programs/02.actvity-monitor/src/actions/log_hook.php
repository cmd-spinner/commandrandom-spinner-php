<?php

namespace plainview\wordpress\activity_monitor\actions;

use \plainview\wordpress\activity_monitor\database\activity;
use \plainview\wordpress\activity_monitor\hooks\data;

/**
	@brief		Log the hook into the database.
	@since		2014-05-11 09:58:26
**/
class log_hook
	extends action
{
	use \plainview\sdk_pvam\traits\method_chaining;

	/**
		@brief		The ID of the blog.
		@since		2014-05-03 19:39:44
	**/
	public $blog_id;

	/**
		@brief		The data object.
		@since		2014-05-01 22:36:18
	**/
	public $data;

	/**
		@brief		The date/time of this activity.
		@since		2014-05-01 22:07:53
	**/
	public $dt_created;

	/**
		@brief		The name of the hook.
		@since		2014-05-01 22:08:13
	**/
	public $hook;

	/**
		@brief		Needed when copying the fields to the activity. Don't use this.
		@since		2014-05-04 01:16:20
	**/
	public $id;

	/**
		@brief		The client's IP address.
		@since		2014-05-01 22:08:38
	**/
	public $ip;

	/**
		@brief		ID of logged in user, if any.
		@since		2014-05-01 22:08:38
	**/
	public $user_id;

	/**
		@brief		Constructor.
		@since		2014-05-01 22:06:52
	**/
	public function _construct()
	{
		$this->blog_id = get_current_blog_id();
		$this->data = new data;
		$this->data->__parent = $this;
		$this->dt_created = date( 'Y-m-d H:i:s', time() );
		if ( isset( $_SERVER[ 'HTTP_X_FORWARDED_FOR' ] ) )
			$ip = $_SERVER[ 'HTTP_X_FORWARDED_FOR' ];
		else
			$ip = $_SERVER[ 'REMOTE_ADDR' ];
		$this->ip = ip2long( $ip );
		$this->user_id = get_current_user_id();
	}

	/**
		@brief		Gets the instance of the activity monitor.
		@since		2014-05-03 14:40:51
	**/
	public function activity_monitor()
	{
		return \plainview\wordpress\activity_monitor\Plainview_Activity_Monitor::instance();
	}

	/**
		@brief		Sets the ID of the blog.
		@since		2014-05-03 15:26:51
	**/
	public function blog_id( $blog_id )
	{
		return $this->set_int( 'blog_id', $blog_id );
	}

	/**
		@brief		Create a DB activity.
		@since		2014-05-03 20:02:24
	**/
	public function create_activity()
	{
		$activity = new activity;
		foreach( activity::keys() as $key )
			$activity->$key = $this->$key;
		$activity->hook = $this->hook->get_hook();
		return $activity;
	}

	/**
		@brief		Sets the name of the hook.
		@since		2014-05-03 15:26:51
	**/
	public function hook( $hook )
	{
		return $this->set_key( 'hook', $hook );
	}

	/**
		@brief		Sets the IP address.
		@since		2014-05-03 15:26:51
	**/
	public function ip( $ip )
	{
		return $this->set_int( 'ip', $ip );
	}

	/**
		@brief		Return the IP address as an HTML (text) string.
		@since		2014-05-03 15:50:39
	**/
	public function ip_html()
	{
		return long2ip( $this->ip );
	}

	/**
		@brief		Sets the user's ID.
		@since		2014-05-03 15:26:51
	**/
	public function user_id( $user_id = 0 )
	{
		return $this->set_int( 'user_id', $user_id );
	}

	/**
		@brief		Return the user data as an HTML string.
		@since		2014-05-03 15:51:07
	**/
	public function user_id_html()
	{
		$r = Plainview_Activity_Monitor()->_( 'Guest' );

		if ( $this->user_id > 0 )
		{
			$user_info = get_userdata( $this->user_id );
			$r = sprintf( '<span title="%s %s">%s</span>',
				$user_info->user_login,
				$this->user_id,
				$user_info->user_login
			 );
		}
		return $r;
	}
}
