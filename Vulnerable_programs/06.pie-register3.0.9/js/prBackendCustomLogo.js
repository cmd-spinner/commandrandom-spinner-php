/*************************************************/
///////////////// CUSTOM LOGO /////////////////////
jQuery(document).on("click", "#pie_custom_logo_button", function() {
	var $Width = window.innerWidth - 100;
	var $Height = window.innerHeight - 100;
	formfield = jQuery("#pie_custom_logo_url").prop("name");
	tb_show(pie_pr_backend_dec_vars.selectLogoText, pie_pr_backend_dec_vars.mediaUploadURL+"?post_id=0&amp;type=image&amp;context=custom-logo&amp;TB_iframe=1&amp;height="+$Height+"&amp;width="+$Width);
});

window.send_to_editor = function(html) {
	var imgsrc;
	if(jQuery(html).is("img"))
	{
		imgsrc = jQuery(html).attr("src");
	}	
	//jQuery("#pie_custom_logo_url").val(jQuery("img", html).attr("src"));
	jQuery("#pie_custom_logo_url").val(imgsrc);
	tb_remove();
}
