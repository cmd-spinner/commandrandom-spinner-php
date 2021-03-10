<?php
/*
 * Pepperminty Wiki
 * ================
 * Inspired by Minty Wiki by am2064
	* Link: https://github.com/am2064/Minty-Wiki
 * 
 * Credits:
	* Code by @Starbeamrainbowlabs
	* Parsedown - by erusev and others on github from http://parsedown.org/
	* Mathematical Expression rendering
		* Code: @con-f-use <https://github.com/con-f-use>
		* Rendering: MathJax (https://www.mathjax.org/)
 * Bug reports:
	* #2 - Incorrect closing tag - nibreh <https://github.com/nibreh/>
	* #8 - Rogue <datalist /> tag - nibreh <https://github.com/nibreh/>
 */
$guiConfig = <<<'GUICONFIG'
{guiconfig}
GUICONFIG;

$settingsFilename = "peppermint.json";

if(file_exists("$settingsFilename.compromised")) {
	http_response_code(500);
	header("content-type: text/plain");
	exit("Error: $settingsFilename.compromised exists on disk, so it's likely you need to block access to 'peppermint.json' from the internet. If you've done this already, please delete $settingsFilename.compromised and reload this page.");
}

$guiConfig = json_decode($guiConfig);
$settings = new stdClass();
if(!file_exists($settingsFilename))
{
	// Copy the default settings over to the main settings array
	foreach ($guiConfig as $key => $value)
		$settings->$key = $value->default;
	// Generate a random secret
	$settings->secret = bin2hex(openssl_random_pseudo_bytes(16));
	file_put_contents("peppermint.json", json_encode($settings, JSON_PRETTY_PRINT));
}
else
	$settings = json_decode(file_get_contents("peppermint.json"));

if($settings === null)
{
	header("content-type: text/plain");
	exit("Error: Failed to decode the settings file! Does it contain a syntax error?");
}

// Fill in any missing properties
$settingsUpgraded = false;
foreach($guiConfig as $key => $propertyData)
{
	if(!isset($settings->$key))
	{
		$settings->$key = $propertyData->default;
		$settingsUpgraded = true;
	}
}
if($settingsUpgraded)
	file_put_contents("peppermint.json", json_encode($settings, JSON_PRETTY_PRINT));

// Insert the default CSS if requested
$defaultCSS = <<<THEMECSS
{default-css}
THEMECSS;
if($settings->css === "auto")
	$settings->css = $defaultCSS;

?>
