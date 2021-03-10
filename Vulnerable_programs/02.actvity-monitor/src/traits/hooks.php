<?php

namespace plainview\wordpress\activity_monitor\traits;

use plainview\wordpress\activity_monitor\actions;
use plainview\wordpress\activity_monitor\hooks\collection as logged_hooks;

/**
	@brief		Hook-related methods.
	@since		2014-05-03 09:04:32
**/
trait hooks
{
	/**
		@brief		Constructor this these related traits.
		@since		2014-05-03 09:06:33
	**/
	public function _hooks_construct()
	{
		$this->add_action( 'plainview_activity_monitor_get_logged_hooks', 9 );
		$this->add_action( 'plainview_activity_monitor_manifest_hooks', 9 );
	}

	/**
		@brief		Return an array of logged hooks.
		@since		2014-05-09 12:16:04
	**/
	public function get_logged_hooks()
	{
		$r = $this->get_site_option( 'logged_hooks', [] );
		if ( ! is_array( $r ) )
			$r = [];
		return $r;
	}

	/**
		@brief		Add all grouped hooks into a select with optgroups.
		@details

		$o is an array containing:
		- input The select input into which to add the hooks.
		- [list_distinct_values] action
		- [manifest_hooks] action

		@since		2015-12-22 21:36:48
	**/
	public function grouped_hooks_to_select( $o = [] )
	{
		$o = (object)array_merge( [
			'list_distinct_values' => false,
			'manifest_hooks' => false,
		], $o );

		if ( ! $o->list_distinct_values )
		{
			$o->list_distinct_values = new actions\list_distinct_values();
			$o->list_distinct_values->execute();
		}

		if ( ! $o->manifest_hooks )
		{
			$o->manifest_hooks = new actions\manifest_hooks;
			$o->manifest_hooks->execute();
		}

		$input = $o->input;		// Conv.
		$database_hooks = clone( $o->list_distinct_values->hooks );

		$old_category = 'xxx';
		foreach( $o->manifest_hooks->by_category() as $hook )
		{
			if ( $hook->get_category() != $old_category )
			{
				$old_category = $hook->get_category();
				// Sanitize the category name.
				$category_slug = sanitize_title( $old_category );

				$optgroup = $input->optgroup( $category_slug );
				$optgroup->label( $old_category );
			}

			$name = $hook->get_hook();

			$optgroup->option( $name, $name );

			$database_hooks->forget( $name );
		}

		// Add the unknowns.
		if( count( $database_hooks ) > 0 )
		{
			$optgroup = $input->optgroup( 'unknown' );
			// Hooks that belong to an unknown category. Input label.
			$optgroup->label_( 'Unknown' );
			foreach( $database_hooks as $name => $ignore )
				$optgroup->option( $name, $name );
		}
	}

	/**
		@brief		Fill in which hooks we are set to log.
		@since		2014-07-06 17:43:01
	**/
	public function plainview_activity_monitor_get_logged_hooks( $action )
	{
		$logged_hooks = $this->get_site_option( 'logged_hooks', [] );
		foreach( $logged_hooks as $id )
			$action->logged_hooks->set( $id, $id );
	}

	/**
		@brief		Add all of our hooks to the collection.
		@since		2014-04-27 15:48:31
	**/
	public function plainview_activity_monitor_manifest_hooks( $action )
	{
		foreach ( [
			'activated_plugin',
			'admin_head',
			'created_term',
			'deactivated_plugin',
			'delete_comment',
			'delete_post',
			'delete_term',
			'delete_user',
			'draft_to_publish',
			'edit_term',
			'edited_term',
			'password_reset',
			'post_password',
			'profile_update',
			'publish_to_publish',
			'publish_to_trash',
			'retrieve_password',
			'switch_theme',
			'trash_to_publish',
			'updated_core',
			'updated_plugin',
			'updated_theme',
			'user_register',
			'wp_head',
			'wp_login',
			'wp_login_failed',
			'wp_logout',
			'wp_set_comment_status',
			'wp_set_comment_status_0',
			'wp_set_comment_status_1',
			'wp_set_comment_status_approve',
			'wp_set_comment_status_hold',
			'wp_set_comment_status_spam',
			'wp_set_comment_status_trash',
			'wpmu_delete_user',
		] as $hook )
		{
			$class = '\\plainview\\wordpress\\activity_monitor\\hooks\\' . $hook;
			$class = new $class;
			$class->register_with( $action->hooks );
		}
	}

	/**
		@brief		Sets the list of logged hooks.
		@since		2014-05-09 12:14:56
	**/
	public function set_logged_hooks( $logged_hooks )
	{
		$this->update_site_option( 'logged_hooks', $logged_hooks );
	}
}
