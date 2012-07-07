/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

// ======================================================
// global definitions
// ------------------------------------------------------
var chat_form;
var msg;
var user_name = "";
var starting;
var chat;
var users;
var name_form;
var count = 0;
var tag_last_id = 0;
var tag_name = "";
var user_last_id = 0;
var qaul_config;
var node_count = 0;
var user_count = 0;

var qaulfiles = [];
var qaulfileevent = 0;
var qaulusers = [];
var qauluserevent = 0;
var qaulmessageevent = 0;
var qaulinitialized = false;
var chat_initialized = false;
var is_chrome = false;

// ======================================================
// initialize
// ------------------------------------------------------

function init_start()
{
	$.mobile.changePage('#page_loading');
	setTimeout(function(){loadingtimer();},1000);
	
	// bugfix check if browser is chrome
	is_chrome = /chrome/.test(navigator.userAgent.toLowerCase());
	
	// declarations
	chat_form=$('#chat_form');
	msg=$('#chat_msg');
	starting=$('#starting');
	chat=$('#chat');
	users=$('#users');
	name_form=$('#name_form');
	
	// events
	$(document).bind("pagechange", onPageChange);
	$(document).bind("pagebeforechange", onPageBeforeChange);
	
	// message forms
	chat_form.validate({
		submitHandler: function(form){
			send_msg();
		}
	});
	
	$("#tag_chat_form").validate({
		submitHandler: function(form){
			send_tag_msg();
		}
	});
	
	$("#user_chat_form").validate({
		submitHandler: function(form){
			send_direct_msg();
		}
	});
	
	// set locale
	$("#locale_submit").click(function(){
		send_locale();
		return false;
	});
	
	// set username
	name_form.validate({
		submitHandler: function(form){
			send_name();
		}
	});
	
	// files
	$("#file_add_form").validate({
		submitHandler: function(form){
			send_file_add();
		}
	});
	
	// ------------------------------------------------------
	// for msie < 9 compatibility (because jqm breaks the onsubmit event)
	if($.browser.msie && $.browser.version < 9)
	{
		// message forms
		$("#chat_form input").keypress(function(e){
			if(e.which == 13)
			{
				send_msg();
				e.preventDefault();
				return false;
			}
		});
		$("#chat_submit").click(function(){
			send_msg();
			return false;
		});
		
		$("#tag_chat_form input").keypress(function(e){
			if(e.which == 13)
			{
				send_tag_msg();
				e.preventDefault();
				return false;
			}
		});
		$("#tag_chat_submit").click(function(){
			send_tag_msg();
			return false;
		});
		
		$("#user_chat_form input").keypress(function(e){
			if(e.which == 13)
			{
				send_direct_msg();
				e.preventDefault();
				return false;
			}
		});
		$("#user_chat_submit").click(function(){
			send_direct_msg();
			return false;
		});
		
		// set username
		$("#name_form input").keypress(function(e){
			if(e.which == 13)
			{
				send_name();
				e.preventDefault();
				return false;
			}
		});
		$("#name_submit").click(function(){
			send_name();
			return false;
		});
		
		// files
		$("#file_add_form input").keypress(function(e){
			if(e.which == 13)
			{
				send_file_add();
				e.preventDefault();
				return false;
			}
		});
		$("#file_add_submit").click(function(){
			send_file_add();
			return false;
		});
	}
	
	qaul_initialized = true;
	init_config();
}

function init_config()
{
	// load configuration
	$.ajax({
		url:   "getconfig.json",
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			qaul_configure(data);
		}
	});
}

function qaul_configure(data)
{
	qaul_config = data;
	
	// set up everything
	if(qaul_config.c_quit) $(".c_quit").show();
	if(qaul_config.c_voip) $(".c_voip").show();
	if(qaul_config.c_debug) $(".c_debug").show();
	if(qaul_config.locale)
	{
		// load locale
		$.ajax({
			url:   "i18n/" +qaul_config.locale +".json",
			cache: false, // needed for IE
			dataType: "json",
			success: function(data){
				qaul_translate(data);
			}
		});
		
		// download language specific css
		if (document.createStyleSheet){
			document.createStyleSheet('i18n/' +qaul_config.locale +'.css');
		}
		else {
			$("head").append($("<link rel=\"stylesheet\" href=\"i18n/" +qaul_config.locale +".css\" type=\"text/css\" media=\"screen\" />"));
		}
	}
}

function qaul_translate(dictionary)
{
	$.i18n.setDictionary(dictionary);
	
	$("a.i18n_quit")._t('quit');
	$("a.i18n_addfile")._t('add file');
	$("label.i18n_choosename")._t('choose a nick name');
	
	$('input.i18n_choose').prop('value',$.i18n._('choose'));
	$('input.i18n_choose').prev('span').find('span.ui-btn-text')._t('choose');
	$('input.i18n_submit').prop('value',$.i18n._('submit'));
	$('input.i18n_submit').prev('span').find('span.ui-btn-text')._t('submit');
	$('input.i18n_add').prop('value',$.i18n._('add'));
	$('input.i18n_add').prev('span').find('span.ui-btn-text')._t('add');
}
function i18n_translate_button(id, txt)
{
	// TODO: check if button was already changed by jqm
}
function i18n_translate_submit(id, txt)
{
	// TODO: check if submit button was already changed by jqm
}


function init_chat()
{
	chat_initialized = true;
	
	// set timer
	updatetimer();
	
	// set name
	$.post('getname',function(data){
		user_name = decodeURIComponent(data);
		$('#chat_name').val(user_name);
	});

	// load files
	file_update();	
}

// ======================================================
// change views
// ------------------------------------------------------

function show_user(id, name, ip)
{
	user_last_id = 0;
	// open page
	$.mobile.changePage($("#page_user"),"slide");
	// set page
	$("#page_user_name").empty().append(name);
	$("#page_user_name").attr("user_id",id);
	$("#user_chat_name").val(name);
	$("#user_chat_ip").val(ip);
	$("#page_user_msgs").empty();
	$("#page_user_files").empty();
	// load messages
	get_user_msgs();
    // get info from remote user
	var path = "http://" +ip +":8081/pub_info.json";
    $.jsonp({
      url: path,
      callback: "abc",
      data: {},
      success: function(data) {
			$.each(data.files, function(i,item){
				var file = "<div class=\"file\">";
				file    += file_suffix2icon(item.suffix);
				file    += file_button_schedule(item.hash, item.suffix, item.size, item.description, name, ip);
				file    += "<div class=\"filename\">" +format_msg_txt(item.description) +"</div>";
				file    += "<div class=\"filemeta\"><span class=\"suffix\">" +item.suffix +"</span> " +file_filesize(item.size) +" ";
				file    += '<abbr class="timeago" title="' +item.time +'">' +time2str(item.time) +'</abbr>';
				file    += "</div>";
				file    += "</div>";
				var myfile = $("#page_user_files").append(file);
				myfile.trigger('create');
			});
      },
      error: function(d,msg) {
          // put error info
		  // create dialog
		  $.mobile.changePage($("#page_dialog"),{role:"dialog"});
      }
    });
}

function show_tag(tag)
{
	tag_last_id = 0;
	tag_name = tag;
	// open page
	$.mobile.changePage($("#page_tag"),"slide");
	// set page
	$("#page_tag_name").empty().append(tag);
	$("#page_tag_msgs").empty();
	// load messages
	get_tag_msgs();
	
	return true;
}

// invoked before a new page will load
function onPageBeforeChange(event, data)
{
	removeIFrame();
}

// invoked after a new page was loaded
function onPageChange(event, data) 
{
	// create iFrame
	createIFrame();
	
	// actualize footer
	update_footer();
	
	// send page id to app
	update_pageid();
}    

function update_pageid()
{
	if(qaul_initialized && $.mobile.activePage.attr("id") != "page_loading" && !/page_config*/.test($.mobile.activePage.attr("id")))
	{
		$.get('setpagename?p=' +$.mobile.activePage.attr("id") +'&e=1');
	}
}

function quit_qaul()
{
	$.ajax({
		  url: "quit",
		  cache: false, // needed for IE
		  dataType: "json",
		  success: function(data){
			  $.mobile.changePage('#page_goodbye');
		  }
		});
	//return false;
}

// ======================================================
// Chat
// ------------------------------------------------------

function format_msg_txt(msg)
{
	// @user
	msg = msg.replace(/(^|\s)(@[^\s]+)/g,"$1<a href=\"\" class=\"user\">$2</a>");
	// #tags
	msg = msg.replace(/(^|\s)(#[^\s]+)/g,"$1<a href=\"#page_tag\" onClick=\"javascript:show_tag('$2');\" class=\"tag\">$2</a>");
	// files
	//msg = msg.replace(/(^|\s)([a-zA-Z0-9]{40}\.([a-zA-Z0-9]{1,5}))/g,"$1<a href=\"\" class=\"file\">[$3]</a>");
	// emails
	msg = msg.replace(/[^\s]+@[^\s]{2,}\.[^\s]{2,}/g,"<a href=\"mailto:$&\" class=\"mail\">$&</a>");	
	// links
	msg = msg.replace(/http:\/\/([^\s]{5,})/g,"<a href=\"$&\" target=\"_blank\" class=\"link\">$1</a>");
	// emoticons
	msg = msg.replace(/(^|\s):-?\)/g,"$1<img src=\"images/emo_smile.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s):-?\(/g,"$1<img src=\"images/emo_sad.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s):-?D/g,"$1<img src=\"images/emo_lough.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s):-?o/g,"$1<img src=\"images/emo_astonishing.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s);-?\)/g,"$1<img src=\"images/emo_joking.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s):-?P/g,"$1<img src=\"images/emo_tongue.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s)8-?\)/g,"$1<img src=\"images/emo_cool.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s):-?\//g,"$1<img src=\"images/emo_embarassed.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s):-?\|/g,"$1<img src=\"images/emo_confused.png\" class=\"emo\" />");
	msg = msg.replace(/(^|\s):-?@/g,"$1<img src=\"images/emo_shouting.png\" class=\"emo\" />");
	return msg;
}

function format_msg_file(msg, desc, name, ip)
{
	// files
	desc = desc.replace(/(^|\s)([a-zA-Z0-9]{40}\.[a-zA-Z0-9]{1,5})/g,"$1");
	desc = $.trim(desc);
	// with button
	var button = "";
	msg = msg.replace(/(^|\s)([a-zA-Z0-9]{40})\.([a-zA-Z0-9]{1,5})/g, function(a,b,c,d){
				button = file_button_schedule( c, d, 0, desc, name, ip);
				return "<span class=\"suffix\">" +d +"</span> " +b;
		});
	return {"msg":msg,"button":button};
}

function format_msg(item)
{
	// format message
	var formated = format_msg_file(format_msg_txt(item.msg), item.msg, item.name, item.ip);
	
	// create html
	var msg = '<div id="msg_' +item.id +'" class="msg msg_' +item.type  +'">';
	msg += formated.button;
	msg += '<div class="msg_time"><abbr class="timeago" id="abbr_msg_' +item.id +'" title="' +item.time +'">' +time2str(item.time) +'</abbr></div>';
	// from
	if(item.type < 10)
	{
		msg += '<div class="sender"><a href="#page_user" onClick="javascript:show_user(' 
				+item.id +',\'' +item.name +'\',\'' +item.ip 
				+'\');">' +item.name +'</a></div>';
	}
	else 
	{
		msg += '<div class="sender">' +user_name +'</div>';
	}
	// message
	msg += '<div class="message">' +formated.msg +'</div>';
	msg += '</div>';
	return msg;
}

function insert_msg(insert, item, inverse)
{
	var new_item = format_msg(item);
	if(!qaul_config.is_mobile) new_item = $(new_item).hide();
	var myitem;
	if(inverse) myitem = insert.append(new_item);
	else myitem = insert.prepend(new_item);
	myitem.trigger('create');
	if(!qaul_config.is_mobile)
	{
		new_item.slideDown().fadeIn('slow');
	}
	insert.children("div.msg:gt(" +qaul_config.msg_max +")").remove();
}

function send_msg()
{
	$.post(
			"sendmsg",
			{ "t": 11, "m": msg.val(), "n": user_name, "e":1},
			function(){
				insert_msg(chat, {id:0,type:11,name:user_name,msg:msg.val(),time:isoDateString(new Date())});
				msg.val('');
			}
		).error(function(){
			// show alert
			$.mobile.changePage($("#page_dialog"),{role:"dialog"});
		});
};

function send_tag_msg()
{
	$.post(
			"sendmsg",
			{"t": 11, "m": $("#tag_chat_msg").val(), "n": user_name, "e":1},
			function(){
				insert_msg($("#page_tag_msgs"), {id:0,type:11,name:user_name,msg:$("#tag_chat_msg").val(),time:isoDateString(new Date())});
				insert_msg(chat, {id:0,type:11,name:user_name,msg:$("#tag_chat_msg").val(),time:isoDateString(new Date())});
				$("#tag_chat_msg").val('');
			}
		).error(function(){
			// show alert
			$.mobile.changePage($("#page_dialog"),{role:"dialog"});
		});
}

function send_direct_msg()
{
    // set loading info
    //$.mobile.pageLoading();

    // send data to remote user
    $.jsonp({
      url: "http://" +$("#user_chat_ip").val() +":8081/pub_msg?",
      callback: "abc",
      data: {
			"n": user_name,
			"m": $("#user_chat_msg").val(),
			"e": 1
      },
      success: function(userProfile) {
          	// clear loading info
          	//$.mobile.pageLoading(true);
          	
          	// send message to db
			$.post(
					'sendmsg',
					{"t": 12, "m": $("#user_chat_msg").val(), "n": user_name, "e":1}
				);
          	// put message into chat
			insert_msg($("#page_user_msgs"), {id:0,type:12,name:user_name,to:$("#user_chat_name").val(),msg:$("#user_chat_msg").val(),time:isoDateString(new Date())});
          	insert_msg(chat, {id:0,type:12,name:user_name,to:$("#user_chat_name").val(),msg:$("#user_chat_msg").val(),time:isoDateString(new Date())});
            // clear message input
            $("#user_chat_msg").val('');
      },
      error: function(d,msg) {
          // clear loading info
          //$.mobile.pageLoading(true);
          
          // put error info
		  // create dialog
		  $.mobile.changePage($("#page_dialog"),{role:"dialog"});
      }
    });
}

function get_msgs()
{
	var path = "getmsgs.json?t=1";
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			$.each(data.messages, function(i,item){
				insert_msg(chat, item);
			})
		} 
	});
}

function get_user_msgs()
{
	var path = 'getmsgs.json?t=5&id=' +user_last_id +'&v=' +encodeURIComponent($("#user_chat_name").val()) +'&e=1';
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			var inverse = false;
			if(user_last_id == 0) inverse = true;
			$.each(data.messages, function(i,item){
					if(item.id > user_last_id) user_last_id = item.id;
					insert_msg($("#page_user_msgs"), item, inverse);
			})
		} 
	});
}

function get_tag_msgs()
{
	var path = 'getmsgs.json?t=6&id=' +tag_last_id +'&v=' +encodeURIComponent(tag_name) +'&e=1';
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			var inverse = false;
			if(tag_last_id == 0) inverse = true;
			$.each(data.messages, function(i,item){
					if(item.id > tag_last_id) tag_last_id = item.id;
					insert_msg($("#page_tag_msgs"), item, inverse);
			})
		} 
	});
}

function time2str(timestr)
{
	return timestr.substr(11,5);
}

var updatetimer=function()
{
	if($.mobile.activePage.attr("id")=="page_chat")
	{
		get_msgs();
	}
	else if($.mobile.activePage.attr("id")=="page_users")
	{
		get_users();
	}
	else if($.mobile.activePage.attr("id")=="page_user")
	{
		get_user_msgs();
	}
	else if($.mobile.activePage.attr("id")=="page_tag")
	{
		get_tag_msgs();
	}
	else if($.mobile.activePage.attr("id")=="page_file")
	{
		file_update();
	}
	
	setTimeout(function(){updatetimer();},3000);
};

// ======================================================
// files
// ------------------------------------------------------
function show_page_file()
{
	file_update();
	$.mobile.changePage($("#page_file"));
}

function send_file_add()
{
	// check if file was selected
	if($("#file_add_path").val() == "")
	{
		// open file select again
		open_filepicker();
		return;
	}
	
	// validate entries
	var advertise = ($("#file_add_advertise").attr('checked'))? 1 : 0;
	
	// send it to webserver
	$.ajax({
			type:'POST',
			url:"file_add.json",
			data:{"p": $("#file_add_path").val(), "m": $("#file_add_msg").val(), "a": advertise, "e": 1},
			cache: false, // needed for IE
			success: function(data){
				// TODO: insert message
				// configure message
				//var message = data.hash +"." +data.suffix +" " +$("#file_add_msg").val();
				//insert_msg(chat, {id:0,type:11,name:user_name,msg:message});
				// cleanup
				$("#file_add_msg").val('');
				$("#file_add_advertise").attr('checked',true).checkboxradio("refresh");
				$("#file_add_path").val('');
				$("#file_add_filename").empty();
				$("#file_add_addbutton .ui-btn-text").text("choose file");
				$("#file_add_addbutton .ui-icon").addClass("ui-icon-plus").removeClass("ui-icon-refresh");
				
				// go to file page
				show_page_file();
			},
			dataType:"json"
	}).error(function(){
			// show alert
			$.mobile.changePage($("#page_dialog"),{role:"dialog"});
	});
}

function show_addfile_page()
{
	// reset message field
	$("#file_add_msg").val('');
	// show page
	$.mobile.changePage($("#page_file_add"));
	open_filepicker();
}

function open_filepicker()
{
	// send message / open socket to show filepicker
	var path = "file_pick.json";
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			// set timer to check if a file was picked
			setTimeout(function(){filepickertimer();},1000);
		} 
	}).error(function(){
		// show alert
		$.mobile.changePage($("#page_dialog"),{role:"dialog"});
	});
}

function open_file(filename)
{
	// send message / open socket to show filepicker
	var path = "file_open.json?f=" +filename;
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			// success
		} 
	}).error(function(){
		// show alert
		$.mobile.changePage($("#page_dialog"),{role:"dialog"});
	});
}

function file_delete(id)
{
	// delete file by id
	var path = "file_delete.json?id=" +id;
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			// reload files
			file_update();
		} 
	}).error(function(){
		// show alert
		$.mobile.changePage($("#page_dialog"),{role:"dialog"});
	});
}

function file_advertise(hash, suffix, size, description)
{
	// open chat with filled in message
	msg.val(hash +"." +suffix +" " +description);
	$.mobile.changePage($("#page_chat"),{});
}

function file_suffix2icon(suffix)
{
	return "<img src=\"images/" +file_suffix2filetype(suffix) +"_64.png\" class=\"fileicon64\">";
}

function file_suffix2filetype(suffix)
{
	var type;
	if(suffix.match(/^((jpe?g)|(png)|(gif)|(bmp)|(tiff?)|(raw)|(svg))$/i)) type = "f_img";
	else if(suffix.match(/^((mov)|(3gpp?)|(avi)|(mpg)|(mp4)|(m4v)|(wmv)|(flv))$/i)) type = "f_mov";
	else if(suffix.match(/^((mp3)|(wav)|(ogg)|(aiff?)|(m4a)|(m4p))$/i)) type = "f_sound";
	else if(suffix.match(/^((pdf))$/i)) type = "f_pdf";
	else if(suffix.match(/^((txt)|(rtf)|(html?)|(docx?)|(xls)|(xml)|(ppt)|(odt))$/i)) type = "f_txt";
	else if(suffix.match(/^((zip)|(tar)|(gz))$/i)) type = "f_zip";
	else type = "f_file";
	return type;
}

function file_button_schedule(hash, suffix, size, description, name, ip)
{
	var button = "";
	if(file_check(hash, suffix)) button += "<div class=\"msg_filebutton " +hash +suffix +" " +file_suffix2filetype(suffix) +"\"><a href=\"#page_file\" class=\"" +file_suffix2filetype(suffix) +"\"><img src=\"images/f_success_64.png\"/></a></div>";
	else button += "<div class=\"msg_filebutton " +hash +suffix +"\"><a href=\"#\" onClick=\"javascript:file_schedule('" +hash +"','" +suffix +"','" +description +"','" +size +"','" +ip +"','" +name +"')\" class=\"" +file_suffix2filetype(suffix) +"\"><img src=\"images/f_add_64.png\"/></a></div>";
	return button;
/*
	// current solution
	var button = "";
	if(file_check(hash, suffix)) button += "<div class=\"filebutton " +hash +suffix +"\"><a href=\"#page_file\" data-role=\"button\" data-icon=\"check\" data-iconpos=\"notext\" class=\"filebutton\">file scheduled</a></div>";
	else button += "<div class=\"filebutton " +hash +suffix +"\"><a href=\"#\" onClick=\"javascript:file_schedule('" +hash +"','" +suffix +"','" +description +"','" +size +"','" +ip +"','" +name +"')\" data-role=\"button\" data-icon=\"plus\" data-iconpos=\"notext\" class=\"filebutton\">download</a></div>";
	return button;
*/
}

function file_button_deactivate(hash, suffix)
{
	var button = "<img src=\"images/f_success_64.png\"/>";
	var a = $("div." +hash +suffix +" a");
	a.prop("onclick", null);
	a.prop("href", "#page_file");
	a.empty().append(button);
	//var button = "<a href=\"#page_file\" class=\"filebutton\">file scheduled</a>";
	//$("div." +hash +suffix).empty().append(button).trigger('create');
}

function file_check(hash, suffix)
{
	var i;
	for(i=0; i < qaulfiles.length; i++)
	{
		if(qaulfiles[i].status > -2 && qaulfiles[i].suffix == suffix && qaulfiles[i].hash == hash) return true;
	}
	return false;
}

function file_update()
{
	var path = "file_list.json?fe=" +qaulfileevent +"&e=1";
	var files = $("#page_file_list");
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			var old_event = qaulfileevent;
			$.each(data.files, function(i,item){
				file_update_check(item);
				if(item.event > qaulfileevent) qaulfileevent = item.event;
			});
		} 
	}).error(function(){
		// show alert
		$.mobile.changePage($("#page_dialog"),{role:"dialog"});
	});
}

// update file list & entries
function file_update_check(item)
{
	var exists = false;
	var i;
	for(i=0; i < qaulfiles.length; i++)
	{
		if(qaulfiles[i].id == item.id)
		{
			// update entry
			exists = true;
			// delete entry
			if(item.status == -2) 
			{
				$("#file_" +item.id).remove();
				//qaulfiles.splice(i,1);
				//break;
			}
			// download failed
			else if(item.status == -1)
			{
				$("#file_" +item.id).removeClass("scheduled downloading").addClass("failed");
				$("#file_" +item.id +" .fileicon64").attr("src","images/f_failed_64.png");
				$("#file_bar_" +item.id).remove();
			}
			// update progress bar
			else if(item.status == 1)
			{
				$("#file_bar_" +item.id).progressBar(item.downloaded);
			}
			// file sucessfully downloaded
			else if(item.status == 2)
			{
				if(qaulfiles[i].status <= 1)
				{
					$("#file_" +item.id).removeClass("scheduled downloading");
					$("#file_bar_" +item.id).progressBar(100);
					// add open file link
					var filename = item.hash;
					if(item.suffix != "") filename += "." +item.suffix;
					$("#file_" +item.id +" img.fileicon64").wrap("<a href=\"#\" onClick=\"javascript:open_file('" +filename +"')\"></a>");
					// add readvertise button
					var button = "<a href=\"#\" onClick=\"javascript:file_advertise('" +item.hash +"','" +item.suffix +"','" +item.size +"','" +item.description +"')\" class=\"filebutton\"><img src=\"images/b_advertise.png\" alt=\"advertise\" /></a>";
					$("#file_" +item.id +" a.filebutton").after(button);
					//$("#file_" +item.id).trigger('create');
				}
			}
			qaulfiles[i] = item;
		}
	}
	
	// add file if not existing
	if(!exists)
	{
		qaulfiles.push(item);
		var htmlitem = file_create_html(item);
		var myitem = $("#page_file_list").prepend(htmlitem);
		myitem.trigger('create');
		//htmlitem.slideDown().fadeIn('slow');
		var percent = 0;
		if(item.status == 1) percent = item.downloaded;
		myitem.find("#file_bar_" +item.id).progressBar(percent,{barImage:'images/progressbg_black.gif'});
		
		// deactivate schedule buttons
		file_button_deactivate(item.hash, item.suffix);
	}
}

function file_create_html(item)
{
	var filename = item.hash;
	if(item.suffix.length > 0) filename += "." +item.suffix;
	var fileclass = "";
	if(item.status == 0) fileclass = "scheduled";
	else if(item.status == 1) fileclass = "downloading";
	else if(item.status < 0) fileclass = "failed";
	var percent = 0;
	if(item.status == 1) percent = item.downloaded;
	var file = "<div class=\"file " +fileclass +"\" id=\"file_" +item.id +"\">";
	if(item.status >= 2) 
		file += "<a href=\"#\" onClick=\"javascript:open_file('" +filename +"')\">";
	if(item.status < 0) 
		file += "<img src=\"images/f_failed_64.png\" class=\"fileicon64\">";
	else
		file += file_suffix2icon(item.suffix);
	if(item.status >= 2) file += "</a>";
	file     += "<a href=\"#\" onClick=\"javascript:file_delete('" +item.id +"')\" class=\"filebutton\"><img src=\"images/b_delete.png\" alt=\"delete\" /></a>";
	if(item.status >= 2) 
		file += "<a href=\"#\" onClick=\"javascript:file_advertise('" +item.hash +"','" +item.suffix +"','" +item.size +"','" +item.description +"')\" class=\"filebutton\"><img src=\"images/b_advertise.png\" alt=\"advertise\" /></a>";
	file     += "<div class=\"filename\">" +format_msg_txt(item.description) +"</div>";
	if(item.status == 0 || item.status == 1)
		file += "<div class=\"fileprogress\"><span class=\"progressBar\" id=\"file_bar_" +item.id +"\">" +percent +"%</span></div>";
	file     += "<div class=\"filemeta\"><span class=\"suffix\">" +item.suffix +"</span> " +file_filesize(item.size) +" " ;
	file     += '<abbr class="timeago" id="abbr_msg_' +item.id +'" title="' +item.time +'">' +time2str(item.time) +'</abbr>';
	file     += "</div>";
	file     += "</div>";
	return file;
}

var filepickertimer=function()
{
	// check if file was selected
	var path = "file_pickcheck.json";
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			if($.mobile.activePage.attr('id')=="page_file_add")
			{
				if(data.picked == 0); // picking was canceld
				else if(data.picked == 2)
				{
					// display picked file
					$("#file_add_path").val(data.path);
					$("#file_add_filename").text(data.name);
					
					// take filename as message if message is empty
					if($("#file_add_msg").val()=="")
					{
						var name = data.name;
						name = name.replace(/\.[a-zA-Z0-9]+$/g,"");
						$("#file_add_msg").val(name.replace(/[._-]+/g," "));
					}
				}
				else setTimeout(function(){filepickertimer();},400);
			}
		} 
	}).error(function(){
			if($.mobile.activePage.attr('id')=="page_file_add")
			{
				// show alert
				//alert("error filepickertimer");
				setTimeout(function(){filepickertimer();},400);
			}
	});	
};

var loadingtimer=function()
{
	// check if file was selected
	var path = "loading.json";
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			if($.mobile.activePage.attr('id')=="page_loading")
			{
				if(data.change == 1) // change page
				{
					// initialize chat
					if(data.page == "#page_chat" && !chat_initialized) init_chat();
					// display page
					$.mobile.changePage($(data.page));
				}
				else setTimeout(function(){loadingtimer();},500);
			}
		} 
	}).error(function(){
			if($.mobile.activePage.attr('id')=="page_loading")
			{
				setTimeout(function(){loadingtimer();},500);
			}
	});	
};

function file_schedule(hash, suffix, description, size, ip, name)
{
	// send message / open socket to show filepicker
	var path = "file_schedule.json";
	$.ajax({
		type: 'POST',
		url:   path,
		data:  {"hash": hash, "suffix": suffix, "description": description, "size": size, "ip": ip, "name": name, "e":1},
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			// go to file page
			show_page_file();
		} 
	}).error(function(){
		// show alert
		$.mobile.changePage($("#page_dialog"),{role:"dialog"});
	});
	
}

function file_filesize(size)
{
	var str = "";
	if(size > 10000000000) str += (Math.round(size/1000000000)) +"GB";
	else if(size > 1000000000) str += (Math.round(size/100000000)/10) +"GB";
	else if(size > 10000000) str += (Math.round(size/1000000)) +"MB";
	else if(size > 1000000) str += (Math.round(size/100000)/10) +"MB";
	else if(size > 10000) str += (Math.round(size/1000)) +"KB";
	else if(size > 1000) str += (Math.round(size/100)/10) +"KB";
	else str += "1KB";
	return str;
}

function isoDateString(d)
{
	function pad(n){return n<10 ? '0'+n : n};
	
	return d.getUTCFullYear()+'-'
      + pad(d.getUTCMonth()+1)+'-'
      + pad(d.getUTCDate())+'T'
      + pad(d.getUTCHours())+':'
      + pad(d.getUTCMinutes())+':'
      + pad(d.getUTCSeconds())+'Z';
}

// ======================================================
// configuration
// ------------------------------------------------------

function send_locale()
{
	// send locale
	$.post(
			'setlocale',
			{"l": $("input[name='l']:checked").val(), "e":1},
			function(data){
				// forward to loading
				$.mobile.changePage($("#page_loading"));
				// set timer to check which page to load
				setTimeout(function(){loadingtimer();},1000);
				// update configuration
				init_config();
		});
};

function send_name()
{
	// send user name
	$.post(
			'setname',
			{"n": $("#name_name").val(), "e":1},
			function(data){
				// forward to chat
				$.mobile.changePage($("#page_loading"));
				// set timer to check which page to load
				setTimeout(function(){loadingtimer();},1000);
		});
};

// ======================================================
// Users
// ------------------------------------------------------

function get_users()
{
	var path = "getusers.json";
	$.ajax({
		url:   path,
		cache: false, // needed for IE
		dataType: "json",
		success: function(data) {
			users_append(data);
		} 
	});
};

function users_append(data)
{
	var items = [];
	users.empty();
	$.each(data.users, function(i,item){
		$("<li></li>")
            .prop('id',item.id)
            .html('<a href="javascript:show_user(' +item.id +',\'' +item.name +'\',\'' +item.ip 
						+'\')">' +item.name +'</a>'
						//+'<a href="javascript:alert(\'add user to favorites\');" data-icon="plus">add</a>'
						)
			//.text(item.name)
            //.insertAfter($("#users_divider"));
            .prependTo(users)
	});
	$("#users").listview("refresh"); // This line now updates the listview
}

function set_usercount(nodes, users)
{
	node_count = nodes;
	user_count = users;
	update_footer();
}
function update_footer()
{
	// TODO: write notification into footer
	//$("#" +$.mobile.activePage.attr("id") +" .i_users .ui-btn-text").text(user_count +" (" +node_count +")");
}

function user_changetofiles(active)
{
	$("#page_user_tab_chat").hide();
	$("#page_user_tab_files").show();
}

function user_changetochat(active)
{
	$("#page_user_tab_files").hide();
	$("#page_user_tab_chat").show();
}

// ======================================================
// Browser specific: Chrome
// ------------------------------------------------------
function createIFrame()
{
	// UI bugfix for chrome
	if(is_chrome)
	{
		var myheight;
		var myposition;
		var activepage = $.mobile.activePage.attr("id");
		var myfooter = $("#" +activepage +" .ui-footer-fixed");
		if(myfooter.length > 0)
		{
			myheight = myfooter.height();
			myposition = myfooter.position();
			// create iframe
			$.mobile.activePage.append('<div id="bugfix_footer" style="width:100%;height:' +myheight +'px;top:' +myposition.top +'px;left:0px;"><iframe src="blank.html" id="bugfix_footer_iframe" style="width:100%;' +myheight +';"></iframe></div>');
		}
		var myheader = $("#" +activepage +" .ui-header-fixed");
		if(myheader.length > 0)
		{
			myheight = myheader.height();
			myposition = myheader.position();
			// create iframe
			$.mobile.activePage.append('<div id="bugfix_header" style="width:100%;height:' +myheight +'px;top:' +myposition.top +'px;left:0px;"><iframe src="blank.html" id="bugfix_header_iframe" style="width:100%;' +myheight +';"></iframe></div>');
		}
	}
}
function removeIFrame()
{
	// UI bugfix for chrome
	if(is_chrome)
	{
		$("#bugfix_footer").remove();
		$("#bugfix_header").remove();
	}	
}

//======================================================
//debug
//------------------------------------------------------
function debugScreenSize(w, h)
{
	window.open(document.URL, w +"_" +h, "width="+w +",height="+h +",status=no,location=no");
}

//-----------------------------------------------------

alert('survived');
//$(document).ready(my_init());
$(init_start);
