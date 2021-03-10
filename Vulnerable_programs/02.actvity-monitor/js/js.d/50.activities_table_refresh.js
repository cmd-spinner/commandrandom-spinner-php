/**
	@brief		Automatically refresh the activities table.
	@since		2015-07-22 21:32:29
**/
;(function( $ )
{
    $.fn.extend(
    {
        pvam_autorefresh_activities_table: function()
        {
            return this.each( function()
            {
                var $this = $( this );

                // Only refresh if the table has a timeout.
                var refresh_timeout = $( 'table', $this ).data( 'refresh_timeout' );

                if ( refresh_timeout < 1 )
                	return;

                /**
                	@brief		Contains the timeout.
                	@since		2015-07-22 21:35:58
                **/
                var timeout;

                /**
                	@brief		Refresh the table.
                	@since		2015-07-22 21:34:51
                **/
                $this.refresh = function()
                {
                	pvam_refresh_activities_table( function()
                	{
                		// Reapply the sort.
                		sorttable.makeSortable( $( 'table', $this ).get( 0 ) );
                		$this.set_timeout();
                	} );
                }

                /**
                	@brief		Set the timeout.
                	@since		2015-07-22 21:36:27
                **/
                $this.set_timeout = function()
                {
                	clearTimeout( timeout );
					timeout = setTimeout( function()
					{
						$this.refresh();
					}, refresh_timeout * 60 * 1000 );
                }

                $this.set_timeout();

            }); // return this.each( function()
        } // plugin: function()
    }); // $.fn.extend({
} )( jQuery );
