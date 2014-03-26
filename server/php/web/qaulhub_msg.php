<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * Send a json file with recent messages to the public qaulhub.html.
 * If @a id GET paramter is given, only messages newer than that are sent, otherwise
 * the 30 most recent messages are sent.
 * 
 * qaulhub_msg.php?id=123
 */

// include needed configuration & libraries
require_once('../qaulhub/config.php');
require_once('../qaulhub/helper_functions.php');


// check get variables
if(isset($_GET['id']))
	$id = $_GET['id'];
else
	$id = 0;

// pull messages from data base
if($id)
{
	$msgs = MsgQuery::create()
  			->filterById(array('min' => $id +1))
  			->orderById('asc')
  			->limit(2)
  			->find();
}
else
{
	$msgs = MsgQuery::create()
  			->orderById('desc')
  			->limit(30)
  			->find();
}

// send json to client
$first = true;
echo '{"name":"' .get_qaul_setting('name') ."\",\n";
echo "\"messages\":[\n";
foreach($msgs as $msg)
{
	if($first)
		$first = false;
	else
		echo ",\n";
	
	echo "{";
/*
	echo '"id":' .$msg->getId() .',';
	echo '"type":' .$msg->getType() .',';
	echo '"name":"' .$msg->getName() .'",';
	echo '"ip":"' .$msg->getIp() .'",';
	echo '"msg":"' .protect_message($msg->getMsg()) .'",';
	echo '"time":"' .$msg->getCreatedAt('Y-m-d H:i:s') .'",';
	echo '"twitterid":"' .$msg->getTwitterid() .'"';
*/
	echo '"id":' .$msg->getId() .',';
	echo '"type":' .$msg->getType() .',';
	echo '"name":"' .$msg->getName() .'",';
	echo '"ip":"' .$msg->getIp() .'",';
	echo '"msg":"' .protect_message($msg->getMsg()) .'",';
	echo '"time":"' .$msg->getCreatedAt('Y-m-d H:i:s') .'",';
	echo '"twitterid":"' .$msg->getTwitterid() .'"';
	
	echo "}";
}
echo "\n]}";
