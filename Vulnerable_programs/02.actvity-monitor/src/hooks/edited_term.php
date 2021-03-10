<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		Finished editing taxonomy terms.
	@since		2015-10-21 21:25:02
**/
class edited_term
	extends hook
{
	use categories\Terms;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A taxonomy term was edited. This is called after the terms are modified.' );
	}

	public function get_parameter_count()
	{
		// do_action( "edited_term", $term_id, $tt_id, $taxonomy );
		return 3;
	}

	public function log()
	{
		// Convenience
		$am = $this->activity_monitor();

		// Edit term must have been called to store the old data.
		if ( ! isset( $am->__edit_terms ) )
			return;

		$data = $am->__edit_terms;

		$new_term = get_term_by( 'id', $data->term->term_id, $data->taxonomy );
		foreach( [ 'name', 'slug' ] as $type )
			if ( $data->term->$type != $new_term->$type )
			{
				// Activity text: Taxonomy term name changed from old to new
				$s = $am->_( '%s term %s changed from %s to %s',
					$this->em( $data->taxonomy ),
					$this->em( $type ),
					$this->em( $data->term->$type ),
					$this->em( $new_term->$type )
				);
				$this->html()->append( $s );
			}

		if ( count( $this->html() ) < 1 )
			return;

		$this->html_and_execute();
	}
}
