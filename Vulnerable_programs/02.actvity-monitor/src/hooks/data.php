<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Contains data about the logged activity.
	@details

	The data object should, at the very least, contain an html property that is displayed to the user if there is no action to handle display of this activity.
	@since		2014-05-01 22:38:01
**/
class data
{
	use \plainview\sdk_pvam\traits\method_chaining;

	/**
		@brief		HTML description.
		@since		2014-05-01 22:38:18
	**/
	public $html;

	public function __construct()
	{
		$this->html = '';
	}

	/**
		@brief		Sets the HTML value. The arguments are sprintf'd.
		@since		2014-05-03 18:45:40
	**/
	public function html( $html )
	{
		$text = @call_user_func_array( 'sprintf', func_get_args() );
		if ( $text == '' )
			$text = $html;
		return $this->set_key( 'html', $text );
	}

	/**
		@brief		Load the data from this array.
		@since		2014-05-03 18:41:22
	**/
	public static function load( $array )
	{
		$r = new data;
		foreach( $array as $key => $value )
			$r->$key = $value;
		return $r;
	}

	/**
		@brief		Serialize this object.
		@since		2014-05-03 15:53:34
	**/
	public function serialize()
	{
		$r = [];
		foreach( $this as $key => $value )
			if ( strpos( $key, '__' ) !== 0 )
				$r[ $key ] = $value;
		return serialize( $r );
		return $r;
	}
}
