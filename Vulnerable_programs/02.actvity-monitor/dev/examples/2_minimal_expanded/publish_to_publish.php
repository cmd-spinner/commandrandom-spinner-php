<?php

// Use a namespace to ensure that the class name does not conflict.
namespace PVAM_Example_2;

/**
	@brief		A hook for the publish_to_publish action.
	@since		2015-12-29 15:09:27
**/
class publish_to_publish
	extends \plainview\wordpress\activity_monitor\hooks\hook
{
	/**
		@brief		Override the hook class' category with our own.
		@details	This method is optional.
		@since		2015-12-29 15:10:02
	**/
	public function get_category()
	{
		return 'Example category';
	}

	/**
		@brief		Tell PVAM how many parameters to expect for this action.
		@details	This optional method is here just to point out a method that could be needed in the future.

		The hook default is one, which is exactly how many parameters publish_to_publish gives.

		@since		2015-12-29 15:16:54
	**/
	public function get_parameter_count()
	{
		// Default, as per Wordpress, is 1.
		return 1;
	}

	/**
		@brief		Return the priority where we want to hook into the action.
		@details	This optional method is here just to point out a method that could be needed in the future.
		@since		2015-12-29 18:07:47
	**/
	public function get_priority()
	{
		return 10;
	}

	/**
		@brief		Change the vendor from the default "Activity Monitor"
		@details	This method is also optional, but can give your hook that extra touch of professionalism. :)
		@since		2015-12-29 15:18:30
	**/
	public function get_vendor()
	{
		return 'Example Vendor';
	}

	/**
		@brief		Prepare the activity for logging.
		@details

		This method converts the hook into a string, optionally stores and extra data necessary, and then sends the string off for further handling.
		This means that other plugins can react to the action, or PVAM saving the activity to the DB.

		@since		2015-12-29 15:24:07
	**/
  	public function log()
	{
		// publish_to_publish supplies the post object as the FIRST parameter.
		$post = $this->parameters->get( 1 );

		// First, create a post link.
		$post_html = sprintf( '<a href="%s">%s</a>',
			get_permalink( $post->ID ),
			esc_html( $post->post_title )
		);

		// And put that link in a description.
		$s = sprintf( 'The post %s was updated.', $post_html );

		// And now put the string into the activity and send it off for logging.
		$this->html_and_execute( $s );

		// Do you want to add more than one string?You could also do this, if you have only one string to add.
		// $this->html()->append( $s1 );
		// $this->html()->append( $s2 );
		// $this->html_and_execute();
	}
}
