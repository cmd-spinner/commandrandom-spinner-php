<?php
register_module([
	"name" => "User Preferences",
	"version" => "0.3.2",
	"author" => "Starbeamrainbowlabs",
	"description" => "Adds a user preferences page, letting pople do things like change their email address and password.",
	"id" => "feature-user-preferences",
	"code" => function() {
		global $env, $settings;
		/**
		 * @api {get} ?action=user-preferences Get a user preferences configuration page
		 * @apiName UserPreferences
		 * @apiGroup Settings
		 * @apiPermission User
		 */
		
		 /*
 		 * ██    ██ ███████ ███████ ██████
 		 * ██    ██ ██      ██      ██   ██
 		 * ██    ██ ███████ █████   ██████  █████
 		 * ██    ██      ██ ██      ██   ██
 		 *  ██████  ███████ ███████ ██   ██
 		 * 
 		 * ██████  ██████  ███████ ███████ ███████
 		 * ██   ██ ██   ██ ██      ██      ██
 		 * ██████  ██████  █████   █████   ███████
 		 * ██      ██   ██ ██      ██           ██
 		 * ██      ██   ██ ███████ ██      ███████
 		 */
		add_action("user-preferences", function() {
			global $env, $settings;
			
			if(!$env->is_logged_in)
			{
				exit(page_renderer::render_main("Error  - $settings->sitename", "<p>Since you aren't logged in, you can't change your preferences. This is because stored preferences are tied to each registered user account. You can login <a href='?action=login&returnto=" . rawurlencode("?action=user-preferences") . "'>here</a>.</p>"));
			}
			
			$statusMessages = [
				"change-password" => "Password changed successfully!"
			];
			
			if(!isset($env->user_data->emailAddress)) {
				$env->user_data->emailAddress = "";
				save_userdata();
			}
			
			$content = "<h2>User Preferences</h2>\n";
			if(isset($_GET["success"]) && $_GET["success"] === "yes")
			{
				$content .= "<p class='user-prefs-status-message'><em>" . $statusMessages[$_GET["operation"]] . "</em></p>\n";
			}
			// If avatar support is present, allow the user to upload a new avatar
			if(has_action("avatar") && module_exists("feature-upload")) {
				$content .= "<a href='?action=upload&avatar=yes' class='preview'><figure>\n";
				$content .= "\t<img class='avatar' src='?action=avatar&user=" . urlencode($env->user) . "&size=256' title='Your current avatar - click to upload a new one' />\n";
				$content .= "<figcaption>Upload a new avatar</figcaption>\n";
				$content .= "</figure></a><br />\n";
			}
			$content .= "<label for='username'>Username:</label>\n";
			$content .= "<input type='text' name='username' value='$env->user' readonly />\n";
			$content .= "<form method='post' action='?action=save-preferences'>\n";
			$content .= "	<label for='email-address'>Email Address:</label>\n";
			$content .= "	<input type='email' id='email-address' name='email-address' placeholder='e.g. bob@bobsrockets.com' value='{$env->user_data->emailAddress}' />\n";
			$content .= "	<p><small>Used to send you notifications etc. Never shared with anyone except $settings->admindetails_name, $settings->sitename's administrator.</small></p>\n";
			$content .= "	<input type='submit' value='Save Preferences' />\n";
			$content .= "</form>\n";
			$content .= "<h3>Change Password</h3\n>";
			$content .= "<form method='post' action='?action=change-password'>\n";
			$content .= "	<label for='old-pass'>Current Password:</label>\n";
			$content .= "	<input type='password' name='current-pass'  />\n";
			$content .= "	<br />\n";
			$content .= "	<label for='new-pass'>New Password:</label>\n";
			$content .= "	<input type='password' name='new-pass' />\n";
			$content .= "	<br />\n";
			$content .= "	<label for='new-pass-confirm'>Confirm New Password:</label>\n";
			$content .= "	<input type='password' name='new-pass-confirm' />\n";
			$content .= "	<br />\n";
			$content .= "	<input type='submit' value='Change Password' />\n";
			$content .= "</form>\n";
			
			if($env->is_admin)
				$content .= "<p>As an admin, you can also <a href='?action=configure'>edit $settings->sitename's master settings</a>.</p>\n";
			
			exit(page_renderer::render_main("User Preferences - $settings->sitename", $content));
		});
		
		/**
		 * @api {post} ?action=save-preferences Save your user preferences
		 * @apiName UserPreferencesSave
		 * @apiGroup Settings
		 * @apiPermission User
		 */
		add_action("save-preferences", function() {
			global $env, $settings;
			
			if(!$env->is_logged_in)
			{
				http_response_code(400);
				exit(page_renderer::render_main("Error Saving Preferences - $settings->sitename", "<p>You aren't logged in, so you can't save your preferences. Try <a href='?action=login&returnto=" . rawurlencode("?action=user-preferences") . "'>logging in</a> first.</p>"));
			}
			
			if(isset($_POST["email-address"]))
			{
				if(mb_strlen($_POST["email-address"]) > 320)
				{
					http_response_code(413);
					exit(page_renderer::render_main("Error Saving Email Address - $settings->sitename", "<p>The email address you supplied (<code>{$_POST['email-address']}</code>) is too long. Email addresses can only be 320 characters long. <a href='javascript:window.history.back();'>Go back</a>."));
				}
				
				if(mb_strpos($_POST["email-address"], "@") === false)
				{
					http_response_code(422);
					exit(page_renderer::render_main("Error Saving Email Address - $settings->sitename", "<p>The email address you supplied (<code>{$_POST['email-address']}</code>) doesn't appear to be valid. <a href='javascript:window.history.back();'>Go back</a>."));
				}
				
				$env->user_data->emailAddress = $_POST["email-address"];
			}
			
			// Save the user's preferences
			if(!save_userdata())
			{
				http_response_code(503);
				exit(page_renderer::render_main("Error Saving Preferences - $settings->sitename", "<p>$settings->sitename had some trouble saving your preferences! Please contact $settings->admindetails_name, $settings->sitename's administrator and tell them about this error if it still occurs in 5 minutes. They can be contacted by email at this address: <a href='mailto:" . hide_email($settings->admindetails_email) . "'>" . hide_email($settings->admindetails_email) . "</a>.</p>"));
			}
			
			exit(page_renderer::render_main("Preferences Saved Successfully - $settings->sitename", "<p>Your preferences have been saved successfully! You could go back your <a href='?action=user-preferences'>preferences page</a>, or on to the <a href='?page=" . rawurlencode($settings->defaultpage) . "'>$settings->defaultpage</a>.</p>"));
		});
		
		/**
		 * @api	{post}	?action=change-password	Change your password
		 * @apiName			ChangePassword
		 * @apiGroup		Settings
		 * @apiPermission	User
		 *
		 * @apiParam	{string}	current-pass		Your current password.
		 * @apiParam	{string}	new-pass			Your new password.
		 * @apiParam	{string}	new-pass-confirm	Your new password again, to make sure you've typed it correctly.
		 *
		 * @apiError	PasswordMismatchError	The new password fields don't match.
		 */
		add_action("change-password", function() {
		    global $env, $settings;
			
			// Make sure the new password was typed correctly
			// This comes before the current password check since that's more intensive
			if($_POST["new-pass"] !== $_POST["new-pass-confirm"]) {
				exit(page_renderer::render_main("Password mismatch - $settings->sitename", "<p>The new password you typed twice didn't match! <a href='javascript:history.back();'>Go back</a>.</p>"));
			}
			// Check the current password
			if(hash_password($_POST["current-pass"]) !== $env->user_data->password) {
				exit(page_renderer::render_main("Password mismatch - $settings->sitename", "<p>Error: You typed your current password incorrectly! <a href='javascript:history.back();'>Go back</a>.</p>"));
			}
			
			// All's good! Go ahead and change the password.
			$env->user_data->password = hash_password($_POST["new-pass"]);
			// Save the userdata back to disk
			save_userdata();
			
			http_response_code(307);
			header("location: ?action=user-preferences&success=yes&operation=change-password");
			exit(page_renderer::render_main("Password Changed Successfully", "<p>You password was changed successfully. <a href='?action=user-preferences'>Go back to the user preferences page</a>.</p>"));
		});
		
		
		/*
		 *  █████  ██    ██  █████  ████████  █████  ██████
		 * ██   ██ ██    ██ ██   ██    ██    ██   ██ ██   ██
		 * ███████ ██    ██ ███████    ██    ███████ ██████
		 * ██   ██  ██  ██  ██   ██    ██    ██   ██ ██   ██
		 * ██   ██   ████   ██   ██    ██    ██   ██ ██   ██
		 */
		
 		/**
 		 * @api	{get}	?action=avatar&user={username}[&size={size}]	Get a user's avatar
 		 * @apiName			Avatar
 		 * @apiGroup		Upload
 		 * @apiPermission	Anonymous
 		 *
 		 * @apiParam	{string}	user			The username to fetch the avatar for
 		 * @apiParam	{string}	size			The preferred size of the avatar
 		 */
		add_action("avatar", function() {
			global $settings;
			
			$size = intval($_GET["size"] ?? 32);
			
			/// Use gravatar if there's some issue with the requested user
			
			// No user specified
			if(empty($_GET["user"])) {
				http_response_code(307);
				header("x-reason: no-user-specified");
				header("location: https://gravatar.com/avatar/?default=blank");
			}
			
			$requested_username = $_GET["user"];
			
			// The user hasn't uploaded an avatar
			if(empty($pageindex->{"User/$requested_username/Avatar"}) || !$pageindex->{"User/$requested_username/Avatar"}->uploadedfile) {
				$user_fragment = !empty($settings->users->$requested_username->emailAddress) ? $settings->users->$requested_username->emailAddress : $requested_username;
				
				http_response_code(307);
				header("x-reason: no-avatar-found");
				header("x-hash-method: " . ($user_fragment === $requested_username ? "username" : "email_address"));
				header("location: https://gravatar.com/avatar/" . md5($user_fragment) . "?default=identicon&rating=g&size=$size");
				exit();
			}
			
			// The user has uploaded an avatar, so we can redirec to the regular previewer :D
			
			http_response_code(307);
			header("x-reason: found-local-avatar");
			header("location: ?action=preview&size=$size&page=" . urlencode("Users/$requested_username/Avatar"));
			exit("This user's avatar can be found at Files/$requested_username/Avatar");
		});
		
		// Display a help section on the user preferences, but only if the user
		// is logged in and so able to access them
		if($env->is_logged_in)
		{
			add_help_section("910-user-preferences", "User Preferences", "<p>As you are logged in, $settings->sitename lets you configure a selection of personal preferences. These can be viewed and tweaked to you liking over on the <a href='?action=user-preferences'>preferences page</a>, which can be accessed at any time by clicking the cog icon (it looks something like this: <a href='?action=user-preferences'>$settings->user_preferences_button_text</a>), though the administrator of $settings->sitename ($settings->admindetails_name) may have changed its appearance.</p>");
		}
		
		if($settings->avatars_show)
		{
			add_help_section("915-avatars", "Avatars", "<p>$settings->sitename allows you to upload an avatar and have it displayed next to your name. If you don't have an avatar uploaded yet, then $settings->sitename will take a <a href='https://www.techopedia.com/definition/19744/hash-function'>hash</a> of your email address and ask <a href='https://gravatar.com'>Gravatar</a> for for your Gravatar instead. If you haven't told $settings->sitename what your email address is either, a hash of your username is used instead. If you don't have a gravatar, then $settings->sitename asks Gravatar for an identicon instead.</p>
			<p>Your avatar on $settings->sitename currently looks like this: <img class='avatar' src='?action=avatar&user=" . urlencode($env->user) . "' />" . ($settings->upload_enabled ? " - you can upload a new one by going to your <a href='?action=user-preferences'>preferences</a>, or <a href='?action=upload&avatar=yes' />clicking here</a>." : ", but $settings->sitename currently has uploads disabled, so you can't upload a new one directly to $settings->sitename. You can, however, set your email address in your <a href='?action=user-preferences'>preferences</a> and <a href='https://en.gravatar.com/'>create a Gravatar</a>, and then it should show up here on $settings->sitename shortly.") . "</p>");
		}
	}
]);

?>
