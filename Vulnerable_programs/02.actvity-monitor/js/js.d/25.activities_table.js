/**
	@brief		Refresh the activities table.
	@since		2015-07-22 21:30:30
**/
function pvam_refresh_activities_table( callback )
{
	$ = jQuery;

	$current_table = $( 'table.activity_monitor_activities' );
	if ( $current_table.data( 'pvam_refreshing' ) === true )
		return;

	$current_table.data( 'pvam_refreshing', true );

	$.get( document.location )
		.done( function( data )
		{
			$data = $( data );

			var $table = $( 'table.activity_monitor_activities', $data );
			if ( $table.length < 1 )
				return;

			// Replace the table body.
			$( 'tbody', $current_table ).replaceWith( $( 'tbody', $table ) );

			// We should also replace the navigation, in case the amount of pages have changed.
			var $tablenav_pages = $( '.tablenav-pages' );
			$tablenav_pages.empty();
			$( '.tablenav-pages', $data ).first().appendTo( $tablenav_pages );

			// Bold / unbold each ajax button.
			var $buttons = $( 'form.filter_settings .button-secondary', $data );
			$.each( $buttons, function( index, item )
			{
				var $item = $( item );
				// Get the item ID
				var id = $item.prop( 'id' );

				// Does a button exist with this id?
				var $button = $( '#' + id );
				if ( $button.length < 1 )
					return;

				// Replace the buttons style with the new style.
				var new_css = $item.css( 'font-weight' );
				$button.css( 'font-weight', new_css );
			} );
		} )
		.then( function()
		{
			$current_table.data( 'pvam_refreshing', false );
			if ( callback !== undefined )
				callback();
		} );
}
