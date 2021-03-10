<?php 
if ( ! defined( 'ABSPATH' ) ) exit; // Exit if accessed directly

function pie_edit_userdata($form_id = "default"){
	global $current_user;
	wp_get_current_user();
	$form 		= new Edit_form_template($current_user,$form_id);
	$profile_fields_data = "";
	$profile_fields_data .= '<div class="pieregProfileWrapper pieregWrapper">
	<form enctype="multipart/form-data" id="pie_regiser_form" method="post" action="'.$_SERVER['REQUEST_URI'].'">';
	
	if( function_exists( 'wp_nonce_field' )) 
		$profile_fields_data .= wp_nonce_field( 'piereg_wp_edit_profile_nonce','piereg_edit_profile_nonce'); 
	
	$output = $form->editProfile($current_user);
	$profile_fields_data .= $output ;
	$profile_fields_data .= '</ul></form></div>';
	return $profile_fields_data;
}