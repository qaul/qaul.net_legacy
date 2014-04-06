<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

require_once('config.php');


/**
 * check if message is a file advertisemet
 */
function file_check_msg($msg)
{
	if(preg_match('/(^|\s)([a-zA-Z0-9]{40})\.([a-zA-Z0-9]{1,5})/', $msg))
		return true;
		
	return false;
}

/**
 * schedule file for downloading at qaul app
 */
function file_schedule($message)
{
	// extract: hash, suffix, description
	if(preg_match('/(^|\s)([a-zA-Z0-9]{40})\.([a-zA-Z0-9]{1,5})(.*)/', $message->msg, $matches))
	{
		$url = 'http://localhost:8081/file_schedule.json';
		$post_data = array(
						'hash' => $matches[2], 
						'suffix' => $matches[3],
						'description' => trim($matches[4]),
						'size' => 0,
						'ip' => $message->ip,
						'name' => $message->name,
						'e' => 1
						);

		// use key 'http' even if you send the request to https://...
		$options = array(
			'http' => array(
				'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
				'method'  => 'POST',
				'content' => http_build_query($post_data),
			),
		);
		$context  = stream_context_create($options);
		$result = file_get_contents($url, false, $context);
		
		if(get_debug())
			var_dump($result);
	}
}
