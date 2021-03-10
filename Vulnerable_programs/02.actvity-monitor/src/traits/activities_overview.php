<?php

namespace plainview\wordpress\activity_monitor\traits;

use \plainview\wordpress\activity_monitor\actions;
use \plainview\wordpress\activity_monitor\database\filters_settings;
use \plainview\sdk_pvam\collections\collection;

trait activities_overview
{
	/**
		@brief		Initialize this trait.
		@since		2015-11-07 00:02:10
	**/
	public function activities_overview_trait_init()
	{
		$this->add_action( 'plainview_activity_monitor_add_filter_settings', 5 );
		$this->add_action( 'plainview_activity_monitor_save_filter_settings', 100 );
	}

	/**
		@brief		Return the script code neccessary to ajaxify the activities overview.
		@since		2015-07-07 23:22:13
	**/
	public function get_overview_ajax( $filters_settings )
	{
		$r = sprintf( '<script type="text/javascript">var pvam_ajax_filters_settings = {
			"css" : "%s/css/css.css",
			"strings" : {
				"clear_filters" : "%s",
				"error_saving_your_filter_settings" : "%s",
				"modify_the_filter" : "%s",
				"please_wait_while_the_dialog_loads" : "%s",
				"saving" : "%s"
			},
			};
			</script>',
			$this->paths( 'url' ),
			// Ajax filter settings popup: Clear the selected filter settings
			$this->_( 'Clear settings' ),
			// Ajax filter settings popup: Error
			$this->_( 'Error saving your filter settings' ),
			// Ajax filter settings popup: Heading when editing the settings
			$this->_( 'Modify the filter' ),
			// Ajax filter settings popup
			$this->_( 'Please wait while the dialog loads...' ),
			// Ajax filter settings popup: Saving setting
			$this->_( 'Saving...' )
		);

		// Generate the filter settings here, since it is so much faster and more efficient to make form2 render the form.
		$fsf = $this->form2();
		$fsf->css_class( 'filter_settings' );

		$filters = new actions\add_filter_settings();
		$filters->filters_settings = filters_settings::load();
		$filters->form = $this->form2();
		$filters->execute();

		foreach( $filters->get_fieldsets() as $fieldset )
		{
			$name = $fieldset->get_name();
			$button = $fsf->secondary_button( $name )
				->value( $fieldset->legend->get_label()->content );

			$has_filters = false;
			foreach( array_keys( $fieldset->inputs ) as $key )
				$has_filters |= $filters->filters_settings->get( 'filters' )->has( $key );
			if ( $has_filters )
				$button->css_style( 'font-weight: bold' );
		}

		$fsf->hidden();
		$r .= $fsf;

		return $r;
	}
	/**
		@brief		Display activities.
		@since		2014-05-03 20:22:31
	**/
	public function menu_activities_display( $list_activities )
	{
		$form = $this->form2();
		$r = '<div class="pvam_overview_table">';
		$table = $this->table();
		$table->css_class( 'activity_monitor_activities' );
		$table->css_class( 'striped' );

		$row = $table->head()->row();
		$table->bulk_actions()
			->form( $form )
			->add( $this->_( 'Delete' ), 'delete' )
			->cb( $row );

		$row->td( 'check_column' )
			->css_class( 'sorttable_nosort' );

		$table->css_class( 'sortable stable_sort' );

		$refresh_timeout = $this->get_site_option( 'refresh_timeout' );
		if ( $refresh_timeout > 0 )
			$table->set_attribute( 'data-refresh_timeout', $refresh_timeout );

		if ( $form->is_posting() )
		{
			$form->post();
			$ids = $table->bulk_actions()->get_rows();
			if ( $table->bulk_actions()->pressed() )
				switch ( $table->bulk_actions()->get_action() )
				{
					case 'delete':
						$remove_activities = new actions\remove_activities();
						foreach( $ids as $id )
							$remove_activities->ids->append( $id );
						$remove_activities->execute();
						break;
				}
		}

		// Are we displaying local activities? Then remove the blog id column.
		if ( ! isset( $list_activities->global ) )
			$list_activities->filters_settings->get( 'table_columns' )->forget( 'blog_id' );

		// Retrieve a count first.
		$count_action = clone( $list_activities );
		$count_action->count = true;
		$count_action->execute();
		$count = $count_action->result;

		// And now retrieve the activities themselves.
		$list_activities->per_page = $this->get_site_option( 'per_page');
		$max_pages = floor( $count / $list_activities->per_page );
		$page = ( isset( $_GET[ 'paged' ] ) ? intval( $_GET[ 'paged' ] ) : 1 );
		$list_activities->page = $this->minmax($page, 1, $max_pages);

		$list_activities->execute();
		$activities = $list_activities->result;

		$page_links = paginate_links( [
			'base' => add_query_arg( 'paged', '%#%' ),
			'format' => '',
			'prev_text' => '&laquo;',
			'next_text' => '&raquo;',
			'current' => $page,
			'total' => $max_pages,
		] );

		if ( $page_links )
			$page_links = '<div style="width: 50%; float: right;" class="tablenav"><div class="tablenav-pages">' . $page_links . '</div></div>';

		// Return a list of all of the table columns. We do this in order to get their translated names.
		$get_activity_table_columns = new actions\get_activity_table_columns();
		$get_activity_table_columns->execute();

		// Decide which columns to display.
		$columns_to_display = [];

		foreach( $get_activity_table_columns->columns as $key => $text )
		{
			if ( ! $list_activities->filters_settings->display_table_column( $key ) )
				continue;

			$columns_to_display []= $key;
			$row->th( $key )->text( $text );
		}

		// Display each activity.
		foreach( $activities as $activity )
		{
			$activity->data = (object)$activity->data;

			$row = $table->body()->row();
			$row->css_class( $activity->hook );

			$table->bulk_actions()->cb( $row, $activity->id );

			foreach( $columns_to_display as $column_id )
			{
				$action = new actions\display_activity_table_column();
				$action->column = $column_id;
				$action->row = $row;
				$action->activity = $activity;
				$action->execute();
			}
		}

		$r .= $page_links;
		$r .= $form->open_tag();

		$r .= $table;
		$r .= $form->close_tag();
		$r .= $page_links;

		$r .= $this->get_overview_ajax( $list_activities->filters_settings );

		$r .= '</div>';		// pvam_overview_table

		echo $r;
	}

	/**
		@brief		Allow the user to filter out activities.
		@since		2014-05-04 09:55:14
	**/
	public function menu_activity_filters()
	{
		$r = $this->p_( 'The settings below will display only the activities that match the filter settings. The settings are saved per-user in the Wordpress user meta table.' );

		$filters_settings = filters_settings::load();

		$r = '';

		$form = $this->form2();
		$form->css_class( 'activity_filters' );
		$form->css_class( 'plainview_form_auto_tabs' );

		$show_filter_options = new actions\add_filter_settings();
		$show_filter_options->filters_settings = $filters_settings;
		$show_filter_options->form = $form;
		$show_filter_options->execute();

		// Save!
		$form->primary_button( 'save' )
			// Save settings button
			->value_( 'Save your filter settings' );

		$form->secondary_button( 'delete' )
			// Clear settings button
			->value_( 'Clear your filter settings' );

		if ( $form->is_posting() )
		{
			$form->post();
			$form->use_post_values();

			if ( ! $form->validates() )
			{
				foreach( $form->get_validation_errors() as $error )
					$this->error( $error );
			}
			else
			{
				if ( $form->input( 'delete' )->pressed() )
				{
					filters_settings::delete();
					$this->message_( 'Your filter settings have been cleared. Revisit the page to see that the filters are cleared.' );
				}

				if ( $form->input( 'save' )->pressed() )
				{
					$show_filter_options = new actions\save_filter_settings();
					$show_filter_options->filters_settings = $filters_settings;
					$show_filter_options->form = $form;
					$show_filter_options->execute();

					$filters_settings->save();
					$this->message_( 'Your filter settings have been saved.' );
				}
			}
		}

		$r .= $form->open_tag();
		$r .= $form->display_form_table();
		$r .= $form->close_tag();

		echo $r;
	}

	/**
		@brief		Show the global activity.
		@since		2014-04-27 14:15:14
	**/
	public function menu_activity_global()
	{
		$list_activities = new actions\list_activities();
		$list_activities->filters_settings = filters_settings::load();
		$list_activities->global = true;
		echo $this->menu_activities_display( $list_activities );
	}

	/**
		@brief		Show the local activity.
		@since		2014-04-27 14:15:14
	**/
	public function menu_activity_local()
	{
		$list_activities = new actions\list_activities();

		// Add the current blog to the blogs to include
		$fs = filters_settings::load();
		$array = $fs->get_filter( 'blog_ids_to_include', [] );
		$array []= get_current_blog_id();
		$fs->set_filter( 'blog_ids_to_include', $array );
		$list_activities->filters_settings = $fs;

		echo $this->menu_activities_display( $list_activities );
	}

	/**
		@brief		Various tools.
		@since		2014-05-04 10:41:51
	**/
	public function admin_menu_tools()
	{
		$r = '';

		// IP converter
		$form = $this->form2();

		$fs = $form->fieldset( 'fs_ip' );
		$fs->legend->label_( 'IP tools' );

		$fs->text( 'ip' )
			->label_( 'IP or integer' )
			->required()
			->size( 15, 15 );

		$fs->markup( 'markup_convert' )
			->markup( 'The convert button will convert the IP address or integer to its equivalent integer or IP address.' );

		$fs->secondary_button( 'convert' )
			->value( 'Convert' );

		$fs->markup( 'markup_lookup' )
			->markup( 'The lookup button will try to resolve an IP address to a host name. If dig is installed on the webserver it will also be used for the lookup.' );

		$fs->secondary_button( 'lookup' )
			->value( 'Lookup' );

		if ( $form->is_posting() )
		{
			$form->post()->use_post_value();

			$ip = $fs->input( 'ip' )->get_filtered_post_value();
			$long = $ip;
			$is_ip = ( strpos( $ip, '.' ) !== false  );
			if ( $is_ip )
				$long = ip2long( $ip );
			else
				$ip = long2ip( $ip );

			if ( $fs->input( 'convert' )->pressed() )
			{
				if ( $is_ip )
					$message = $this->p_( 'The integer value of this IP address %s is <strong>%s</strong>.', $ip, $long );
				else
					$message = $this->p_( 'The IP address of the integer %s is <strong>%s</strong>.', $long, $ip );
			}

			if ( $fs->input( 'lookup' )->pressed() )
			{
				$address = gethostbyaddr( $ip );
				$message = $this->p_( 'The IP address %s resolves to <strong>%s</strong>.', $ip, $address );

				$output = '';
				exec( 'dig -x ' . $ip, $output );
				if ( count( $output ) > 0 )
				{
					$output = array_filter( $output );
					$output = implode( "\n", $output );
					$message .= $this->p_( 'Output from dig: %s', $this->p( $output ) );
				}
			}

			$this->message( $message );
		}

		$r .= $form->open_tag();
		$r .= $form->display_form_table();
		$r .= $form->close_tag();

		echo $r;
	}

	/**
		@brief		Add our filter options to the form.
		@since		2015-11-07 00:03:36
	**/
	public function plainview_activity_monitor_add_filter_settings( $action )
	{
		$this->__list_distinct_values = new actions\list_distinct_values();
		$this->__list_distinct_values->execute();

		if ( $this->is_network )
		{
			// BLOGS

			$fs = $action->form->fieldset( 'fs_blog_ids' );
			$fs->legend->label_( 'Blogs' );

			$blog_options = [];
			foreach( $this->__list_distinct_values->as_options( 'blog_ids' ) as $blog_id )
			{
				if ( $this->is_network )
					switch_to_blog( $blog_id );
				$blog_name = get_bloginfo( 'blogname' );
				if ( ! $blog_name )
					$blog_name = $blog_id . ' (deleted)';
				else
				{
					if ( $this->is_network )
						restore_current_blog();
				}
				$blog_options[ $blog_name ] = $blog_id;
			}
			ksort( $blog_options );

			$fs->select( 'blog_ids_to_exclude' )
				->description_( 'Do not display the selected blogs in the global view.' )
				->label_( 'Hide blogs' )
				->multiple()
				->options( $blog_options )
				->size( 10 )
				->value( $action->filters_settings->get_filter( 'blog_ids_to_exclude' ) );

			$fs->select( 'blog_ids_to_include' )
				->description_( 'Display the selected blogs in the global view.' )
				->label_( 'Display blogs' )
				->multiple()
				->options( $blog_options )
				->size( 10 )
				->value( $action->filters_settings->get_filter( 'blog_ids_to_include' ) );
		}

		// HOOKS

		$fs = $action->form->fieldset( 'fs_hooks' );
		$fs->legend->label_( 'Hooks' );

		$fs->select( 'hooks_to_exclude' )
			->description_( 'Do not display the selected hooks.' )
			->label_( 'Hide hooks' )
			->multiple()
			->size( 10 )
			->value( $action->filters_settings->get_filter( 'hooks_to_exclude' ) );

		$fs->select( 'hooks_to_include' )
			->description_( 'Display the selected hooks.' )
			->label_( 'Display hooks' )
			->multiple()
			->size( 10 )
			->value( $action->filters_settings->get_filter( 'hooks_to_include' ) );

		foreach( [ 'hooks_to_exclude', 'hooks_to_include' ] as $key )
			$this->grouped_hooks_to_select( [
				'input' => $action->form->input( $key ),
				'list_distinct_values' => $this->__list_distinct_values,
			] );

		// IPS

		$fs = $action->form->fieldset( 'fs_ips' );
		$fs->legend->label_( 'IP addresses' );

		$ips = [];
		foreach( $this->__list_distinct_values->as_options( 'ips' ) as $ip )
			$ips[ long2ip( $ip ) ] = $ip;

		$fs->select( 'ips_to_exclude' )
			->description_( 'Do not display the selected IP addresses.' )
			->label_( 'Hide IPs' )
			->multiple()
			->options( $ips )
			->size( 10 )
			->value( $action->filters_settings->get_filter( 'ips_to_exclude' ) );

		$fs->select( 'ips_to_include' )
			->description_( 'Display the selected IP addresses.' )
			->label_( 'Display IPs' )
			->multiple()
			->options( $ips )
			->size( 10 )
			->value( $action->filters_settings->get_filter( 'ips_to_include' ) );

		// LIMIT

		$fs = $action->form->fieldset( 'fs_limit' );
		$fs->legend->label_( 'Limit' );

		$fs->number( 'limit' )
			// Limit input desc
			->description_( 'Maximum number of activities to return. 0 for no limit.' )
			// Limit input label
			->label_( 'Limit' )
			->min( 0 )
			->size( 5 )
			->value( $action->filters_settings->get_filter( 'limit' ) );

		// Table columns

		$fs = $action->form->fieldset( 'fs_table_columns' );
		$fs->legend->label_( 'Table columns' );

		$get_table_columns = new actions\get_activity_table_columns();
		$get_table_columns->execute();
		$columns = array_flip( $get_table_columns->columns->to_array() );

		$fs->select( 'table_columns' )
			->description( 'Which columns in the activities table do you wish to see?' )
			->label( 'Visible columns' )
			->options( $columns )
			->multiple()
			->value( $action->filters_settings->get_table_columns() )
			->autosize();

		// TEXT SEARCH

		$fs = $action->form->fieldset( 'fs_text_search' );
		$fs->legend->label_( 'Text' );

		$fs->markup( 'm_text_search' )
			->p_( 'Perform a text search on the activity description. Choose to either include or exclude the matching activities. One string per line. Strings are not case sensitive.' );

		$fs->textarea( 'texts_to_include' )
			->cols( 40, 5 )
			->description( $this->_( 'Display only activities that match the above strings.' ) )
			->label( $this->_( 'Strings to include' ) );

		$fs->textarea( 'texts_to_exclude' )
			->cols( 40, 5 )
			->description( $this->_( 'Hide activities that contain any of the above strings.' ) )
			->label( $this->_( 'Strings to exclude' ) );

		foreach( [ 'texts_to_include', 'texts_to_exclude' ] as $key )
		{
			$value = $action->filters_settings->get_filter( $key, [] );
			$value = implode( "\n", $value );
			$fs->input( $key )
				->value( $value );
		}

		// TIME PERIOD
		$fs = $action->form->fieldset( 'fs_time' );
		$fs->legend->label_( 'Time' );

		$fs->number( 'time_days_maximum' )
			// Maximum days ago input desc
			->description_( 'Maximum age for the activities to be shown.' )
			// Maximum days input label
			->label_( 'Maximum days' )
			->min( 0 )
			->size( 5 )
			->value( $action->filters_settings->get_filter( 'time_days_maximum' ) );

		$fs->number( 'time_days_minimum' )
			// Minimum days input desc
			->description_( 'Minimum age for the activities to be shown.' )
			// Minimum days input label
			->label_( 'Minimum days' )
			->min( 0 )
			->size( 5 )
			->value( $action->filters_settings->get_filter( 'time_days_minimum' ) );

		// USERS

		$user_ids = [];
		foreach( $this->__list_distinct_values->as_options( 'user_ids' ) as $user_id )
		{
			if ( $user_id > 0 )
			{
				$user_info = get_userdata( $user_id );
				if ( ! $user_info )
					$user_ids[ $user_id . ' (deleted)' ] = $user_id;
				else
					$user_ids[ $user_info->data->user_login ] = $user_id;
			}
			else
			{
				$user_ids[ 'Guest' ] = 0;
			}
		}
		ksort( $user_ids );

		$fs = $action->form->fieldset( 'fs_user_ids' );
		$fs->legend->label_( 'Users' );

		$fs->select( 'user_ids_to_exclude' )
			->description_( 'Do not display the selected users.' )
			->label_( 'Hide users' )
			->multiple()
			->options( $user_ids )
			->size( 10 )
			->value( $action->filters_settings->get_filter( 'user_ids_to_exclude' ) );

		$fs->select( 'user_ids_to_include' )
			->description_( 'Display the selected users.' )
			->label_( 'Display users' )
			->multiple()
			->options( $user_ids )
			->size( 10 )
			->value( $action->filters_settings->get_filter( 'user_ids_to_include' ) );
	}

	/**
		@brief		Save the form inputs to the filters settings.
		@since		2015-11-07 00:03:36
	**/
	public function plainview_activity_monitor_save_filter_settings( $action )
	{
		if ( $action->is_finished() )
			return;

		$keys = [
			'hook',
			'ip',
			'user_id',
		];

		if ( $this->is_network )
			$keys []= 'blog_id';

		foreach( [ 'include', 'exclude' ] as $inc )
			foreach( $keys as $key )
			{
				$key = $key . 's_to_' . $inc;
				$values = $action->form->input( $key )->get_post_value();
				if ( count( $values ) > 0 )
					$action->filters_settings->set_filter( $key, $values );
				else
					$action->filters_settings->clear_filter( $key );
			}

		$table_columns = $action->form->input( 'table_columns' )->get_post_value();
		$action->filters_settings->set_table_columns( $table_columns );

		// Ints
		foreach( [
			'limit',
			'time_days_maximum',
			'time_days_minimum',
		] as $key )
		{
			$value = $action->form->input( $key )->get_filtered_post_value();
			if ( $value != 0 )
				$action->filters_settings->set_filter( $key, $value );
			else
				$action->filters_settings->clear_filter( $key );
		}

		foreach( [
			'texts_to_exclude',
			'texts_to_include',
		] as $key )
		{
			$value = $action->form->input( $key )->get_post_value();
			$value = trim( $value );
			$value = Plainview_Activity_Monitor()->textarea_to_array( $value );
			if ( count( $value ) > 0 )
				$action->filters_settings->set_filter( $key, $value );
			else
				$action->filters_settings->clear_filter( $key );
		}
	}
}
