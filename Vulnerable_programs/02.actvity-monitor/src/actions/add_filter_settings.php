<?php

namespace plainview\wordpress\activity_monitor\actions;

/**
	@brief		Add filter settings to the filter settings form.
	@since		2015-11-06 23:49:41
**/
class add_filter_settings
	extends action
{
	/**
		@brief		IN: The filters settings object, containing the current settings.
		@since		2015-11-08 22:50:44
	**/
	public $filters_settings;

	/**
		@brief		IN: The form2 object.
		@since		2015-11-08 22:28:06
	**/
	public $form;

	/**
		@brief		Convenience method to return all of the fieldsets.
		@since		2015-11-08 22:28:18
	**/
	public function get_fieldsets()
	{
		$r = [];

		foreach ( $this->form->inputs as $input )
		{
			if ( ! is_a( $input, 'plainview\\sdk_pvam\\form2\\inputs\\fieldset' ) )
				continue;
			$r []= $input;
		}

		return $r;
	}
}
