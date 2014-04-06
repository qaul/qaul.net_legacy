<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * this scripts redirects the browser from a short link with file id to the file download.
 * 
 * invoke this script via web browser:
 * /f/file2db.php?id=123
 */

// include needed configuration & libraries
require_once('../../qaulhub/config.php');


// get id
$id = $_GET['id'];

// get file by id
$q = new FileQuery();
$file = $q->findPK($id);

if(!$file)
{
	// file was not found
	echo "file not found ...";
	exit();
}

header("Location: ../files/" .$file->getHash() ."." .$file->getSuffix());
exit();

/*
$path = get_qaul_setting('download_folder') .$file->getHash() ."." .$file->getSuffix();

if (file_exists($path) && is_readable($path))
{
	//header('Content-type: '.$mime);
	header('Content-length: '.filesize($path));
	$myfile = @ fopen($path, 'rb');
	if ($myfile)
	{
		fpassthru($myfile);
	}
}
exit();
*/

