<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A page was visited in the front-end.
	@since		2014-10-17 07:19:36
**/
class wp_head
	extends hook
{
	use categories\Page_Views;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A page was visited in the front-end. Does not work if the pages are cached.' );
	}

	public function log()
	{
		$link = $this->activity_monitor()->current_url();
		$title = $link;

		if ( is_single() || is_page() )
			$title = get_the_title();

		// Activity text: A page was viewed.
		$s = $this->activity_monitor()->_( 'Page viewed: <a href="%s">%s</a>', $link, $title );

		$this->html_and_execute( $s );
	}
}
