/**
	@brief		Ajaxify the selection of filters.
	@since		2015-07-07 16:11:48
**/
;(function( $ )
{
    $.fn.extend(
    {
        pvam_ajax_filters: function()
        {
            return this.each( function()
            {
                var $this = $( this );

				/**
					@brief		Handle the fetching and display of the ajaxed filter settings.
					@since		2015-07-07 20:43:51
				**/
				$this.handle_settings = function( $button )
				{
					var $popup = pvam_popup()
						.set_title( pvam_ajax_filters_settings.strings.modify_the_filter )
						.set_content( pvam_ajax_filters_settings.strings.please_wait_while_the_dialog_loads )
						.open();

					var type = $button.prop( 'name' );

					// Fetch the filters page.
					$.get( filters_url )
						.done( function( data )
						{
							$page = $( data );

							// Retrieve the form.
							var $form = $( 'form.activity_filters', $page );

							// From the form, hide all of the fieldsets.
							$( 'div.fieldset', $form ).hide();

							// Hide the fieldset headers.
							$( 'h3', $form ).hide();

							// Hide the "delete" button.
							$( 'input[name="delete"]', $form ).hide();

							// Unhide the fieldset we're looking for.
							var $fieldset = $( '.fieldset_' + type, $form ).show();

							// The save button we shall override.
							var $save = $( 'input[name="save"]', $form )
								.click( function( save_e )
								{
									save_e.preventDefault();

									$popup.set_content( pvam_ajax_filters_settings.strings.saving );

									var $save = $( this );
									var form_url = $form.prop( 'action' );

									// Assemble the form and send it in.
									$.ajax({
										'data' : $form.serialize() + '&save=save',
										'type' : 'post',
										'url' : form_url,
									} )
									.fail( function( jqXHR )
									{
										$popup.set_title( pvam_ajax_filters_settings.strings.error_saving_your_filter_settings );
										$popup.set_content( jqXHR.responseText );
									} )
									.done( function( data )
									{
										$popup.close();
										pvam_refresh_activities_table();
									});

									return false;
								} );

							// Only insert clear button if there are selects
							var $selects = $( 'select', $fieldset );
							if ( $selects.length > 0 )
							{
								// Insert a clear button next to the save.
								var $clear = $( '<br/>&nbsp;<br/><div class="secondary button">' + pvam_ajax_filters_settings.strings.clear_filters + '</div>' )
									.click( function()
									{
										$selects.val( 'value', [] );
										$save.click();
									} )
									.insertAfter( $save.parents( 'div.input_itself' ) );
							}

							// And put it in the popup.
							$popup.clear_content();
							$form.appendTo( $popup.$content );
						} ); // done
				}

                var filters_url = $( 'a.tab_activity_filters' ).prop( 'href' );

                // Load the CSS file.
                $( 'head' ).append( "<link rel='stylesheet' href='" + pvam_ajax_filters_settings.css + "' type='text/css' media='all' />" );

                var $container = $( '<span class="filters_container">&emsp;Filter: </span>' )
                	.appendTo( $( '.tablenav.top .alignleft', $this ) );

                // Extract the filter settings inputs from the filter settings form.
                $.each( $( 'form.filter_settings input', $this ), function ( index, item )
                {
                	$container.append( ' ' );
                	var $item = $( item );
                	$item
                		// move it to the container
                		.appendTo( $container )
                		.click( function( e )
                		{
                			e.preventDefault();
                			$this.handle_settings( $( this ) );
                		} );
                } );

            }); // return this.each( function()
        } // plugin: function()
    }); // $.fn.extend({
} )( jQuery );
