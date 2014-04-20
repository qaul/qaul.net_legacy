<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * Check a locally running qaul app if new private messages or file advertisements arrived. 
 * Put these messages into data base and forward them to twitter.
 * Analyze file advertisements and schedule them in qaul app for downloading.
 * 
 * invoke this script regularly via cron or cli:
 * php msg2twitter.php
 */

// include needed configuration & libraries
require_once('config.php');
require_once('file_schedule.php');
require_once('twitter_functions.php');


// load json file with unchecked messages from qaul app
$json = file_get_contents('http://localhost:8081/getmsgs.json?t=1&e=1'); 
$data = json_decode($json, false);

// loop through messages
foreach($data->messages as $message)
{
	// check if message is connection
	if($message->type == 13 || $message->type == 3)
	{
		// don't do anything
	}
	// check if message is sent by this qaul app
	elseif($message->type == 11 || $message->type == 12)
	{
		// don't do anything
		// message was already added before sending it to the qaul app
	}
	// check if message is a file advertisment
	elseif(file_check_msg($message->msg))
	{
		// schedule file
		file_schedule($message);
	}
	// check if message is public messages
	elseif($message->type == 1)
	{
		// don't do anything
	}	
	// check if message is private message
	elseif($message->type == 2)
	{
		// put message into data base
		$msg = new Msg();
		$msg->setType($message->type);
		$msg->setName($message->name);
		$msg->setMsg($message->msg);
		$msg->setIp($message->ip);
		$msg->setTime($message->time);
		$msg->setStatus(0);
		$msg->save();
		
		// send message via twitter
		$txt = twitter_message_string_utf8($msg->getMsg(), $msg->getName());
		twitter_send2twitter($txt);
		
		// wait before sending the next message
		sleep(1);
	}
}
