<?php

namespace plainview\wordpress\activity_monitor\actions;

use \plainview\sdk_pvam\collections\collection;

/**
	@brief		Return an array of activity DB objects.
	@details	The action is run as follows:

	* list_activities action is created.
	* filters_settings is inserted.
	* execute()
	* AM will prepare the basic query: preliminary select, some wheres, limit, etc.
	* The action is sent to other interested plugins
	* The action arrives at plainview_activity_monitor_list_distinct_values, which does the final setting up and the query exec.
	* SQL result is put in $result.

	If you want paging, set the $page to > 0.
	Set some form of limit, else all objects in the DB will be retrieved. If you don't want paging, at least set the limit filter setting to > 0.

	@since		2014-05-11 09:57:48
**/
class list_activities
	extends action
{
	/**
		@brief		[IN]: Fetch results in ascending or descending value?
		@since		2014-05-03 19:26:36
	**/
	public $ascending = false;

	/**
		@brief		[IN]: Return a count of rows?
		@since		2014-05-03 19:52:45
	**/
	public $count = false;

	/**
		@brief		IN: The filters settings object that decides what is to be shown and how.
		@since		2015-11-10 19:25:28
	**/
	public $filters_settings;

	/**
		@brief		[OUT]: Group by SQL string.
		@since		2015-11-17 14:20:18
	**/
	public $group_by = '';

	/**
		@brief		[OUT]: SQL limit string.
		@since		2015-11-18 16:57:20
	**/
	public $limit = '';

	/**
		@brief		[OUT]: SQL order.
		@since		2015-11-10 19:27:59
	**/
	public $order = 'ORDER BY id';

	/**
		@brief		[IN]: How many rows per page?
		@since		2014-05-03 19:31:30
	**/
	public $per_page = 100;

	/**
		@brief		[IN]: Page to fetch.
		@see		$per_page
		@since		2014-05-03 19:26:36
	**/
	public $page = false;

	/**
		@brief		OUT: The activity objects are placed here.
		@details	if count, then as a number, else an array.
		@since		2015-11-10 19:26:52
	**/
	public $result;

	/**
		@brief		[IN]: Convert the result as activity objects or just return the SQL result as is.
		@since		2015-11-17 14:23:22
	**/
	public $return_objects = true;

	/**
		@brief		[OUT]: The select string.
		@since		2015-11-10 19:25:03
	**/
	public $select = '';

	/**
		@brief		[OUT]: The SQL where array.
		@since		2015-11-10 19:24:18
	**/
	public $where = [];
}
