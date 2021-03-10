<?php

namespace plainview\wordpress\activity_monitor\hooks;

/**
	@brief		A term was created.
	@since		2015-10-21 21:25:02
**/
class created_term
	extends hook
{
	use categories\Terms;

	public function get_description()
	{
		// Hook description
		return $this->activity_monitor()->_( 'A taxonomy term was created.' );
	}

	public function get_parameter_count()
	{
		// do_action( 'created_term', $term_id, $tt_id, $taxonomy );
		return 3;
	}

	public function log()
	{
		$taxonomy = $this->parameters->get( 3 );
		$term_id = $this->parameters->get( 1 );

		$term = get_term_by( 'id', $term_id, $taxonomy );

		// Activity text: Taxonomy term abc created
		$s = $this->activity_monitor()->_( '%s term %s created',
			$this->em( $taxonomy ),
			$this->em( $term->name )
		);
		$this->html()->append( $s );
		$this->html_and_execute();
	}
}
