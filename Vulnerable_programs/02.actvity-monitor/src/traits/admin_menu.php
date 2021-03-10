<?php

namespace plainview\wordpress\activity_monitor\traits;

use \plainview\wordpress\activity_monitor\actions;
use \plainview\wordpress\activity_monitor\database\filters_settings;

trait admin_menu
{
	public function admin_menu()
	{
		if ( $this->is_network )
		{
			if ( ! is_super_admin() )
				return;
		}
		else
		{
			if ( ! $this->user_has_roles( 'administrator' ) )
				return;
		}

		$this->load_language();

		wp_enqueue_script( 'plainview_activity_monitor',
			sprintf( '%s/js/js.js', $this->paths( 'url' ) ),
			[ 'jquery' ],
			PLAINVIEW_ACTIVITY_MONITOR_VERSION
		);

		$icon_base64 = file_get_contents( __DIR__ . '../../../css/icon.svg' );
		$icon_base64 = base64_encode( $icon_base64 );

		$menu_page = $this->menu_page()
			->callback_this( 'admin_menu_tabs' )
			->icon_url( 'data:image/svg+xml;base64,' . $icon_base64 )		// Allow the css to override
			->menu_title( $this->_( 'Activity monitor' ) )
			->menu_slug( 'plainview_activity_monitor' )
			->page_title( $this->_( 'Activity monitor' ) );

		if ( ! defined( 'PLAINVIEW_ACTIVITY_MONITOR_PLUGIN_PACK_VERSION' ) )
			$menu_page
			->submenu( 'activity_monitor_premium_pack' )
			->callback_this( 'admin_menu_premium_pack_info' )
			->menu_title( $this->_( 'Premium Pack' ) )
			->page_title( $this->_( 'Activity Monitor Premium Pack Information' ) );

		$action = new \plainview\wordpress\activity_monitor\actions\admin_menu;
		$action->menu_page = $menu_page;
		$action->execute();

		$menu_page->add_all();
	}

	/**
		@brief		Select which hooks are to be logged.
		@since		2014-05-03 13:46:40
	**/
	public function admin_menu_hooks()
	{
		$form = $this->form2();
		$r = $this->p_( 'Select the actions you wish to store in the database. All the below actions, whether they are stored or not, are detectable and usable by Activity Monitor plugins.' );

		$action = new \plainview\wordpress\activity_monitor\actions\manifest_hooks;
		$action->execute();
		$hooks = $action->hooks;

		$logged_hooks = new actions\get_logged_hooks();
		$logged_hooks = $logged_hooks->execute()->logged_hooks;

		$table = $this->table();
		$table->css_class( 'hooks' );
		$table->css_class( 'pvam plugins with_groups' );		// We want to use the coloring from the plugins table.
		$row = $table->head()->row();
		$table->bulk_actions()
			->form( $form )
			->add( $this->_( 'Activate' ), 'activate' )
			->add( $this->_( 'Deactivate' ), 'deactivate' )
			->cb( $row );
		$row->th( 'hook' )->text_( 'Hook' );
		$row->th( 'vendor' )->text_( 'Vendor' );
		$row->th( 'description' )->text_( 'Description' );
		if ( $this->debugging() )
			$row->th( 'debug' )->text_( 'Debug info' );

		if ( $form->is_posting() )
		{
			$form->post();

			if ( $table->bulk_actions()->pressed() )
			{
				$ids = $table->bulk_actions()->get_rows();

				// Add all of the current hooks.
				switch ( $table->bulk_actions()->get_action() )
				{
					case 'activate':
						// Find the hook with these names
						foreach( $ids as $id )
							$logged_hooks[ $id ] = $id;
					break;
					case 'deactivate':
						// Find the hook with these names
						foreach( $ids as $id )
							unset( $logged_hooks[ $id ] );
					break;
				}

				// Prune non-existent hooks off of the list of logged hooks.
				$modified = false;
				foreach( $logged_hooks as $id )
				{
					$found = false;
					foreach( $hooks as $a_hook )
					{
						if ( $a_hook->get_id() == $id )
						{
							$found = true;
							break;
						}
					}
					if ( ! $found )
						unset( $logged_hooks[ $id ] );
				}

				$this->update_site_option( 'logged_hooks', $logged_hooks );
			}

			$this->message_( 'The list of logged hooks has been saved.' );
		}

		// Hooks are sorted into categories, but the grouping JS uses "groups" instead. No biggie.
		$old_category = 'xxx';
		foreach( $action->by_category() as $hook )
		{
			if ( $hook->get_category() != $old_category )
			{
				$old_category = $hook->get_category();

				// Sanitize the category name.
				$category_slug = sanitize_title( $old_category );

				$row = $table->body()->row()
					->css_class( 'group inactive' )
					->data( 'group', $category_slug );

				$row->th()
					->colspan( 5 )
					->text( str_replace( '_', ' ', $old_category ) );	// Remove the underscore.
			}

			$row = $table->body()->row()
				->css_class( 'plugin' )
				->data( 'group', $category_slug );

			$bulk_id = $hook->get_id();
			$cb = $table->bulk_actions()->cb( $row, $bulk_id );

			$td = $row->td( 'hook' );

			$td->css_class( 'plugin-title' )
				->title( $hook->get_class() );

			// Assemble a label.
			$label = new \plainview\sdk_pvam\html\div();
			$label->tag = 'label';
			$label->set_attribute( 'for', $cb->get_id() );
			$label->content = $hook->get_hook();

			$td->text( $label );

			if ( isset( $logged_hooks[ $hook->get_id() ]  ) )
				$row->css_class( 'active' );
			else
				$row->css_class( 'inactive' );

			$row->td( 'vendor' )->text( $hook->get_vendor() );
			$row->td( 'description' )->text( $hook->get_description() );

			if ( $this->debugging() )
			{
				$debug = [];
				$debug[ 'Priority' ] = $hook->get_priority();
				$debug[ 'Parameter count' ] = $hook->get_parameter_count();
				$text = [];
				foreach( $debug as $key => $value )
					$text[] = sprintf( '<em>%s</em> %s', $key, $value );
				$row->td( 'debug' )->text( implode( '<br/>', $text ) );
			}
		}

		$r .= $form->open_tag();
		$r .= $table;
		$r .= $form->display_form_table();
		$r .= $form->close_tag();

		$this->debug( 'The logged_hooks setting is: %s', $logged_hooks );

		echo $r;
	}

	/**
		@brief		Mass delete activities.
		@since		2015-12-07 11:35:14
	**/
	public function admin_menu_mass_delete()
	{
		$form = $this->form2();
		$form->css_class( 'plainview_form_auto_tabs' );

		$r = $this->p_( 'This tool can be used to mass delete activites from the database. Use the filters to select the activities to be deleted.' );
		$filters_settings = new filters_settings();
		$show_filter_options = new actions\add_filter_settings();
		$show_filter_options->filters_settings = $filters_settings;
		$show_filter_options->form = $form;
		$show_filter_options->execute();

		$form->primary_button( 'delete_activities' )
			// Button to delete activities
			->value_( 'Delete selected activities' );

		if ( $form->is_posting() )
		{
			$form->post()
				->use_post_values();

			$filters_settings = new filters_settings();
			$save_filter_options = new actions\save_filter_settings();
			$save_filter_options->filters_settings = $filters_settings;
			$save_filter_options->form = $form;
			$save_filter_options->execute();

			// If the user did not specify a limit, set it to 100 in order to prevent getting too much info back.
			if ( $filters_settings->get_filter( 'limit', 0 ) < 1 )
				$filters_settings->set_filter( 'limit', 100 );

			// Use these filter settings to keep retrieving activities until there are no more to delete.
			while ( true )
			{
				$list_activities = new actions\list_activities();
				$list_activities->filters_settings = $filters_settings;
				$list_activities->execute();

				if ( count( $list_activities->result ) < 1 )
					break;

				$remove_activities = new actions\remove_activities();

				foreach( $list_activities->result as $activity )
					$remove_activities->ids->append( $activity->id );

				$remove_activities->execute();
			}

			$this->message_( 'The selected activities have been deleted.' );
		}

		$r .= $form->open_tag();
		$r .= $form->display_form_table();
		$r .= $form->close_tag();
		echo $r;
	}

	/**
		@brief		Display the settings tab.
		@since		2014-04-27 14:50:35
	**/
	public function admin_menu_settings()
	{
		$form = $this->form2();
		$form->css_class( 'plainview_form_auto_tabs' );
		$r = '';

		// DATABASE-------------------------------------------------------------

		$fs = $form->fieldset( 'fs_activities' );
		$fs->legend->label_( 'Activities' );

		// Get an activity count.
		$list_activities = new actions\list_activities;
		$list_activities->filters_settings = filters_settings::load();
		$list_activities->count = true;
		$list_activities->execute();

		$fs->number( 'activities_in_database' )
			->description_( 'How many activities to store in the database. There are currently %s activities stored.', $list_activities->result )
			->label_( 'Activities in database' )
			->min( 1 )
			->required()
			->size( 5 )
			->value( $this->get_site_option( 'activities_in_database' ) );

		$fs->markup( 'mu_activities_in_database' )
			->p_( 'This value is dependent on how much activity there is on your blog and how much history you wish to keep. Updating this value cleans the database. There is also a 5% chance that the database will be cleaned up when a new activity is logged.' );

		// DEBUG----------------------------------------------------------------

		$this->add_debug_settings_to_form( $form );

		// DISPLAY--------------------------------------------------------------

		$fs = $form->fieldset( 'fs_display' );
		$fs->legend->label_( 'Display' );

		$fs->number( 'refresh_timeout' )
			->description_( 'How many minutes to wait before periodically refreshing the activities overview table. Set to 0 to disable.' )
			->label_( 'Autorefresh' )
			->min( 0 )
			->required()
			->size( 5 )
			->value( $this->get_site_option( 'refresh_timeout' ) );

		$fs->number( 'per_page' )
			->description_( 'How many items to show per table page.' )
			->label_( 'Items per page' )
			->min( 1, 10000 )
			->required()
			->size( 5, 5 )
			->value( $this->get_site_option( 'per_page' ) );

		$fs->checkbox( 'wp_login_failed_password' )
			->checked( $this->get_site_option( 'wp_login_failed_password' ) )
			->description_( "Should the attempted password be shown in the wp_login_failed activity? If you're security conscious, uncheck this box." )
			->label_( 'Log failed passwords?' );

		// ---------------------------------------------------------------------

		$form->primary_button( 'save' )
			->value_( 'Save settings' );

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
				$this->save_debug_settings_from_form( $form );
				$this->update_site_option( 'per_page', $form->input( 'per_page' )->get_filtered_post_value() );
				$activities_in_database = $form->input( 'activities_in_database' )->get_filtered_post_value();
				$this->update_site_option( 'activities_in_database', $activities_in_database );
				$this->update_site_option( 'refresh_timeout', $form->input( 'refresh_timeout' )->get_filtered_post_value() );
				$this->update_site_option( 'wp_login_failed_password', intval( $form->input( 'wp_login_failed_password' )->is_checked() ) );	// intval because otherwise it returns the default, which is true.

				$prune_activities = new actions\prune_activities;
				$prune_activities->count = $activities_in_database;
				$prune_activities->execute();

				$this->message_( 'The settings have been saved.' );

				$_POST = [];
				echo $this->admin_menu_settings();
				return;
			}
		}

		$r .= $form->open_tag();
		$r .= $form->display_form_table();
		$r .= $form->close_tag();

		echo $r;
	}

	/**
		@brief		Display info about the plugin pack.
		@since		2016-02-03 22:05:53
	**/
	public function admin_menu_premium_pack_info()
	{
		$r = file_get_contents( __DIR__ . '../../../html/plugin_pack_info.html' );
		echo $r;
	}

	public function admin_menu_tabs()
	{
		$this->load_language();

		if ( $this->is_network )
			$show_admin_tabs = is_super_admin();
		else
			$show_admin_tabs = $this->user_has_roles( 'administrator' );

		$tabs = $this->tabs();

		// Only admin on a network is allowed to see these activities.
		if ( $this->is_network && $show_admin_tabs )
			$tabs->tab( 'activity_global' )
				->callback_this( 'menu_activity_global' )
				->heading_( 'Activity on all blogs' )
				->name_( 'Global activity' )
				->sort_order( 20 );	// Global always before local.

		// Don't display local activity on the network admin.
		if ( ( $this->is_network AND ! is_network_admin() )
			OR
			( ! $this->is_network AND $show_admin_tabs )
			)
			$tabs->tab( 'activity_local' )
				->callback_this( 'menu_activity_local' )
				->heading_( 'Activity on %s', get_bloginfo( 'blogname' ) )
				->name_( 'Local activity' )
				->sort_order( 30 );

		$tabs->tab( 'activity_filters' )
			->callback_this( 'menu_activity_filters' )
			// Tab name
			->name_( 'Filters' );

		if ( $show_admin_tabs )
			$tabs->tab( 'hooks' )
				->callback_this( 'admin_menu_hooks' )
				->heading_( 'Logged hooks' )
				// Tab name
				->name_( 'Logged hooks' );

		if ( $show_admin_tabs )
			$tabs->tab( 'mass_delete' )
				->callback_this( 'admin_menu_mass_delete' )
				// Page heading
				->heading_( 'Mass delete activites' )
				// Tab name
				->name_( 'Mass delete' );

		if ( $show_admin_tabs )
			$tabs->tab( 'settings' )
				->callback_this( 'admin_menu_settings' )
				// Tab name
				->name_( 'Settings' );

		$tabs->tab( 'activity_tools' )
			->callback_this( 'admin_menu_tools' )
			->name_( 'Tools' );

		if ( $show_admin_tabs )
			$tabs->tab( 'uninstall' )
				->callback_this( 'admin_uninstall' )
				// Tab name
				->name_( 'Uninstall' )
				->sort_order( 90 );		// Uninstall is always last.

		// Allow other plugins to add to our tabs.
		$action = new \plainview\wordpress\activity_monitor\actions\admin_menu_tabs;
		$action->tabs = $tabs;
		$action->execute();

		echo $tabs;
	}
}
