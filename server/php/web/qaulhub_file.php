<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * Send a json file with recent files to the public qaulhub.html.
 * If @a id GET paramter is given, only files newer than that are sent, otherwise
 * all files are sent.
 * 
 * qaulhub_file.php?id=123
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
	$files = FileQuery::create()
  			->filterById(array('min' => $id))
  			->orderById('asc')
  			->limit(30)
  			->find();
}
else
{
	$files = FileQuery::create()
  			->orderById('desc')
  			->limit(150)
  			->find();
}

// send json to client
$first = true;
echo "{\"files\":[\n";
foreach($files as $file)
{
	if($first)
		$first = false;
	else
		echo ",\n";
	
	echo "{";
	
	echo '"id":' .$file->getId() .',';
	echo '"hash":"' .$file->getHash() .'",';
	echo '"suffix":"' .$file->getSuffix() .'",';
	echo '"size":' .$file->getSize() .',';
	echo '"description":"' .protect_message($file->getDescription()) .'",';
	echo '"time":"' .$file->getCreatedAt('Y-m-d H:i:s') .'",';
	echo '"status":4';
	
	echo "}";
}
echo "\n]}";
