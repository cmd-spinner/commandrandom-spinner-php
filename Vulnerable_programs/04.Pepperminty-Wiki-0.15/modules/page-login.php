<?php
register_module([
	"name" => "Login",
	"version" => "0.8.5",
	"author" => "Starbeamrainbowlabs",
	"description" => "Adds a pair of actions (login and checklogin) that allow users to login. You need this one if you want your users to be able to login.",
	"id" => "page-login",
	"code" => function() {
		global $settings;
		
		/**
		 * @api		{get}	?action=login[&failed=yes][&returnto={someUrl}]	Get the login page
		 * @apiName		Login
		 * @apiGroup	Authorisation
		 * @apiPermission	Anonymous
		 * 
		 * @apiParam	{string}	failed		Setting to yes causes a login failure message to be displayed above the login form.
		 * @apiParam	{string}	returnto	Set to the url to redirect to upon a successful login.
		 */
		
		/*
		 * ██       ██████   ██████  ██ ███    ██
		 * ██      ██    ██ ██       ██ ████   ██
		 * ██      ██    ██ ██   ███ ██ ██ ██  ██
		 * ██      ██    ██ ██    ██ ██ ██  ██ ██
		 * ███████  ██████   ██████  ██ ██   ████
		 */
		add_action("login", function() {
			global $settings, $env;
			
			// Build the action url that will actually perform the login
			$login_form_action_url = "index.php?action=checklogin";
			if(isset($_GET["returnto"]))
				$login_form_action_url .= "&returnto=" . rawurlencode($_GET["returnto"]);
			
			if($env->is_logged_in && !empty($_GET["returnto"]))
			{
				http_response_code(307);
				header("location: " . $_GET["returnto"]);
			}
			
			$title = "Login to $settings->sitename";
			$content = "<h1>Login to $settings->sitename</h1>\n";
			if(isset($_GET["failed"]))
				$content .= "\t\t<p><em>Login failed.</em></p>\n";
			if(isset($_GET["required"]))
				$content .= "\t\t<p><em>$settings->sitename requires that you login before continuing.</em></p>\n";
			$content .= "\t\t<form method='post' action='$login_form_action_url'>
				<label for='user'>Username:</label>
				<input type='text' name='user' id='user' autofocus />
				<br />
				<label for='pass'>Password:</label>
				<input type='password' name='pass' id='pass' />
				<br />
				<input type='submit' value='Login' />
			</form>\n";
			exit(page_renderer::render_main($title, $content));
		});
		
		/**
		 * @api		{post}	?action=checklogin	Perform a login
		 * @apiName		CheckLogin
		 * @apiGroup	Authorisation
		 * @apiPermission	Anonymous
		 * 
		 * @apiParam	{string}	user		The user name to login with.
		 * @apiParam	{string}	password	The password to login with.
		 * @apiParam	{string}	returnto	The URL to redirect to upon a successful login.
		 *
		 * @apiError	InvalidCredentialsError	The supplied credentials were invalid. Note that this error is actually a redirect to ?action=login&failed=yes (with the returnto parameter appended if you supplied one)
		 */
		
		/*
 		 * ██████ ██   ██ ███████  ██████ ██   ██
		 * ██     ██   ██ ██      ██      ██  ██
		 * ██     ███████ █████   ██      █████
		 * ██     ██   ██ ██      ██      ██  ██
 		 * ██████ ██   ██ ███████  ██████ ██   ██
 		 * 
		 * ██       ██████   ██████  ██ ███    ██
		 * ██      ██    ██ ██       ██ ████   ██
		 * ██      ██    ██ ██   ███ ██ ██ ██  ██
		 * ██      ██    ██ ██    ██ ██ ██  ██ ██
		 * ███████  ██████   ██████  ██ ██   ████
		 */
		add_action("checklogin", function() {
			global $settings, $env;
			
			//actually do the login
			if(isset($_POST["user"]) and isset($_POST["pass"]))
			{
				//the user wants to log in
				$user = $_POST["user"];
				$pass = $_POST["pass"];
				if($settings->users->$user->password == hash_password($pass))
				{
					$env->is_logged_in = true;
					$expiretime = time() + 60*60*24*30; //30 days from now
					$_SESSION["$settings->sessionprefix-user"] = $user;
					$_SESSION["$settings->sessionprefix-pass"] = hash_password($pass);
					$_SESSION["$settings->sessionprefix-expiretime"] = $expiretime;
					//redirect to wherever the user was going
					http_response_code(302);
					if(isset($_GET["returnto"]))
						header("location: " . $_GET["returnto"]);
					else
						header("location: index.php");
					exit();
				}
				else
				{
					http_response_code(302);
					$nextUrl = "index.php?action=login&failed=yes";
					if(!empty($_GET["returnto"]))
						$nextUrl .= "&returnto=" . rawurlencode($_GET["returnto"]);
					header("location: $nextUrl");
					exit();
				}
			}
			else
			{
				http_response_code(302);
				$nextUrl = "index.php?action=login&failed=yes&badrequest=yes";
				if(!empty($_GET["returnto"]))
					$nextUrl .= "&returnto=" . rawurlencode($_GET["returnto"]);
				header("location: $nextUrl");
				exit();
			}
		});
		
		// Register a section on logging in on the help page.
		add_help_section("30-login", "Logging in", "<p>In order to edit $settings->sitename and have your edit attributed to you, you need to be logged in. Depending on the settings, logging in may be a required step if you want to edit at all. Thankfully, loggging in is not hard. Simply click the &quot;Login&quot; link in the top left, type your username and password, and then click login.</p>
		<p>If you do not have an account yet and would like one, try contacting <a href='mailto:" . hide_email($settings->admindetails_email) . "'>$settings->admindetails_name</a>, $settings->sitename's administrator and ask them nicely to see if they can create you an account.</p>");
	}
]);

/**
 * Hashes the given password according to the current settings defined
 * in $settings.
 * @package	page-login
 * @param	string	$pass	The password to hash.
 * 
 * @return	string	The hashed password. Uses sha3 if $settings->use_sha3 is
 * 					enabled, or sha256 otherwise.
 */
function hash_password($pass)
{
	global $settings;
	if($settings->use_sha3)
	{
		return sha3($pass, 256);
	}
	else
	{
		return hash("sha256", $pass);
	}
}

?>
