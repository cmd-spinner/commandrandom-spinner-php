jQuery(document).ready( function( $ )
{
	$( '.pvam_overview_table' ).pvam_ajax_filters();
	$( '.pvam_overview_table' ).pvam_autorefresh_activities_table();
	$( 'form.plainview_form_auto_tabs' ).plainview_form_auto_tabs();
	$( 'table.pvam.with_groups' ).plainview_table_with_groups( {
		'collapsible' : 0,
		'collapse' : 15,
	} );	// Only the activities table.
	$( 'table.plugin_pack.with_groups' ).plainview_table_with_groups( {
		'collapsible' : 0,
		'collapse' : 1000,
	} );	// Only the plugin pack table. Do not collapse automatically.
} );
