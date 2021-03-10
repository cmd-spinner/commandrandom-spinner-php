<?php
register_module([
	"name" => "Redirect pages",
	"version" => "0.3.1",
	"author" => "Starbeamrainbowlabs",
	"description" => "Adds support for redirect pages. Uses the same syntax that Mediawiki does.",
	"id" => "feature-redirect",
	"code" => function() {
		global $settings;
		
		register_save_preprocessor(function(&$index_entry, &$pagedata) {
			$matches = [];
			if(preg_match("/^# ?REDIRECT ?\[\[([^\]]+)\]\]/i", $pagedata, $matches) === 1)
			{
				//error_log("matches: " . var_export($matches, true));
				// We have found a redirect page!
				// Update the metadata to reflect this.
				$index_entry->redirect = true;
				$index_entry->redirect_target = $matches[1];
			}
			else
			{
				// This page isn't a redirect. Unset the metadata just in case.
				if(isset($index_entry->redirect))
					unset($index_entry->redirect);
				if(isset($index_entry->redirect_target))
					unset($index_entry->redirect_target);
			}
		});
		
		// Register a help section
		add_help_section("25-redirect", "Redirect Pages", "<p>$settings->sitename supports redirect pages. To create a redirect page, enter something like <code># REDIRECT [[pagename]]</code> on the first line of the redirect page's content. This <em>must</em> appear as the first line of the page, with no whitespace before it. You can include content beneath the redirect if you want, too (such as a reason for redirecting the page).</p>");
	}
]);

?>
