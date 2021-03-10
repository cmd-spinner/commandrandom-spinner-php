<?php

namespace plainview\wordpress\activity_monitor\helpers\blogs;

class Collection
	extends \plainview\sdk_pvam\collections\collection
{
	/**
		@brief		Sort with the root blog first, then the rest alphabetically.
		@since		2015-11-28 20:38:07
	**/
	public function sort_logically()
	{
		$this->sortBy( function( $item )
		{
			if ( $item->id == 1 )
				return 'AAA';
			return strtolower( $item->get_name() );
		} );
		return $this;
	}
}
