<?php
register_module([
	"name" => "Parsedown",
	"version" => "0.9.9",
	"author" => "Emanuil Rusev & Starbeamrainbowlabs",
	"description" => "An upgraded (now default!) parser based on Emanuil Rusev's Parsedown Extra PHP library (https://github.com/erusev/parsedown-extra), which is licensed MIT. Please be careful, as this module adds some weight to your installation, and also *requires* write access to the disk on first load.",
	"id" => "parser-parsedown",
	"code" => function() {
		global $settings;
		
		$parser = new PeppermintParsedown();
		$parser->setInternalLinkBase("?page=%s");
		add_parser("parsedown", function($source) use ($parser) {
			global $settings;
			if($settings->clean_raw_html)
				$parser->setMarkupEscaped(true);
			else
				$parser->setMarkupEscaped(false);
			$result = $parser->text($source);
			
			return $result;
		});
		
		// Wanted pages
		statistic_add([
			"id" => "wanted-pages",
			"name" => "Wanted Pages",
			"type" => "page",
			"update" => function($old_stats) {
				global $pageindex, $env;
				
				$result = new stdClass(); // completed, value, state
				$pages = [];
				foreach($pageindex as $pagename => $pagedata) {
					if(!file_exists($env->storage_prefix . $pagedata->filename)) {
						continue;
					}
					$page_content = file_get_contents($env->storage_prefix . $pagedata->filename);
					preg_match_all("/\[\[([^\]]+)\]\]/", $page_content, $linked_pages);
					if(count($linked_pages[1]) === 0)
						continue; // No linked pages here
					foreach($linked_pages[1] as $linked_page) {
						// Strip everything after the | and the #
						if(strpos($linked_page, "|") !== false)
							$linked_page = substr($linked_page, 0, strpos($linked_page, "|"));
						if(strpos($linked_page, "#") !== false)
							$linked_page = substr($linked_page, 0, strpos($linked_page, "#"));
						if(strlen($linked_page) === 0)
							continue;
						// Make sure we try really hard to find this page in the
						// pageindex
						if(!empty($pageindex->{ucfirst($linked_page)}))
							$linked_page = ucfirst($linked_page);
						else if(!empty($pageindex->{ucwords($linked_page)}))
							$linked_page = ucwords($linked_page);
						
						// We're only interested in pages that don't exist
						if(!empty($pageindex->$linked_page)) continue;
						
						if(empty($pages[$linked_page]))
							$pages[$linked_page] = 0;
						$pages[$linked_page]++;
					}
				}
				
				arsort($pages);
				
				$result->value = $pages;
				$result->completed = true;
				return $result;
			},
			"render" => function($stats_data) {
				$result = "<h2>$stats_data->name</h2>\n";
				$result .= "<table class='wanted-pages'>\n";
				$result .= "\t<tr><th>Page Name</th><th>Linking Pages</th></tr>\n";
				foreach($stats_data->value as $pagename => $linking_pages) {
					$result .= "\t<tr><td>$pagename</td><td>$linking_pages</td></tr>\n";
				}
				$result .= "</table>\n";
				return $result;
			}
		]);
		
		add_help_section("20-parser-default", "Editor Syntax",
		"<p>$settings->sitename's editor uses an extended version of <a href='http://parsedown.org/'>Parsedown</a> to render pages, which is a fantastic open source Github flavoured markdown parser. You can find a quick reference guide on Github flavoured markdown <a href='https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet'>here</a> by <a href='https://github.com/adam-p/'>adam-p</a>, or if you prefer a book <a href='https://www.gitbook.com/book/roachhd/master-markdown/details'>Mastering Markdown</a> by KB is a good read, and free too!</p>
		<h3>Tips</h3>
		<ul>
			<li>Put 2 spaces at the end of a line to add a soft line break. Leave a blank line to add a head line break (i.e. a new paragraph).</li>
			<li>You can add an id to a header that you can link to. Put it in curly braces after the heading name like this: <code># Heading Name {#HeadingId}</code>. Then you can link to like like this: <code>[[Page name#HeadingId}]]</code>. You can also link to a heading id on the current page by omitting the page name: <code>[[#HeadingId]]</code>.</li>
		</ul>
		<h3>Extra Syntax</h3>
		<p>$settings->sitename's editor also supports some extra custom syntax, some of which is inspired by <a href='https://mediawiki.org/'>Mediawiki</a>.
		<table>
			<tr><th style='width: 40%'>Type this</th><th style='width: 20%'>To get this</th><th>Comments</th></th>
			<tr><td><code>[[Internal link]]</code></td><td><a href='?page=Internal%20link'>Internal Link</a></td><td>An internal link.</td></tr>
			<tr><td><code>[[Display Text|Internal link]]</code></td><td><a href='?page=Internal%20link'>Display Text</a></td><td>An internal link with some display text.</td></tr>
			<tr><td><code>![Alt text](http://example.com/path/to/image.png | 256x256 | right)</code></td><td><img src='http://example.com/path/to/image.png' alt='Alt text' style='float: right; max-width: 256px; max-height: 256px;' /></td><td>An image floating to the right of the page that fits inside a 256px x 256px box, preserving aspect ratio.</td></tr>
			<tr><td><code>![Alt text](http://example.com/path/to/image.png | 256x256 | caption)</code></td><td><figure><img src='http://example.com/path/to/image.png' alt='Alt text' style='max-width: 256px; max-height: 256px;' /><figcaption>Alt text</figcaption></figure></td><td>An image with a caption that fits inside a 256px x 256px box, preserving aspect ratio. The caption is taken from the alt text.</td></tr>
			<tr><td><code>![Alt text](Files/Cheese.png)</code></td><td><img src='index.php?action=preview&page=Files/Cheese.png' alt='Alt text' style='' /></td><td>An example of the short url syntax for images. Simply enter the page name of an image (or video / audio file), and Pepperminty Wiki will sort out the url for you.</td></tr>
		</table>
		<p>Note that the all image image syntax above can be mixed and matched to your liking. The <code>caption</code> option in particular must come last or next to last.</p>
		<h4>Templating</h4>
		<p>$settings->sitename also supports including one page in another page as a <em>template</em>. The syntax is very similar to that of Mediawiki. For example, <code>{{Announcement banner}}</code> will include the contents of the \"Announcement banner\" page, assuming it exists.</p>
		<p>You can also use variables. Again, the syntax here is very similar to that of Mediawiki - they can be referenced in the included page by surrrounding the variable name in triple curly braces (e.g. <code>{{{Announcement text}}}</code>), and set when including a page with the bar syntax (e.g. <code>{{Announcement banner | importance = high | text = Maintenance has been planned for tonight.}}</code>). Currently the only restriction in templates and variables is that you may not include a closing curly brace (<code>}</code>) in the page name, variable name, or value.</p>
		<h5>Special Variables</h5>
		<p>$settings->sitename also supports a number of special built-in variables. Their syntax and function are described below:</p>
		<table>
			<tr><th>Type this</th><th>To get this</th></tr>
			<tr><td><code>{{{@}}}</code></td><td>Lists all variables and their values in a table.</td></tr>
			<tr><td><code>{{{#}}}</code></td><td>Shows a 'stack trace', outlining all the parent includes of the current page being parsed.</td></tr>
			<tr><td><code>{{{~}}}</code></td><td>Outputs the requested page's name.</td></tr>
			<tr><td><code>{{{*}}}</code></td><td>Outputs a comma separated list of all the subpages of the current page.</td></tr>
			<tr><td><code>{{{+}}}</code></td><td>Shows a gallery containing all the files that are sub pages of the current page.</td></tr>
		</table>");
	}
]);

/*** Parsedown versions ***
 * Parsedown Core: 1.6.0  *
 * Parsedown Extra: 0.7.0 *
 **************************/
$env->parsedown_paths = new stdClass();
$env->parsedown_paths->parsedown = "https://cdn.rawgit.com/erusev/parsedown/3ebbd730b5c2cf5ce78bc1bf64071407fc6674b7/Parsedown.php";
$env->parsedown_paths->parsedown_extra = "https://cdn.rawgit.com/erusev/parsedown-extra/11a44e076d02ffcc4021713398a60cd73f78b6f5/ParsedownExtra.php";

// Download parsedown and parsedown extra if they don't already exist
// These must still use this old method, as the parser may be asked to render some HTML before Pepperminty Wiki has had a chance to run the downloads
if(!file_exists("./Parsedown.php") || filesize("./Parsedown.php") === 0)
	file_put_contents("./Parsedown.php", fopen($env->parsedown_paths->parsedown, "r"));
if(!file_exists("./ParsedownExtra.php") || filesize("./ParsedownExtra.php") === 0)
	file_put_contents("./ParsedownExtra.php", fopen($env->parsedown_paths->parsedown_extra, "r"));

require_once("./Parsedown.php");
require_once("./ParsedownExtra.php");

/*
 * ██████   █████  ██████  ███████ ███████ ██████   ██████  ██     ██ ███    ██
 * ██   ██ ██   ██ ██   ██ ██      ██      ██   ██ ██    ██ ██     ██ ████   ██
 * ██████  ███████ ██████  ███████ █████   ██   ██ ██    ██ ██  █  ██ ██ ██  ██
 * ██      ██   ██ ██   ██      ██ ██      ██   ██ ██    ██ ██ ███ ██ ██  ██ ██
 * ██      ██   ██ ██   ██ ███████ ███████ ██████   ██████   ███ ███  ██   ████
 * 
 * ███████ ██   ██ ████████ ███████ ███    ██ ███████ ██  ██████  ███    ██ ███████
 * ██       ██ ██     ██    ██      ████   ██ ██      ██ ██    ██ ████   ██ ██
 * █████     ███      ██    █████   ██ ██  ██ ███████ ██ ██    ██ ██ ██  ██ ███████
 * ██       ██ ██     ██    ██      ██  ██ ██      ██ ██ ██    ██ ██  ██ ██      ██
 * ███████ ██   ██    ██    ███████ ██   ████ ███████ ██  ██████  ██   ████ ███████
*/
class PeppermintParsedown extends ParsedownExtra
{
	private $internalLinkBase = "./%s";
	
	protected $maxParamDepth = 0;
	protected $paramStack = [];
	
	function __construct()
	{
		// Prioritise our internal link parsing over the regular link parsing
		array_unshift($this->InlineTypes["["], "InternalLink");
		// Prioritise our image parser over the regular image parser
		array_unshift($this->InlineTypes["!"], "ExtendedImage");
		
		$this->inlineMarkerList .= "{";
		if(!isset($this->InlineTypes["{"]) or !is_array($this->InlineTypes["{"]))
			$this->InlineTypes["{"] = [];
		$this->InlineTypes["{"][] = "Template";
	}
	
	/*
	 * ████████ ███████ ███    ███ ██████  ██       █████  ████████ ██ ███    ██  ██████
	 *    ██    ██      ████  ████ ██   ██ ██      ██   ██    ██    ██ ████   ██ ██
	 *    ██    █████   ██ ████ ██ ██████  ██      ███████    ██    ██ ██ ██  ██ ██   ███
	 *    ██    ██      ██  ██  ██ ██      ██      ██   ██    ██    ██ ██  ██ ██ ██    ██
	 *    ██    ███████ ██      ██ ██      ███████ ██   ██    ██    ██ ██   ████  ██████
	 */
	protected function inlineTemplate($fragment)
	{
		global $env, $pageindex;
		
		// Variable parsing
		if(preg_match("/\{\{\{([^}]+)\}\}\}/", $fragment["text"], $matches))
		{
			$params = [];
			if(!empty($this->paramStack))
			{
				$stackEntry = array_slice($this->paramStack, -1)[0];
				$params = !empty($stackEntry) ? $stackEntry["params"] : false;
			}
			
			$variableKey = trim($matches[1]);
			
			$variableValue = false;
			switch ($variableKey)
			{
				case "@": // Lists all variables and their values
					if(!empty($params))
					{
						$variableValue = "<table>
	<tr><th>Key</th><th>Value</th></tr>\n";
						foreach($params as $key => $value)
						{
							$variableValue .= "\t<tr><td>" . $this->escapeText($key) . "</td><td>" . $this->escapeText($value) . "</td></tr>\n";
						}
						$variableValue .= "</table>";
					}
					break;
				case "#": // Shows a stack trace
					$variableValue = "<ol start=\"0\">\n";
					$variableValue .= "\t<li>$env->page</li>\n";
					foreach($this->paramStack as $curStackEntry)
					{
						$variableValue .= "\t<li>" . $curStackEntry["pagename"] . "</li>\n";
					}
					$variableValue .= "</ol>\n";
					break;
				case "~": // Show requested page's name
					if(!empty($this->paramStack))
						$variableValue = $this->escapeText($env->page);
					break;
				case "*": // Lists subpages
					$subpages = get_subpages($pageindex, $env->page);
					$variableValue = [];
					foreach($subpages as $pagename => $depth)
					{
						$variableValue[] = $pagename;
					}
					$variableValue = implode(", ", $variableValue);
					if(strlen($variableValue) === 0)
						$variableValue = "<em>(none yet!)</em>";
					break;
				case "+": // Shows a file gallery for subpages with files
					// If the upload module isn't present, then there's no point
					// in checking for uploaded files
					if(!module_exists("feature-upload"))
						break;
					
					$variableValue = [];
					$subpages = get_subpages($pageindex, $env->page);
					foreach($subpages as $pagename => $depth)
					{
						// Make sure that this is an uploaded file
						if(!$pageindex->$pagename->uploadedfile)
							continue;
						
						$mime_type = $pageindex->$pagename->uploadedfilemime;
						
						$previewSize = 300;
						$previewUrl = "?action=preview&size=$previewSize&page=" . rawurlencode($pagename);
						
						$previewHtml = "";
						switch(substr($mime_type, 0, strpos($mime_type, "/")))
						{
							case "video":
								$previewHtml .= "<video src='$previewUrl' controls preload='metadata'>$pagename</video>\n";
								break;
							case "audio":
								$previewHtml .= "<audio src='$previewUrl' controls preload='metadata'>$pagename</audio>\n";
								break;
							case "application":
							case "image":
							default:
								$previewHtml .= "<img src='$previewUrl' />\n";
								break;
						}
						$previewHtml = "<a href='?page=" . rawurlencode($pagename) . "'>$previewHtml$pagename</a>";
						
						$variableValue[$pagename] = "<li style='min-width: $previewSize" . "px; min-height: $previewSize" . "px;'>$previewHtml</li>";
					}
					
					if(count($variableValue) === 0)
						$variableValue["default"] = "<li><em>(No files found)</em></li>\n";
					$variableValue = implode("\n", $variableValue);
					$variableValue = "<ul class='file-gallery'>$variableValue</ul>";
					break;
			}
			if(isset($params[$variableKey]))
			{
				$variableValue = $params[$variableKey];
				$variableValue = $this->escapeText($variableValue);
			}
			
			if($variableValue !== false)
			{
				return [
					"extent" => strlen($matches[0]),
					"markup" => $variableValue
				];
			}
		}
		else if(preg_match("/\{\{([^}]+)\}\}/", $fragment["text"], $matches))
		{
			$templateElement = $this->templateHandler($matches[1]);
			
			if(!empty($templateElement))
			{
				return [
					"extent" => strlen($matches[0]),
					"element" => $templateElement
				];
			}
		}
	}
	
	protected function templateHandler($source)
	{
		global $pageindex, $env;
		
		
		$parts = preg_split("/\\||¦/", trim($source, "{}"));
		$parts = array_map("trim", $parts);
		
		// Extract the name of the template page
		$templatePagename = array_shift($parts);
		// If the page that we are supposed to use as the tempalte doesn't
		// exist, then there's no point in continuing.
		if(empty($pageindex->$templatePagename))
			return false;
		
		// Parse the parameters
		$this->maxParamDepth++;
		$params = [];
		$i = 0;
		foreach($parts as $part)
		{
			if(strpos($part, "=") !== false)
			{
				// This param contains an equals sign, so it's a named parameter
				$keyValuePair = explode("=", $part, 2);
				$keyValuePair = array_map("trim", $keyValuePair);
				$params[$keyValuePair[0]] = $keyValuePair[1];
			}
			else
			{
				// This isn't a named parameter
				$params["$i"] = trim($part);
				
				$i++;
			}
		}
		// Add the parsed parameters to the parameter stack
		$this->paramStack[] = [
			"pagename" => $templatePagename,
			"params" => $params
		];
		
		$templateFilePath = $env->storage_prefix . $pageindex->$templatePagename->filename;
		
		$parsedTemplateSource = $this->text(file_get_contents($templateFilePath));
		
		// Remove the parsed parameters from the stack
		array_pop($this->paramStack);
		
		return [
			"name" => "div",
			"text" => $parsedTemplateSource,
			"attributes" => [
				"class" => "template"
			]
		];
	}
	
	/*
	 * ██ ███    ██ ████████ ███████ ██████  ███    ██  █████  ██
	 * ██ ████   ██    ██    ██      ██   ██ ████   ██ ██   ██ ██
	 * ██ ██ ██  ██    ██    █████   ██████  ██ ██  ██ ███████ ██
	 * ██ ██  ██ ██    ██    ██      ██   ██ ██  ██ ██ ██   ██ ██
	 * ██ ██   ████    ██    ███████ ██   ██ ██   ████ ██   ██ ███████
	 * 
	 * ██      ██ ███    ██ ██   ██ ███████
	 * ██      ██ ████   ██ ██  ██  ██
	 * ██      ██ ██ ██  ██ █████   ███████
	 * ██      ██ ██  ██ ██ ██  ██       ██
	 * ███████ ██ ██   ████ ██   ██ ███████
	 */
	protected function inlineInternalLink($fragment)
	{
		global $pageindex, $env;
		
		if(preg_match('/^\[\[([^\]]*)\]\]([^\s!?",.()\[\]{}*=+\/]*)/u', $fragment["text"], $matches))
		{
			$linkPage = trim($matches[1]);
			$display = $linkPage . trim($matches[2]);
			if(strpos($matches[1], "|") !== false || strpos($matches[1], "¦") !== false)
			{
				// We have a bar character
				$parts = preg_split("/\\||¦/", $matches[1], 2);
				$linkPage = trim($parts[0]); // The page to link to
				$display = trim($parts[1]); // The text to display
			}
			
			$hashCode = "";
			if(strpos($linkPage, "#") !== false)
			{
				// We want to link to a subsection of a page
				$hashCode = substr($linkPage, strpos($linkPage, "#") + 1);
				$linkPage = substr($linkPage, 0, strpos($linkPage, "#"));
				
				// If $linkPage is empty then we want to link to the current page
				if(strlen($linkPage) === 0)
					$linkPage = $env->page;
			}
			
			// If the page doesn't exist, check varying different
			// capitalisations to see if it exists under some variant.
			if(empty($pageindex->$linkPage))
			{
				if(!empty($pageindex->{ucfirst($linkPage)}))
					$linkPage = ucfirst($linkPage);
				else if(!empty($pageindex->{ucwords($linkPage)}))
					$linkPage = ucwords($linkPage);
			}
			
			
			// Construct the full url
			$linkUrl = str_replace(
				"%s", rawurlencode($linkPage),
				$this->internalLinkBase
			);
			
			if(strlen($hashCode) > 0)
				$linkUrl .= "#$hashCode";
			
			$result = [
				"extent" => strlen($matches[0]),
				"element" => [
					"name" => "a",
					"text" => $display,
					"attributes" => [
						"href" => $linkUrl
					]
				]
			];
			
			if(empty($pageindex->{makepathsafe($linkPage)}))
				$result["element"]["attributes"]["class"] = "redlink";
			
			return $result;
		}
		return;
	}
	
	/*
	 * ███████ ██   ██ ████████ ███████ ███    ██ ██████  ███████ ██████
	 * ██       ██ ██     ██    ██      ████   ██ ██   ██ ██      ██   ██
	 * █████     ███      ██    █████   ██ ██  ██ ██   ██ █████   ██   ██
	 * ██       ██ ██     ██    ██      ██  ██ ██ ██   ██ ██      ██   ██
	 * ███████ ██   ██    ██    ███████ ██   ████ ██████  ███████ ██████
	 * 
	 * ██ ███    ███  █████   ██████  ███████ ███████
	 * ██ ████  ████ ██   ██ ██       ██      ██
	 * ██ ██ ████ ██ ███████ ██   ███ █████   ███████
	 * ██ ██  ██  ██ ██   ██ ██    ██ ██           ██
	 * ██ ██      ██ ██   ██  ██████  ███████ ███████
 	 */
	protected function inlineExtendedImage($fragment)
	{
		global $pageindex;
		
		if(preg_match('/^!\[(.*)\]\(([^|¦)]+)\s*(?:(?:\||¦)([^|¦)]*))?(?:(?:\||¦)([^|¦)]*))?(?:(?:\||¦)([^)]*))?\)/', $fragment["text"], $matches))
		{
			/*
			 * 0 - Everything
			 * 1 - Alt text
			 * 2 - Url
			 * 3 - First param (optional)
			 * 4 - Second param (optional)
			 * 5 - Third param (optional)
			 */
			$altText = $matches[1];
			$imageUrl = trim(str_replace("&amp;", "&", $matches[2])); // Decode & to allow it in preview urls
			$param1 = empty($matches[3]) ? false : strtolower(trim($matches[3]));
			$param2 = empty($matches[4]) ? false : strtolower(trim($matches[4]));
			$param3 = empty($matches[5]) ? false : strtolower(trim($matches[5]));
			$floatDirection = false;
			$imageSize = false;
			$imageCaption = false;
			$shortImageUrl = false;
			
			if($this->isFloatValue($param1))
			{
				// Param 1 is a valid css float: ... value
				$floatDirection = $param1;
				$imageSize = $this->parseSizeSpec($param2);
			}
			else if($this->isFloatValue($param2))
			{
				// Param 2 is a valid css float: ... value
				$floatDirection = $param2;
				$imageSize = $this->parseSizeSpec($param1);
			}
			else if($this->isFloatValue($param3))
			{
				$floatDirection = $param3;
				$imageSize = $this->parseSizeSpec($param1);
			}
			else if($param1 === false and $param2 === false)
			{
				// Neither params were specified
				$floatDirection = false;
				$imageSize = false;
			}
			else
			{
				// Neither of them are floats, but at least one is specified
				// This must mean that the first param is a size spec like
				// 250x128.
				$imageSize = $this->parseSizeSpec($param1);
			}
			
			if($param1 !== false && strtolower(trim($param1)) == "caption")
				$imageCaption = true;
				if($param2 !== false && strtolower(trim($param2)) == "caption")
					$imageCaption = true;
			if($param3 !== false && strtolower(trim($param3)) == "caption")
				$imageCaption = true;
			
			//echo("Image url: $imageUrl, Pageindex entry: " . var_export(isset($pageindex->$imageUrl), true) . "\n");
			
			if(isset($pageindex->$imageUrl) and $pageindex->$imageUrl->uploadedfile)
			{
				// We have a short url! Expand it.
				$shortImageUrl = $imageUrl;
				$imageUrl = "index.php?action=preview&size=" . max($imageSize["x"], $imageSize["y"]) ."&page=" . rawurlencode($imageUrl);
			}
			
			$style = "";
			if($imageSize !== false)
				$style .= " max-width: " . $imageSize["x"] . "px; max-height: " . $imageSize["y"] . "px;";
			if($floatDirection)
				$style .= " float: $floatDirection;";
			
			$urlExtension = pathinfo($imageUrl, PATHINFO_EXTENSION);
			$urlType = system_extension_mime_type($urlExtension);
			$result = [];
			switch(substr($urlType, 0, strpos($urlType, "/")))
			{
				case "audio":
					$result = [
						"extent" => strlen($matches[0]),
						"element" => [
							"name" => "audio",
							"text" => $altText,
							"attributes" => [
								"src" => $imageUrl,
								"controls" => "controls",
								"preload" => "metadata",
								"style" => trim($style)
							]
						]
					];
					break;
				case "video":
					$result = [
						"extent" => strlen($matches[0]),
						"element" => [
							"name" => "video",
							"text" => $altText,
							"attributes" => [
								"src" => $imageUrl,
								"controls" => "controls",
								"preload" => "metadata",
								"style" => trim($style)
							]
						]
					];
					break;
				case "image":
				default:
					// If we can't work out what it is, then assume it's an image
					$result = [
						"extent" => strlen($matches[0]),
						"element" => [
							"name" => "img",
							"attributes" => [
								"src" => $imageUrl,
								"alt" => $altText,
								"title" => $altText,
								"style" => trim($style)
							]
						]
					];
					break;
			}
			
			// ~ Image linker ~
			
			$imageHref = $shortImageUrl !== false ? "?page=" . rawurlencode($shortImageUrl) : $imageUrl;
			$result["element"] = [
				"name" => "a",
				"attributes" => [
					"href" => $imageHref
				],
				"text" => [$result["element"]],
				"handler" => "elements"
			];
			
			// ~
			
			if($imageCaption)
			{
				$rawStyle = $result["element"]["attributes"]["style"];
				$containerStyle = preg_replace('/^.*float/', "float", $rawStyle);
				$mediaStyle = preg_replace('/\s*float.*;/', "", $rawStyle);
				$result["element"] = [
					"name" => "figure",
					"attributes" => [
						"style" => $containerStyle
					],
					"text" => [
						$result["element"],
						[
							"name" => "figcaption",
							"text" => $altText
						],
					],
					"handler" => "elements"
				];
				$result["element"]["text"][0]["attributes"]["style"] = $mediaStyle;
			}
			return $result;
		}
	}
	
	# ~
	# Utility Methods
	# ~
	
	private function isFloatValue($value)
	{
		return in_array(strtolower($value), [ "left", "right" ]);
	}
	
	private function parseSizeSpec($text)
	{
		if(strpos($text, "x") === false)
			return false;
		$parts = explode("x", $text, 2);
		
		if(count($parts) != 2)
			return false;
		
		array_map("trim", $parts);
		array_map("intval", $parts);
		
		if(in_array(0, $parts))
			return false;
		
		return [
			"x" => $parts[0],
			"y" => $parts[1]
		];
	}
	
	protected function escapeText($text)
	{
		return htmlentities($text, ENT_COMPAT | ENT_HTML5);
	}
	
	/**
	 * Sets the base url to be used for internal links. '%s' will be replaced
	 * with a URL encoded version of the page name.
	 * @param string $url The url to use when parsing internal links.
	 */
	public function setInternalLinkBase($url)
	{
		$this->internalLinkBase = $url;
	}
}

?>
