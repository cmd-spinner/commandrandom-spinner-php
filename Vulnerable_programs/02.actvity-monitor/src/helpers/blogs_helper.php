<?php

namespace plainview\wordpress\activity_monitor\helpers;

/**
	@brief		Blog helper methods.
	@details	Class is using small letters to ease the dynamic loading.
	@since		2015-11-28 20:34:58
**/
class blogs_helper
{
	/**
		@brief		Convenience method to add a select of user writeable blogs to a forum.
		@details	The options array contains the following parameters.

		- [description]				Optional input description.
		- label						Input label.
		- form						The form2 object.
		- [multiple] = false		May the user select several blogs?
		- name						The name of the input. "blogs" is a good name.
		- [required] = false		Is the input required?
		- [size] = 0				Optional size, else the input is autosized.
		- value						Value or values to be selected.

		@since		2015-11-28 19:25:30
	**/
	public static function add_blog_list_input( $options )
	{
		$options = (object)array_merge( [
			'description' => '',
			'label' => '',
			'form' => null,
			'required' => true,
			'multiple' => true,
			'name' => '',
			'size' => 0,
			'value' => '',
		], $options );

		$input = $options->form->select( $options->name )
			->label_( $options->label )
			->value( $options->value );

		foreach( static::get_blogs() as $blog )
			$input->option( $blog->get_name(), $blog->get_id() );

		if ( $options->description != '' )
			$input->description( $options->description );

		if ( $options->multiple )
			$input->multiple();

		if ( $options->required )
			$input->required();

		if ( $options->size > 0 )
			$input->size( $options->size );
		else
			$input->autosize();

		return $input;
	}

	/**
		@brief		Return a blogs collection.
		@since		2015-11-28 20:36:36
	**/
	public static function get_blogs()
	{
		$r = new blogs\Collection();
		$blogs = wp_get_sites( [ 'limit' => PHP_INT_MAX ] );
		foreach( $blogs as $blog)
		{
			// After having retrieved the blog, get the details because wp_get_sites doesn't do that.
			$blog = get_blog_details( $blog[ 'blog_id' ] );
			$blog = blogs\Blog::make( $blog );
			$blog->id = $blog->id;
			$r->set( $blog->id, $blog );
		}
		$r->sort_logically();
		return $r;
	}
}