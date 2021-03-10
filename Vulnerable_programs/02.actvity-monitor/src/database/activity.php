<?php

namespace plainview\wordpress\activity_monitor\database;

use plainview\wordpress\activity_monitor\hooks;

/**
	@brief		An activity row from the database.
	@details	The data column is STORED as serialized array that is USED as a data object.

	The class takes care of automatically serializing and unserializing the data.

	Since 2015-11-09, the data column is stored as a serialized array, instead of serialize+base64.
	The data is transparently converted when the activity is loaded from the db.
	The cost is one update per row per load.

	@since		2014-05-11 09:59:05
**/
class activity
{
	use \plainview\sdk_pvam\wordpress\traits\db_aware_object;

	/**
		@brief		The ID of the blog on which this activity happened.
		@since		2015-12-29 14:52:32
	**/
	public $blog_id;

	/**
		@brief		A hooks/data object in which plugins can store arbitrary data.
		@since		2015-12-29 14:52:53
	**/
	public $data;

	/**
		@brief		The date and time when the activity was created. The timestamp.
		@since		2015-12-29 14:53:32
	**/
	public $dt_created;

	/**
		@brief		The name of the hook as a string.
		@since		2015-12-29 14:53:47
	**/
	public $hook;

	/**
		@brief		Unique row ID in the database.
		@since		2015-12-29 14:52:24
	**/
	public $id;

	/**
		@brief		The IP address of the user as a long.
		@details	Use long2ip and ip2long to convert.
		@since		2015-12-29 14:53:57
	**/
	public $ip;

	/**
		@brief		The ID of the user that is logged in. Else 0.
		@since		2015-12-29 14:54:26
	**/
	public $user_id;

	public static function db_table()
	{
		return \plainview\wordpress\activity_monitor\Plainview_Activity_Monitor::instance()->get_table_name( 'activities' );
	}

	public static function keys()
	{
		return [
			'id',
			'blog_id',
			'data',
			'dt_created',
			'hook',
			'ip',
			'user_id',
		];
	}

	public static function serialize_keys( $o )
	{
		$o->data = $o->data->serialize();
		\plainview\sdk_pvam\wordpress\traits\db_aware_object::serialize_keys( $o );
	}

	public static function unserialize_keys( $object )
	{
		\plainview\sdk_pvam\wordpress\traits\db_aware_object::unserialize_keys( $object );

		$data = maybe_unserialize( $object->data );

		// If normal unserialization fails, then it is probably also base64 encoded. Upgrade.
		if ( ! is_array( $data ) )
		{
			$data = base64_decode( $object->data );
			$data = maybe_unserialize( $data );

			// Only resave if it was correctly unserialized.
			if ( is_array( $data ) )
			{
				$object->data = hooks\data::load( $data );
				$object->db_update();
			}
		}

		$object->data = $data;
		$object->data = hooks\data::load( $data );
	}
}
