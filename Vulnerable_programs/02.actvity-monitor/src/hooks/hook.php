<?php

namespace plainview\wordpress\activity_monitor\hooks;

use \plainview\wordpress\activity_monitor\actions;

/**
	@brief		A hook that hooks into actions.
	@details

	- wp_login, profile_update, created_term are existing actions and therefore supposed to use the hook class.
	- ip_too_often, bruteforce_detect, send_to_htaccess, being hooks that we created ourselves, should use custom_hook.

	@see		custom_hook
	@since		2014-04-27 15:36:41
**/
class hook
{
	use \plainview\sdk_pvam\traits\method_chaining;

	/**
		@brief		The name of the hook - the default being the class name without the namespace.
		@since		2014-07-07 22:00:50
	**/
	public $hook;

	public function __construct()
	{
		$class = get_called_class();
		$class = preg_replace( '/.*\\\/', '', $class );
		$this->hook = $class;
	}

	/**
		@brief		Return the instance of the activity monitor.
		@since		2014-05-06 22:42:55
	**/
	public function activity_monitor()
	{
		return \plainview\wordpress\activity_monitor\Plainview_Activity_Monitor::instance();
	}

	/**
		@brief		Returns the linked blog name.
		@since		2014-05-07 00:12:58
	**/
	public function blog_html()
	{
		return sprintf( '<a href="%s">%s</a>',
			get_bloginfo( 'url' ),
			esc_html( get_bloginfo( 'blogname' ) )
		);
	}

	/**
		@brief		Sets the disabled flag of the hook.
		@since		2014-05-04 19:53:12
	**/
	public function disabled( $disabled = true )
	{
		if ( ! $disabled )
		{
			unset( $this->disabled );
			return $this;
		}
		else
			return $this->set_boolean( 'disabled', $disabled );
	}

	/**
		@brief		do_action this hook.
		@details	If you are creating hooks, it would be a good idea to run this.

		If your hook is listening to wp_login, an existing action, then this method is pointless.
		If your hook is creating something new, like what_a_great_idea, then run this method to inform everyone else.

		@see		get_do_action_parameters()
		@see		hook()
		@since		2015-12-02 15:06:33
	**/
	public function do_action()
	{
		$parameters = $this->get_do_action_parameters();
		$parameters = array_merge( [ $this->get_hook() ], $parameters );
		call_user_func_array( 'do_action', $parameters );
	}

	/**
		@brief		The internal method that is called when the hook is executed.
		@details	Does some housekeeping in the form of saving the parameters and prepping the log_hook action and then calling log().
		@since		2015-12-25 00:14:44
	**/
	public function do_log()
	{
		if ( $this->is_disabled() )
			return;

		$this->log_hook = new actions\log_hook();
		$this->log_hook->hook( $this );

		$args = func_get_args();
		$this->parameters = new \plainview\sdk_pvam\collections\collection;
		foreach( $args as $index => $arg )
			$this->parameters->set( $index + 1, $arg );

		return $this->log();
	}

	/**
		@brief		Convenience method to encase a string in em tags.
		@since		2015-11-06 10:25:07
	**/
	public function em( $s )
	{
		return '<em>' . $s . '</em>';
	}

	/**
		@brief		Return the name of the action into which we hook.
		@details	The default is the name of this hook. This is useful if your hook is called something, but actually hooks into something else.
		@see		get_hook()
		@see		hook()
		@see		wp_set_comment_status_0
		@since		2015-12-04 09:46:31
	**/
	public function get_action_name()
	{
		return $this->get_hook();
	}

	/**
		@brief		Return the hook's category.
		@details	The category is a word or short phrase.

		delete_post, publish_to_trash and draft_to_publish could have the category "Posts".
		user_register, wpmu_delete_user could have the category "Users".

		@since		2015-07-05 20:39:10
	**/
	public function get_category()
	{
		return Plainview_Activity_Monitor()->_( 'Uncategorized' );
	}

	/**
		@brief		Return the basename of this class.
		@since		2014-11-22 12:17:53
	**/
	public function get_class()
	{
		$r = get_called_class();
		$r = preg_replace( '/.*\\\\/', '', $r );
		return $r;
	}

	/**
		@brief		Get the name of the vendor.
		@since		2014-04-27 23:45:01
	**/
	public function get_description()
	{
		return 'Default description.';
	}

	/**
		@brief		Return an array of parameters to send to do_action.
		@details	The default is to return this hook object.
		@see		do_action()
		@since		2015-12-02 15:57:21
	**/
	public function get_do_action_parameters()
	{
		return [ $this ];
	}

	/**
		@brief		Get the name of the hook that this activity handles.
		@details	Override only if the name of the child class does not match the hook name.
		@since		2014-04-27 23:45:50
	**/
	public function get_hook()
	{
		return $this->hook;
	}

	/**
		@brief		Return an unique ID for this activity.
		@since		2014-05-01 11:25:31
	**/
	public function get_id()
	{
		return sprintf( '%s:%s',
			$this->get_class(),
			$this->get_hook()
		);
	}

	/**
		@brief		How many parameters should we tell Wordpress that we want?
		@since		2014-05-01 08:33:37
	**/
	public function get_parameter_count()
	{
		// Default, as per Wordpress, is 1.
		return 1;
	}

	/**
		@brief		The priority we want to hook into.
		@since		2014-05-01 08:33:10
	**/
	public function get_priority()
	{
		// Default, as per Wordpress, is 10.
		return 10;
	}

	/**
		@brief		Get the name of the vendor.
		@since		2014-04-27 23:45:01
	**/
	public function get_vendor()
	{
		return 'Activity Monitor';
	}

	/**
		@brief		Insert ourselves into the Wordpress hook.
		@details	If you are creating a new action, override this method and return null, else you might end up in a hook() do_action() loop.
		@see		do_action()
		@since		2014-04-27 23:47:03
	**/
	public function hook()
	{
		if ( $this->is_disabled() )
			return;
		add_action( $this->get_action_name(), [ $this, 'do_log' ], $this->get_priority(), $this->get_parameter_count() );
	}

	/**
		@brief		Retrieve or create the HTML collection for this hook.
		@since		2014-05-04 13:11:42
	**/
	public function html()
	{
		if ( ! isset( $this->__html ) )
			$this->__html = new \plainview\sdk_pvam\collections\html;
		return $this->__html;
	}

	/**
		@brief		Convenience method to set the HTML data and then execute the log_look action.
		@since		2014-05-04 11:59:56
	**/
	public function html_and_execute()
	{
		$args = func_get_args();

		$text = '';

		if ( count( $args ) > 0 )
		{
			$text = @call_user_func_array( 'sprintf', $args );
			if ( $text == '' )
				$text = $args[ 0 ];
		}

		$text = $this->html() . $text;

		$this->log_hook->data->html( $text );
		$this->html()->flush();
		$this->log_hook->execute();
	}

	/**
		@brief		Is this hook disabled?
		@since		2014-05-05 21:06:52
	**/
	public function is_disabled()
	{
		return isset( $this->disabled ) && ( $this->disabled == true );
	}

	/**
		@brief		Old method of beginning a log action.
		@details	Deprecated since 20151225. Use do_log instead.
		@see		do_log()
		@since		2014-05-03 15:21:54
	**/
	public function _log()
	{
		Plainview_Activity_Monitor()->deprecated_function( 'Use ->do_log() instead.' );
		$args = func_get_args();
		call_user_func_array( [ $this, 'do_log' ], $args );
	}

	/**
		@brief		Log the activity by creating it and then calling ->log_hook->execute().
		@details	This overriable method handles the creation and, optionally, logging of the activity, depending on the logged hook settings.

		The hook parameters are prepped in the ->parameters object and the log_hook action can be called with $this->log_hook->execute() action.

		The rule here is that the activity should describe enough information to the admin by self, without requiring the hook column be visible.

		Examples:
		 When updating a post, the text should be "Post 123 updated" instead of "Post 123" and relying on the user reading the hook column.
		 When activating a plugin, the text should be "Plugin XYZ activated" instead of "Plugin XYZ" and relying on the user reading the hook column.

		The rule for usernames is: only include usernames if the action has directly to do with a user: creation, logging in, deletion, logging out.
		If a post is updated the main activity is the post being updated, not that user xyz is updating the post.

		@see		html_and_execute()
		@since		2014-04-28 08:25:37
	**/
	public function log()
	{
	}

	/**
		@brief		Register ourself with a hooks collection.
		@since		2014-04-27 20:55:27
	**/
	public function register_with( $hooks )
	{
		$hooks->register( $this );
	}
}
