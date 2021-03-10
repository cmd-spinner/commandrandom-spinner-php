<?php

namespace plainview\wordpress\activity_monitor\helpers\blogs;

/**
	@brief		Smart Blog class.
	@since		2015-11-28 20:41:05
**/
class Blog
{
	public $id;

	/**
		@brief		Allow construction of blog with ID.
		@since		2015-06-15 10:43:57
	**/
	public function __construct( $id = null )
	{
		if ( absint( $id ) > 0 )
			$this->id = absint( $id );
	}

	public function __toString()
	{
		$info = get_blog_details( $this->id );
		$r = $this->blogname ? $this->blogname : $this->domain . $this->path;
		return $r . '';
	}

	/**
		@brief		Return a unique ID for this blog.
		@details	This is the preferred way of getting the ID of the blog.
	**/
	public function get_id()
	{
		return $this->id;
	}

	/**
		@brief		Return the blog's name.
		@details	This is the preferred way of getting the ID of the blog name.
	**/
	public function get_name()
	{
		return $this->__toString();
	}

	public static function make( $data )
	{
		$r = new blog;
		foreach( (array)$data as $key=>$value )
			$r->$key = $value;
		if ( property_exists( $r, 'blog_id' ) )
			$r->id = intval( $r->blog_id );
		return $r;
	}

}
