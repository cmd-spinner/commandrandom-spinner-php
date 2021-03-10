<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Editing taxonomy terms.
	@since		2015-10-21 21:25:02
**/
class edit_term
	extends hook
{
	use categories\Terms;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'Editing taxonomy terms. This is called before the terms are modified. Does not store any data.' );
	}

	public function get_parameter_count()
	{
		// do_action( "edit_term", $term_id, $tt_id, $taxonomy );
		return 3;
	}

	public function log()
	{
		// Save the previous term data in order to give edited_terms something to compare the new term with.
		$term_id = $this->parameters->get( 1 );
		$taxonomy = $this->parameters->get( 3 );

		// Convenience
		$am = $this->activity_monitor();

		$data = $am->collection();
		$am->__edit_terms = $data;

		$data->term = get_term_by( 'id', $term_id, $taxonomy );
		$data->taxonomy = $taxonomy;
	}
}
