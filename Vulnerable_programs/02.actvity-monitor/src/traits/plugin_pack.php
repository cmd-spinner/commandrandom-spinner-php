<?php

namespace plainview\wordpress\activity_monitor\traits;

trait plugin_pack
{
	public function html_css()
	{
		return file_get_contents( WP_PLUGIN_DIR . $this->paths()->path_from_plugin_directory . '/html/style.css' );
	}

	public function menu_plugin_pack_info()
	{
		$r = '';
		$r .= $this->html_css();
		$contents = file_get_contents( WP_PLUGIN_DIR . $this->paths()->path_from_plugin_directory .  '/html/plugin_pack_info.html' );
		$r .= $this->wrap( $contents, $this->_( 'Plainview Activity Monitor Premium Pack info' ) );
		echo $r;
	}
}
