<?php

namespace plainview\wordpress\activity_monitor\traits;

use \plainview\wordpress\activity_monitor\actions;
use \plainview\wordpress\activity_monitor\database as db;

/**
	@brief		SQL-related callbacks and what not.
	@since		2014-05-03 09:04:32
**/
trait database
{
	/**
		@brief		Constructor this these related traits.
		@since		2014-05-03 09:06:33
	**/
	public function _database_construct()
	{
		$this->add_action( 'plainview_activity_monitor_list_activities', 5 );
		$this->add_action( 'plainview_activity_monitor_list_activities', 'plainview_activity_monitor_list_activities_actual', 100 );
		$this->add_action( 'plainview_activity_monitor_list_distinct_values' );
		$this->add_action( 'plainview_activity_monitor_log_hook', 100 );
		$this->add_action( 'plainview_activity_monitor_prune_activities', 100 );
		$this->add_action( 'plainview_activity_monitor_remove_activities', 100 );
	}

	/**
		@brief		Returns the complete, prefixed name of a database table.
		@since		2014-05-03 14:59:02
	**/
	public static function get_table_name( $table = 'activities' )
	{
		$class = static::instance();
		$r = $class->wpdb->base_prefix;

		switch( $table )
		{
			case 'activities':
			default:
				$r .= 'pv_am_activities';
		}

		return $r;
	}

	/**
		@brief		Return a collection of activities.
		@since		2014-05-03 19:33:02
	**/
	public function plainview_activity_monitor_list_activities( $action )
	{
		// If no filters_settings is specified, use the user's.
		if ( ! is_object( $action->filters_settings ) )
			$action->filters_settings = db\filters_settings::load();

		// Conv
		$fs = $action->filters_settings;

		if ( $action->count )
			$action->select = 'COUNT(*) as rows';
		else
		{
			if ( $action->page > 0 )
				$action->limit = sprintf( 'LIMIT %s,%s', ( $action->page - 1 ) * $action->per_page, $action->per_page );

			if ( ! $action->ascending )
				$action->order .= ' DESC';

			$action->select = '*';
		}

		foreach( [
				'blog_id',
				'hook',
				'ip',
				'user_id',
			] as $key )
		{
			$property = $key . 's_to_exclude';
			$values = $fs->get_filter( $property );
			if ( count( $values ) > 0 )
				$action->where[] = sprintf( "`%s` NOT IN ('%s')", $key, implode( "','", $values ) );
			$property = $key . 's_to_include';
			$values = $fs->get_filter( $property );
			if ( count( $values ) > 0 )
				$action->where[] = sprintf( "`%s` IN ('%s')", $key, implode( "','", $values ) );
		}

		$value = $fs->get_filter( 'time_days_maximum', 0 );
		if ( $value > 0 )
		{
			$time = strtotime( '-' . $value . ' days' );
			$date = date( 'Y-m-d', $time );
			$action->where[] = sprintf( "`dt_created` >= '%s'", $date );
		}

		$value = $fs->get_filter( 'time_days_minimum', 0 );
		if ( $value > 0 )
		{
			$time = strtotime( '-' . $value . ' days' );
			$date = date( 'Y-m-d', $time );
			$action->where[] = sprintf( "`dt_created` <= '%s'", $date );
		}

		$value = $fs->get_filter( 'limit', 0 );
		if ( $value > 0 )
			$action->limit = sprintf( 'LIMIT %s', $value );

		$texts_to_include = $fs->get_filter( 'texts_to_include', [] );
		if ( count( $texts_to_include ) > 0 )
			foreach( $texts_to_include as $text )
				$action->where[] = sprintf( "`data` LIKE '%%%s%%'", $text );

		$texts_to_exclude = $fs->get_filter( 'texts_to_exclude', [] );
		if ( count( $texts_to_exclude ) > 0 )
			foreach( $texts_to_exclude as $text )
				$action->where[] = sprintf( "`data` NOT LIKE '%%%s%%'", $text );
	}

	/**
		@brief		This method is called once the action has been prepared by everyone else.
		@since		2015-11-10 19:23:21
	**/
	public function plainview_activity_monitor_list_activities_actual( $action )
	{
		// Assemble the where string.
		if ( count( $action->where ) > 0 )
			$where = sprintf( 'WHERE %s', implode( ' AND ', $action->where ) );
		else
			$where = '';

		$query = sprintf(
			'SELECT %s FROM %s
			%s
			%s
			%s
			%s',
			$action->select,
			static::get_table_name( 'activities' ),
			$where,
			$action->group_by,
			$action->order,
			$action->limit
		);

		$this->debug( 'Running query: %s', $query );
		$results = $this->query( $query );

		if ( $action->count )
		{
			$action->result = $results[ 0 ][ 'rows' ];
			// If the user has requested a max limit, return either the row count of the limit, whichever is smaller.
			$action->result = min( $action->result, $action->filters_settings->get_filter( 'limit', PHP_INT_MAX ) );
		}
		else
		{
			if ( $action->return_objects )
			{
				$action->result = db\activity::sqls( $results );
				// Convert the data array to an object.
				foreach( $action->result as $index => $result )
					$action->result[ $index ]->data = (object)$action->result[ $index ]->data;
			}
			else
				$action->result = $results;
		}
	}

	/**
		@brief		Fill the action with collections of the distinct values in the DB.
		@since		2014-05-04 10:04:14
	**/
	public function plainview_activity_monitor_list_distinct_values( $action )
	{
		$query = sprintf( "SELECT DISTINCT blog_id FROM `%s` ORDER BY blog_id", static::get_table_name( 'activities' ) );
		$this->debug( 'Running query: %s', $query );
		$results = $this->query( $query );
		foreach( $results as $result )
			$action->blog_ids->set( $result[ 'blog_id' ], true );

		$query = sprintf( "SELECT DISTINCT hook FROM `%s` ORDER BY hook", static::get_table_name( 'activities' ) );
		$this->debug( 'Running query: %s', $query );
		$results = $this->query( $query );
		foreach( $results as $result )
			$action->hooks->set( $result[ 'hook' ], true );

		$query = sprintf( "SELECT DISTINCT ip FROM `%s` ORDER BY ip", static::get_table_name( 'activities' ) );
		$this->debug( 'Running query: %s', $query );
		$results = $this->query( $query );
		foreach( $results as $result )
			$action->ips->set( $result[ 'ip' ], true );

		$query = sprintf( "SELECT DISTINCT user_id FROM `%s` ORDER BY user_id", static::get_table_name( 'activities' ) );
		$this->debug( 'Running query: %s', $query );
		$results = $this->query( $query );
		foreach( $results as $result )
			$action->user_ids->set( $result[ 'user_id' ], true );
	}

	/**
		@brief		Adds this hook to the log.
		@since		2014-05-03 18:48:57
	**/
	public function plainview_activity_monitor_log_hook( $action )
	{
		// Don't log it if it's finished.
		if ( $action->is_finished() )
			return;

		// Only log this hook if it is set to be logged.
		$logged = new actions\get_logged_hooks;
		$logged->execute();
		if ( ! $logged->logged_hooks->has( $action->hook->get_id() ) )
			return;

		$action->activity = $action->create_activity();
		$action->activity->db_update();

		// We need a 5% chance of pruning.
		if ( rand( 1, 100 ) > 5 )
			return;

		// And now we prune.
		$prune_activities = new actions\prune_activities;
		$prune_activities->count = $this->get_site_option( 'activities_in_database' );
		$prune_activities->execute();
	}

	/**
		@brief		Keep only a certain amount of activities in the database.
		@since		2014-07-08 12:08:40
	**/
	public function plainview_activity_monitor_prune_activities( $action )
	{
		if ( $action->is_finished() )
			return;
		$query = sprintf( "SELECT ( `id` ) FROM `%s` ORDER BY `id` DESC LIMIT %s,1",
			static::get_table_name( 'activities' ),
			$action->count
		);
		$this->debug( 'Running query: %s', $query );
		$results = $this->query( $query );
		$id = reset( $results );
		$id = $id[ 'id' ];
		$query = sprintf( "DELETE FROM `%s` WHERE `id` <= '%s'",
			static::get_table_name( 'activities' ),
			$id
		);
		$this->debug( 'Running query: %s', $query );
		$results = $this->query( $query );
	}

	/**
		@brief		Deletes activities from the DB.
		@since		2014-05-04 01:07:47
	**/
	public function plainview_activity_monitor_remove_activities( $action )
	{
		if ( $action->is_finished() )
			return;
		$query = sprintf( 'DELETE FROM %s WHERE id in (%s)', static::get_table_name( 'activities' ), implode( ',', $action->ids->to_array() ) );
		$this->debug( 'Running query: %s', $query );
		$this->query( $query );
	}
}
