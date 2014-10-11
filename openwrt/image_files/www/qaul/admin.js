$(document).ready(function(){
	loading_show("");
	load_config();
	
	$("#form_login").submit(function(e){submit_login(e);});
	$("#form_newpassword").submit(function(e){submit_newpassword(e);});
	$("#form_storage").submit(function(e){submit_storage(e);});
	$("#form_mobile").submit(function(e){submit_mobile(e);});
	$("#form_wifi").submit(function(e){submit_wifi(e);});
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
				
			if(data.mobile.configured)
			{
				$('#mobile').addClass('selected');
				
				$('#mobile_apn').val(data.mobile.apn);
				$('#mobile_pin').val(data.mobile.pin);
				$('#mobile_username').val(data.mobile.user);
				$('#mobile_password').val(data.mobile.password);
			}
			else
			{
				$('#mobile').removeClass('selected');
				
				$('#mobile_apn').val("");
				$('#mobile_pin').val("");
				$('#mobile_username').val("");
				$('#mobile_password').val("");
			}
				
			if(data.storage)
			{
				$('#storage').addClass('selected');
				$('#storage_on').show();
				$('#storage_off').hide();
			}
			else
			{
				$('#storage').removeClass('selected');
				$('#storage_on').hide();
				$('#storage_off').show();
			}
			$("#storage_total span").empty().append(data.storage_total);
			$("#storage_free span").empty().append(data.storage_free);
				
			if(data.eth_config)
			{
				$("#network_eth").show();
				
				if(data.eth == 'qauleth')
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
			
			if(data.wifi.qaulwifi == 1)
			{
				$('#wifi_qaul_checkbox').prop('checked', true);
			}
			else
			{
				$('#wifi_qaul_checkbox').prop('checked', false);
			}
			if(data.wifi.lanwifi == 1)
			{
				$('#wifi_lan_checkbox').prop('checked', true);
			}
			else
			{
				$('#wifi_lan_checkbox').prop('checked', false);
			}
			if(data.wifi.distance)
			{
				$('#wifi_distance option').prop('selected', false)
                   .filter('[value="' +data.wifi.distance +'"]')
                   .prop('selected', true);
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

function loading_show(info)
{
	$("#loading_info").empty().append(info);
	$("#loading").fadeIn();
	if(!$("img#loader").hasClass("rotate"))
	{
		$("img#loader").addClass("rotate");
	}
}
function loading_hide()
{
	$("#loading").fadeOut();
	$("img#loader").removeClass("rotate");
	$("#loading_info").empty();
}

function config_reboot()
{
	loading_show("rebooting ...");
	setTimeout(function(){load_config();},50000);
}

function config_network(target)
{
	loading_show("configuring network ...");
	$.getJSON("/cgi-bin/qaul/" +target, function(data){
		setTimeout(function(){load_config();},25000);
	}).error(function(){
		alert('network configuration error');
	});
	
	return false;
}

function config_storage()
{
	loading_show("configuring ...");
	$.getJSON("/qaul/storage_config", function(data){
		setTimeout(function(){config_storage();},3000);
	}).error(function(){
		config_reboot();
	});
}

function config(target)
{
	loading_show("");
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

function config_wifi()
{
	hide_all("#page_wifi");
	$("#page_wifi").show();
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
	if(exept != "#page_wifi")
		$("#page_wifi").hide();
}

function back()
{
	hide_all("#page_main");
	$("#page_main").show();
}

function submit_login(e)
{
	if(e.preventDefault)
		e.preventDefault();
    else
        e.returnValue = false;

	loading_show("");
	var formdata = {pw:$("#login_password").val()};
	$.post(
		"/cgi-bin/qaul/login",
		formdata,
		function(data){
			load_config();
		}
	).error(function(){
		alert('login error');
	});	
	load_config();
	return false;
}

function submit_newpassword(e)
{
	if(e.preventDefault)
		e.preventDefault();
    else
        e.returnValue = false;
        
	if($("#new_password").val() == $("#new_password2").val())
	{
		loading_show("");
		var formdata = {pw:$("#new_password").val()};
		$.post(
			"/cgi-bin/qaul/newpassword",
			formdata,
			function(data){
				load_config();
			}
		).error(function(){
			alert('new password error');
		});
	}
	else
		alert('Passwords are not equal!');
		
	return false;
}

function submit_mobile(e)
{
	if(e.preventDefault)
		e.preventDefault();
    else
        e.returnValue = false;
        
	loading_show("");	
	var formdata = {apn:$("#mobile_apn").val(),pin:$("#mobile_pin").val(),un:$("#mobile_un").val(),pw:$("#mobile_pw").val()};
	$.post(
		"/cgi-bin/qaul/mobile",
		formdata,
		function(data){
			load_config();
		}
	).error(function(){
		alert('mobile configuration error');
	});
	return false;
}

function submit_storage(e)
{
	if(e.preventDefault)
		e.preventDefault();
    else
        e.returnValue = false;

	loading_show("configuring ...");
	var formdata = {"s":1};
	$.post(
		"/cgi-bin/qaul/storage",
		formdata,
		function(data){
			config_storage();
		}
	).error(function(){
		config_reboot();
	});
	return false;
}

function submit_wifi(e)
{
	if(e.preventDefault)
		e.preventDefault();
    else
        e.returnValue = false;

	loading_show("configuring wifi ...");
	if($("#wifi_qaul_checkbox").is(":checked"))
		qaulwifi = 1;
	else
		qaulwifi = 0;
	if($("#wifi_lan_checkbox").is(":checked"))
		lanwifi = 1;
	else
		lanwifi = 0;
	var formdata = {"qaulwifi":qaulwifi,"lanwifi":lanwifi,"dist":$("#wifi_distance").val()};
	$.post(
		"/cgi-bin/qaul/wifi",
		formdata,
		function(data){
			setTimeout(function(){load_config();},25000);
		}
	).error(function(){
		alert('wifi configuration error');
	});
	return false;
}
