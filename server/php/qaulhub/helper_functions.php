<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * helper functions for qaulhub app
 */


/**
 * protect @a message string
 * 
 * @retval the protected message string
 */
function protect_message($message)
{
	$str1 = str_replace("\n", " ", $message); 
	$str2 = str_replace('"', "'", $str1);
	$str3 = str_replace("\\", "/", $str2); 
	return $str3; 
}
