<?php
register_module([
	"name" => "Page list",
	"version" => "0.11",
	"author" => "Starbeamrainbowlabs",
	"description" => "Adds a page that lists all the pages in the index along with their metadata.",
	"id" => "page-list",
	"code" => function() {
		global $settings;
		
		/**
		 * @api		{get}	?action=list	List all pages 
		 * @apiDescription	Gets a list of all the pages currently stored on the wiki.
		 * @apiName		ListPages
		 * @apiGroup	Page
		 * @apiPermission	Anonymous
		 */
		
		/*
		 * ██      ██ ███████ ████████ 
		 * ██      ██ ██         ██    
		 * ██      ██ ███████    ██    
		 * ██      ██      ██    ██    
		 * ███████ ██ ███████    ██    
		 */
		add_action("list", function() {
			global $pageindex, $settings;
			
			$supported_formats = [ "html", "json", "text" ];
			$format = $_GET["format"] ?? "html";
			
			$sorted_pageindex = get_object_vars($pageindex);
			ksort($sorted_pageindex, SORT_NATURAL);
			
			switch($format) {
				case "html":
					$title = "All Pages";
					$content = "	<h1>$title on $settings->sitename</h1>";
					
					$content .= generate_page_list(array_keys($sorted_pageindex));
					exit(page_renderer::render_main("$title - $settings->sitename", $content));
					break;
					
				case "json":
					header("content-type: application/json");
					exit(json_encode(array_keys($sorted_pageindex), JSON_PRETTY_PRINT));
				
				case "text":
					header("content-type: text/plain");
					exit(implode("\n", array_keys($sorted_pageindex)));
				
				default:
					http_response_code(400);
					exit(page_renderer::render_main("Format error - $settings->sitename", "<p>Error: The format '$format' is not currently supported by this action on $settings->sitename. Supported formats: " . implode(", ", $supported_formats) . "."));
			}
			
		});
		
		/**
		 * @api		{get}	?action=list-tags[&tag=]	Get a list of tags or pages with a certain tag
		 * @apiDescription	Gets a list of all tags on the wiki. Adding the `tag` parameter causes a list of pages with the given tag to be returned instead.
		 * @apiName		ListTags
		 * @apiGroup	Utility
		 * @apiPermission	Anonymous
		 * 
		 * @apiParam	{string}	tag		Optional. If provided a list of all the pages with that tag is returned instead.
		 * @apiParam	{string}	format	Optional. If specified sets the format of the returned result. Supported values: html, json. Default: html
		 */
		
		/*
		 * ██      ██ ███████ ████████ ████████  █████   ██████  ███████ 
		 * ██      ██ ██         ██       ██    ██   ██ ██       ██      
		 * ██      ██ ███████    ██ █████ ██    ███████ ██   ███ ███████ 
		 * ██      ██      ██    ██       ██    ██   ██ ██    ██      ██ 
		 * ███████ ██ ███████    ██       ██    ██   ██  ██████  ███████ 
		 */
		add_action("list-tags", function() {
			global $pageindex, $settings;
			
			$supported_formats = [ "html", "json", "text" ];
			$format = $_GET["format"] ?? "html";
			
			if(!in_array($format, $supported_formats)) {
				http_response_code(400);
				exit(page_renderer::render_main("Format error - $settings->sitename", "<p>Error: The format '$format' is not currently supported by this action on $settings->sitename. Supported formats: " . implode(", ", $supported_formats) . "."));
			}
			
			if(!isset($_GET["tag"]))
			{
				// Render a list of all tags
				$all_tags = get_all_tags();
				
				sort($all_tags, SORT_NATURAL);
				
				switch($format) {
					case "html":
						$content = "<h1>All tags</h1>
						<ul class='tag-list'>\n";
						foreach($all_tags as $tag)
						{
							$content .= "			<li><a href='?action=list-tags&tag=" . rawurlencode($tag) . "' class='mini-tag'>$tag</a></li>\n";
						}
						$content .= "</ul>\n";
						
						exit(page_renderer::render("All tags - $settings->sitename", $content));
						break;
					
					case "json":
						header("content-type: application/json");
						exit(json_encode($all_tags, JSON_PRETTY_PRINT));
					
					case "text":
						header("content-type: text/plain");
						exit(implode("\n", $all_tags));
				}
			}
			$tag = $_GET["tag"];
			
			
			$sorted_pageindex = get_object_vars($pageindex);
			ksort($sorted_pageindex, SORT_NATURAL);
			
			$pagelist = [];
			foreach($pageindex as $pagename => $pagedetails)
			{
				if(empty($pagedetails->tags)) continue;
				if(in_array($tag, $pagedetails->tags))
					$pagelist[] = $pagename;
			}
			
			switch($format)
			{
				case "html":
					$content = "<h1>Tag List: $tag</h1>\n";
					$content .= generate_page_list($pagelist);
					
					$content .= "<p>(<a href='?action=list-tags'>All tags</a>)</p>\n";
					
					exit(page_renderer::render("$tag - Tag List - $settings->sitename", $content));
				
				case "json":
					header("content-type: application/json");
					exit(json_encode($pagelist, JSON_PRETTY_PRINT));
				
				case "text":
					header("content-type: text/plain");
					exit(implode("\n", $pagelist));
			}
			
		});
		
		statistic_add([
			"id" => "tag-count",
			"name" => "Number of Tags",
			"type" => "scalar",
			"update" => function($old_data) {
				global $pageindex;
				
				$result = new stdClass(); // value, state, completed
				$result->value = count(get_all_tags());
				$result->completed = true;
				return $result;
			}
		]);
		statistic_add([
			"id" => "tags-per-page",
			"name" => "Average Number of Tags per Page",
			"type" => "scalar",
			"update" => function($old_data) {
				global $pageindex;
				$tag_counts = [];
				foreach($pageindex as $page_entry)
					$tag_counts[] = count($page_entry->tags ?? []);
				
				$result = new stdClass(); // value, state, completed
				$result->value = round(array_sum($tag_counts) / count($tag_counts), 3);
				$result->completed = true;
				return $result;
			}
		]);
		statistic_add([
			"id" => "most-tags",
			"name" => "Most tags on a single page",
			"type" => "scalar",
			"update" => function($old_data) {
				global $pageindex;
				
				$highest_tag_count = 0;
				$highest_tag_page = "";
				foreach($pageindex as $pagename => $page_entry) {
					if(count($page_entry->tags ?? []) > $highest_tag_count) {
						$highest_tag_count = count($page_entry->tags ?? []);
						$highest_tag_page = $pagename;
					}
				}
				
				$result = new stdClass(); // value, state, completed
				$result->value = "$highest_tag_count (<a href='?page=" . rawurlencode($highest_tag_page) . "'>" . htmlentities($highest_tag_page) . "</a>)";
				$result->completed = true;
				return $result;
			}
		]);
		statistic_add([
			"id" => "untagged-pages",
			"name" => "Untagged Pages",
			"type" => "page-list",
			"update" => function($old_data) {
				global $pageindex;
				
				$untagged_pages = [];
				foreach($pageindex as $pagename => $page_entry) {
					if(empty($page_entry->tags) || count($page_entry->tags ?? []) == 0)
						$untagged_pages[] = $pagename;
				}
				
				sort($untagged_pages, SORT_STRING | SORT_FLAG_CASE);
				
				$result = new stdClass(); // value, state, completed
				$result->value = $untagged_pages;
				$result->completed = true;
				return $result;
			}
		]);
		
		add_help_section("30-all-pages-tags", "Listing pages and tags", "<p>All the pages and tags on $settings->sitename are listed on a pair of pages to aid navigation. The list of all pages on $settings->sitename can be found by clicking &quot;All Pages&quot; on the top bar. The list of all the tags currently in use can be found by clicking &quot;All Tags&quot; in the &quot;More...&quot; menu in the top right.</p>
		<p>Each tag on either page can be clicked, and leads to a list of all pages that possess that particular tag.</p>
		<p>Redirect pages are shown in italics. A page's last known editor is also shown next to each entry on a list of pages, along with the last known size (which should correct, unless it was changed outside of $settings->sitename) and the time since the last modification (hovering over this will show the exact time that the last modification was made in a tooltip).</p>");
	}
]);

/**
 * Gets a list of all the tags currently used across the wiki.
 * @package	page-list
 * @since	v0.15
 * @return	string[]	A list of all unique tags present on all pages across the wiki.
 */
function get_all_tags()
{
	global $pageindex;
	
	$all_tags = [];
	foreach($pageindex as $page_entry) {
		if(empty($page_entry->tags))
			continue;
			
		foreach($page_entry->tags as $tag) {
			if(!in_array($tag, $all_tags))
				$all_tags[] = $tag;
		}
	}
	return $all_tags;
}

/**
 * Renders a list of pages as HTML.
 * @package	page-list
 * @param	string[]	$pagelist	A list of page names to include in the list.
 * @return	string					The specified list of pages as HTML.
 */
function generate_page_list($pagelist)
{
	global $pageindex;
	// ✎ &#9998; 🕒 &#128338;
	$result = "<ul class='page-list'>\n";
	foreach($pagelist as $pagename)
	{
		// Construct a list of tags that are attached to this page ready for display
		$tags = "";
		// Make sure that this page does actually have some tags first
		if(isset($pageindex->$pagename->tags))
		{
			foreach($pageindex->$pagename->tags as $tag)
			{
				$tags .= "<a href='?action=list-tags&tag=" . rawurlencode($tag) . "' class='mini-tag'>$tag</a>, ";
			}
			$tags = substr($tags, 0, -2); // Remove the last ", " from the tag list
		}
		
		$pageDisplayName = $pagename;
		if(isset($pageindex->$pagename) and
			!empty($pageindex->$pagename->redirect))
			$pageDisplayName = "<em>$pageDisplayName</em>";
		
		$result .= "<li><a href='index.php?page=" . rawurlencode($pagename) . "'>$pageDisplayName</a>
		<em class='size'>(" . human_filesize($pageindex->$pagename->size) . ")</em>
		<span class='editor'><span class='texticon cursor-query' title='Last editor'>&#9998;</span> " . $pageindex->$pagename->lasteditor . "</span>
		<time class='cursor-query' title='" . date("l jS \of F Y \a\\t h:ia T", $pageindex->$pagename->lastmodified) . "'>" . human_time_since($pageindex->$pagename->lastmodified) . "</time>
		<span class='tags'>$tags</span></li>";
	}
	$result .= "		</ul>\n";
	
	return $result;
}

?>
