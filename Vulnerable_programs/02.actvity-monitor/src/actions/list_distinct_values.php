<?php

namespace plainview\wordpress\activity_monitor\actions;

use \plainview\sdk_pvam\collections\collection;

/**
	@brief		Return a collection of the distinct hook names in the database.
	@since		2014-05-04 10:02:08
**/
class list_distinct_values
	extends action
{
	/**
		@brief		OUT: A collection of blog IDs found in the database.
		@since		2014-05-04 01:07:01
	**/
	public $blog_ids;

	/**
		@brief		OUT: A collection of hook names that are found in the database.
		@since		2014-05-04 01:07:01
	**/
	public $hooks;

	/**
		@brief		OUT: A collection of IP addresses (as longs) found in the database.
		@since		2014-05-04 01:07:01
	**/
	public $ips;

	/**
		@brief		OUT: A collection of user IDs found in the database.
		@since		2014-05-04 01:07:01
	**/
	public $user_ids;

	public function __construct()
	{
		$this->blog_ids = new collection;
		$this->hooks = new collection;
		$this->ips = new collection;
		$this->user_ids = new collection;
	}

	/**
		@brief		Return a collection as an array suitable as a form2 select options array.
		@since		2014-05-04 10:13:21
	**/
	public function as_options( $type )
	{
		$array = $this->$type->to_array();
		$array = array_keys( $array );
		$array = array_combine( $array, $array );
		return $array;
	}
}
