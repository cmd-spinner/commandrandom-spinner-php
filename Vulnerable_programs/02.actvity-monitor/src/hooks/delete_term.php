<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A term was deleted.
	@since		2015-10-21 21:25:02
**/
class delete_term
	extends hook
{
	use categories\Terms;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A taxonomy term was deleted.' );
	}

	public function get_parameter_count()
	{
		// do_action( 'delete_term', $term, $tt_id, $taxonomy, $deleted_term );
		return 4;
	}

	public function log()
	{
		// Activity text: Taxonomy term deleted
		$s = $this->activity_monitor()->_( '%s term %s deleted',
			$this->em( $this->parameters->get( 3 ) ),
			$this->em( $this->parameters->get( 4 )->name )
		);
		$this->html()->append( $s );
		$this->html_and_execute();
	}
}
