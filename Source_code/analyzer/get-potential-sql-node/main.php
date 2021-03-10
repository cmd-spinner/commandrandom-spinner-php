<?php
require_once __DIR__."/php-parser/lib/bootstrap.php";
function getAllPhpFiles($Path)
{
	$Directory = new RecursiveDirectoryIterator($Path);
	$Iterator = new RecursiveIteratorIterator($Directory);
	$list=[];
	foreach ($Iterator as $it)
		if (pathinfo($it->getPathName(), PATHINFO_EXTENSION)=="php")
			if (basename($it->getPathName())!="wp-sql-sink.php") //don't list your own script file!
				$list[]=$it->getPathName();
	return $list;
}
function plugin_name($file)
{
	global $wpdir;
	$plugindir=realpath($wpdir."/wp-content/plugins");
	if (substr($file,0,strlen($plugindir))==$plugindir)
	{
		$rest=substr($file,strlen($plugindir)+1);
		$t=explode("/",$rest);
		return $t[0];
	}
	return null;
}
function is_plugin($file)
{
	return plugin_name($file)!==null;
}

$usage="Usage: ".basename($argv[0])." install|uninstall|extract wordpress_folder\n".
		"		extract wordpress_folder\n".
		"		install wordpress_folder\n".
		"		uninstall wordpress_folder\n";
if (!isset($argc) or $argc<3)
	die($usage);
if (function_exists("xdebug_is_enabled"))
	echo "WARNING: Xdebug detected. For this script to work properly, you need to add 'xdebug.max_nesting_level = 5000' to your php.ini.\n";
global $wpdir;
$wpdir=realpath($argv[2]);
if (!$wpdir)
	die("Invalid Wordpress path.\n");
if ($argv[1]=="extract")
	require_once "src/extract.php";
elseif ($argv[1]=="install")
	require_once "src/install.php";
elseif ($argv[1]=="uninstall")
	require_once "src/uninstall.php";
else
	die($usage);

