<?php

namespace plainview\wordpress\activity_monitor\traits;

use plainview\wordpress\activity_monitor\actions;

trait activities
{
	public function _activities_construct()
	{
		$this->add_action( 'plainview_activity_monitor_display_activity_table_column', 100 );
		$this->add_action( 'plainview_activity_monitor_get_activity_description', 5 );
		$this->add_action( 'plainview_activity_monitor_get_activity_table_columns', 5 );
	}

	/**
		@brief		Convert some of the activity's data into a column in the activity table.
		@since		2015-10-19 17:47:24
	**/
	public function plainview_activity_monitor_display_activity_table_column( $action )
	{
		// Honor other actions.
		if ( $action->is_finished() )
			return;

		switch( $action->column )
		{
			case 'blog_id':
				$blog_id = $action->activity->blog_id;
				$blogname = $this->cache()->blog_html( $blog_id );
				$action->row->td( 'blog_id' )->text( $blogname );
			break;
			case 'description':
				$get_activity_description = new actions\get_activity_description();
				$get_activity_description->activity = $action->activity;
				$get_activity_description->execute();
				$action->row->td( 'description' )->text( $get_activity_description->data );
			break;
			case 'dt_created':
				$time = strtotime( $action->activity->dt_created );
				// Add the blog timezone
				$offset = wp_timezone_override_offset();
				$time += $offset * HOUR_IN_SECONDS;
				$action->row->td( 'dt_created' )->text( date( 'Y-m-d H:i:s', $time ) );
			break;
			case 'hook':
				$action->row->td( 'hook' )->text( $action->activity->hook );
			break;
			case 'ip':
				$action->row->td( 'ip' )->text( long2ip( $action->activity->ip ) );
			break;
			case 'user_id':
				$user_id = $action->activity->user_id;
				$username = $this->cache()->user_html( $user_id );
				$action->row->td( 'user_id' )->text( $username );
			break;
		}
	}

	/**
		@brief		Displays the HTML in the data.
		@details	This is a simple fallback, in case no other plugins want to return anything more detailed.
		@since		2014-05-03 23:57:43
	**/
	public function plainview_activity_monitor_get_activity_description( $action )
	{
		$action->data = $action->activity->data->html;
	}

	/**
		@brief		Return the table columns we natively support.
		@since		2015-10-19 17:34:38
	**/
	public function plainview_activity_monitor_get_activity_table_columns( $action )
	{
		if ( $action->is_finished() )
			return;

		// Table column name
		$action->columns->set( 'dt_created', $this->_( 'Timestamp' ) );
		// Table column name
		$action->columns->set( 'blog_id', $this->_( 'Blog' ) );
		// Table column name
		$action->columns->set( 'hook', $this->_( 'Hook' ) );
		// Table column name
		$action->columns->set( 'user_id', $this->_( 'User' ) );
		// Table column name
		$action->columns->set( 'ip', $this->_( 'IP address' ) );
		// Table column name
		$action->columns->set( 'description', $this->_( 'Description' ) );
	}
}
