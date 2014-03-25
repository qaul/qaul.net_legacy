<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * Check a locally running qaul app for newly downloaded files. 
 * Put the file information into data base and advertise it on twitter.
 * 
 * invoke this script regularly via cron or cli:
 * php file2db.php
 */

// include needed configuration & libraries
require_once('config.php');

// load json file update list from qaul app
// variable r (0=just updates, 1=all, 2=all and don't update gui_notify)
$json = file_get_contents('http://localhost:8081/file_list.json?r=0&e=1'); 
$data = json_decode($json, false);
if(get_debug())
	var_dump($data);

// loop through messages
foreach($data->files as $item)
{
	// check if has been successfully downloaded
	if($item->status >= 4)
	{
		// check if file already exists
		$file = FileQuery::create()
  			->filterByHash($item->hash)
  			->findOne();

		if(!$file)
		{
			// put file information into data base
			$file = new File();
			$file->setHash($item->hash);
			$file->setSuffix($item->suffix);
			$file->setDescription($item->description);
			$file->setSize($item->size);
			$file->setTime($item->time);
			$file->setStatus($item->status);
			$file->save();
		
			// advertise it via twitter
		}
	}
	// check if file has been delete
	else if($item->status == -2)
	{
		// delete file
		$file = FileQuery::create()
  			->filterByHash($item->hash)
  			->delete();
	}
}
