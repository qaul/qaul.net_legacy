$(document).ready(function(){
	loading_show();
	load_config();
});

function load_config()
{
	$.getJSON("/cgi-bin/qaul/config", function(data){
		if(data.auth == 0)
		{
			hide_all("#page_login");
			$("#page_login").show();
		}
		else if(data.auth == 2)
		{
			hide_all("#page_newpassword");
			$("#page_newpassword").show();
		}
		else if(data.auth == 1)
		{
			if($("#page_eth").is(':visible'))
			{
				hide_all("#page_eth");
				$("#page_eth").show();
			}
			else
			{
				hide_all("#page_main");
				$("#page_main").show();
			}
			
			if(data.internet)
			{
				$('#internet_button').addClass('selected')
					.removeAttr('onclick')
					.attr('onclick',"config('internet_off')");
				$('#internet_checkbox').prop('checked', true);
				$('#internet_checkbox').removeAttr('onclick')
					.attr('onclick',"config('internet_off')");
			}
			else
			{
				$('#internet_button').removeClass('selected')
					.removeAttr('onclick')
					.attr('onclick',"config('internet_on')");
				$('#internet_checkbox').prop('checked', false);
				$('#internet_checkbox').removeAttr('onclick')
					.attr('onclick',"config('internet_on')");
			}
				
			if(data.mobile)
				$('#mobile').addClass('selected');
			else
				$('#mobile').removeClass('selected');
				
			if(data.storage)
				$('#storage').addClass('selected');
			else
				$('#storage').removeClass('selected');
				
			if(data.eth_config)
			{
				$("#network_eth").show();
				
				if(data.eth == 'qaul')
				{
					$('#eth_qaul').addClass('selected');
					$('#eth_wan').removeClass('selected');
				}
				else
				{
					$('#eth_wan').addClass('selected');
					$('#eth_qaul').removeClass('selected');
				}
			}
			
			if(data.config)
			{
				$("#header_ifconfig").empty();
				data.config.forEach(function(item){
					$("#header_ifconfig").append(item +"<br/>");
				});
			}
		}
		
		loading_hide();
	}).error(function(){
		alert('load error');
	});
}

function loading_show()
{
	$("#loading").fadeIn();
	$("img#loader").addClass("rotate");	
}
function loading_hide()
{
	$("#loading").fadeOut();
	$("img#loader").removeClass("rotate");
}

function config(target)
{
	loading_show();
	$.getJSON("/cgi-bin/qaul/" +target, function(data){
		load_config();
	}).error(function(){
		alert('configuration error');
	});
	
	return false;
}

function config_storage()
{
	hide_all("#page_storage");
	$("#page_storage").show();
}

function config_mobile()
{
	hide_all("#page_mobile");
	$("#page_mobile").show();
}

function config_eth()
{
	hide_all("#page_eth");
	$("#page_eth").show();
}

function hide_all(exept)
{
	if(exept != "#page_main")
		$("#page_main").hide();
	if(exept != "#page_login")
		$("#page_login").hide();
	if(exept != "#page_newpassword")
		$("#page_newpassword").hide();
	if(exept != "#page_storage")
		$("#page_storage").hide();
	if(exept != "#page_mobile")
		$("#page_mobile").hide();
	if(exept != "#page_eth")
		$("#page_eth").hide();
}

function back()
{
	hide_all("#page_main");
	$("#page_main").show();
}

function submit_login()
{
	if(event.preventDefault)
		event.preventDefault();
    else
        event.returnValue = false;

	loading_show();
	var formdata = {pw:$("#login_password").val()};
	$.ajax({
		url: "/cgi-bin/qaul/login",
		type: "POST",
		data: formdata,
		cache: false,
		dataType: "json",
		success: function(data){
			load_config();
		}
	}).error(function(){
		alert('login error');
	});	
	load_config();
	return false;
}

function submit_newpassword()
{
	if(event.preventDefault)
		event.preventDefault();
    else
        event.returnValue = false;
        
	if($("#new_password").val() == $("#new_password2").val())
	{
		loading_show();
		var formdata = {pw:$("#new_password").val(),pin:$("#mobile_pin").val(),un:$("#mobile_un").val(),pw:$("#mobile_pw").val()};
		$.ajax({
			url: "/cgi-bin/qaul/newpassword",
			type: "POST",
			data: formdata,
			cache: false,
			dataType: "json",
			success: function(data){
				load_config();
			}
		}).error(function(){
			alert('new password error');
		});
	}
	else
		alert('Passwords are not equal!');
		
	return false;
}

function submit_mobile()
{
	if(event.preventDefault)
		event.preventDefault();
    else
        event.returnValue = false;
        
	loading_show();	
	var formdata = {apn:$("#mobile_apn").val(),pin:$("#mobile_pin").val(),un:$("#mobile_un").val(),pw:$("#mobile_pw").val()};
	$.ajax({
		url: "/cgi-bin/qaul/mobile",
		type: "POST",
		data: formdata,
		cache: false,
		dataType: "json",
		success: function(data){
			load_config();
		}
	}).error(function(){
		alert('mobile configuration error');
	});
	return false;
}
