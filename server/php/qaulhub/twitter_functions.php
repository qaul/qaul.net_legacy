<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

require_once('config.php');

/**
 * loop through twitter messages, put them to database and send them to qaul app
 */
function twitter_process_messages($data, $type)
{
	foreach($data as $item)
	{
		// check if message already exists 
		$msg = MsgQuery::create()
  			->filterByTwitterid($item->id_str)
  			->findOne();
		
		if(!$msg)
		{
			// save message in data base
			$msg = new Msg();
			$msg->setType($type);
			$msg->setName($item->user->screen_name);
			$msg->setMsg($item->text);
			$msg->setIp(get_qaul_setting('ip'));
			$msg->setTime($item->created_at);
			$msg->setStatus(0);
			$msg->setTwitterid($item->id_str);
			$msg->save();
		
			// send message to qaul app
			twitter_send2qaul($msg);
		}
	}
}

/**
 * send message to qaul app
 */
function twitter_send2qaul($msg)
{
	$url = 'http://localhost:8081/sendmsg';
	$post_data = array(
					't' => 11, 
					'm' => twitter_message_string_byte($msg->getMsg(), $msg->getName()),
					'e' => 1
					);
	$content = http_build_query($post_data);

	// use key 'http' even if you send the request to https://...
	$options = array(
		'http' => array(
			'header'  => "Content-type: application/x-www-form-urlencoded\r\nAccept-Charset: UTF-8\r\n",
			'method'  => 'POST',
			'content' => $content,
		),
	);
	$context  = stream_context_create($options);
	$result = file_get_contents($url, false, $context);
	
	if(get_debug())
		echo "context: $content \n";
}

/**
 * creates a message string that includes the sender name in the message text. 
 * The string is not longer than 140 UTF-8 characters.
 * 
 * @retval message text
 */
function twitter_message_string_utf8($text, $name)
{
	$max = 140;
	$free = $max - mb_strlen($text);
	$name_len = mb_strlen($name);
	
	if($free < 0)
	{
		$msg_name = '';
		$text = mb_substr($text, 0, $max);
	}
	elseif($free < 3)
		$msg_name = '';
	elseif($free == 3)
		$msg_name = '[] ';
	elseif($free == 4)
		$msg_name = '[.] ';
	elseif($free == 5 && $name_len > 2)
		$msg_name = '[..] ';
	elseif($free < $name_len +3)
	{
		$name_part = mb_substr($name, 0, $free -5);
		$msg_name = '[' .$name_part .'..] ';
	}
	else
		$msg_name = '[' .$name .'] ';
	
	$message = $msg_name .$text;
	
	return $message;
}

/**
 * creates a message string that includes the sender name in the message text.
 * The string is not longer than 140 bytes.
 * 
 * @retval message text
 */
function twitter_message_string_byte($text, $name)
{
	$max = 140;
	$free = $max - strlen($text);
	$name_len = strlen($name);
	
	if($free < 0)
	{
		$msg_name = '';
		$text = mb_strcut($text, 0, $max);
	}
	elseif($free < 3)
		$msg_name = '';
	elseif($free == 3)
		$msg_name = '[] ';
	elseif($free == 4)
		$msg_name = '[.] ';
	elseif($free == 5 && $name_len > 2)
		$msg_name = '[..] ';
	elseif($free < $name_len +3)
	{
		$name_part = mb_strcut($name, 0, $free -5);
		$msg_name = '[' .$name_part .'..] ';
	}
	else
		$msg_name = '[' .$name .'] ';
	
	$message = $msg_name .$text;
	
	if(get_debug())
		var_dump($message);
	
	return $message;
}

/**
 * create a short file link.
 * 
 * @retval link string
 */
function twitter_file_link($file)
{
	$link = "http://" .get_qaul_setting('domain') ."/f/" .$file->id;
	return $link;
}
