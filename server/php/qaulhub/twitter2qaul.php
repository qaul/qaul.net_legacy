<?php
/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * Search for Twitter messages for this twitter user and broadcast them via qaul app.
 * Search for Twitter messages for the specified hash tag and broadcast them via qaul app.
 *
 * Configure search term in $qaul_settings in config.php
 * $qaul_settings[twitter_search_term]     defines the term to search for in twitter
 *                                         messages. 
 * $qaul_settings[twitter_search_language] defines the language of the twitter messages
 *                                         to search for. 
 * 
 * Invoke this script regularly via cron or cli:
 * php twitter2qaul.php
 */

// include needed configuration & libraries
require_once('config.php');
require_once('twitter_functions.php');

// load twitter library
$twitter = new TwitterAPIExchange($twitter_settings);

// --------------------------------------------------
// search for messages sent to this twitter account
// --------------------------------------------------
// check for last message entry.
// if there is a message not older than 60 minutes
$type = 21;
$last = MsgQuery::create()
  			->filterByCreatedAt(array('min' => time() - 24 * 60 * 60))
  			->filterByType($type)
  			->orderByCreatedAt('desc')
  			->findOne();

// request messages for this user from twitter library
// https://dev.twitter.com/docs/api/1.1/get/statuses/mentions_timeline
$twitter_url = 'https://api.twitter.com/1.1/statuses/mentions_timeline.json';
$twitter_requestMethod = 'GET';
$twitter_getfield = '?count=10';
if($last)
	$twitter_getfield .= '&since_id=' .$last->getTwitterid();

if(get_debug())
	echo "mentions_timeline twitter_getfield: $twitter_getfield \n";

$txt = $twitter->setGetfield($twitter_getfield)
                              ->buildOauth($twitter_url, $twitter_requestMethod)
                              ->performRequest();
$data = json_decode($txt);

if(get_debug())
	var_dump($data);

// put messages into data base and send them via qaul app
if(isset($data->errors))
	echo "twitter error occured ... \n";
else
	twitter_process_messages($data, $type);



// --------------------------------------------------
// search twitter for messages containing the 
// configured search term.
// --------------------------------------------------
if(get_qaul_setting("twitter_search_term"))
{
	// check for last message entry.
	// if there is a message not older than 60 minutes
	$type = 22;
	$last = MsgQuery::create()
				->filterByCreatedAt(array('min' => time() - 24 * 60 * 60))
				->filterByType($type)
				->orderByCreatedAt('desc')
				->findOne();

	// request messages for hash tag from twitter library
	// https://dev.twitter.com/docs/api/1.1/get/search/tweets
	$twitter_url = 'https://api.twitter.com/1.1/search/tweets.json';
	$twitter_requestMethod = 'GET';
	$twitter_getfield = '?q=' .get_qaul_setting("twitter_search_term");
	if($last)
		$twitter_getfield .= '&since_id=' .$last->getTwitterid();

	// search only from this language
	if(get_qaul_setting("twitter_search_language"))
		$twitter_getfield .= '&lang=' .get_qaul_setting("twitter_search_language");
	
	// restrict number of results
	$twitter_getfield .= '&count=20';

	if(get_debug())
		echo "search/tweets twitter_getfield: $twitter_getfield \n";

	$txt = $twitter->setGetfield($twitter_getfield)
								  ->buildOauth($twitter_url, $twitter_requestMethod)
								  ->performRequest();            
	$data = json_decode($txt);

	if(get_debug())
		var_dump($data);

	// put messages into data base and send them via qaul app
	if(isset($data->statuses))
		twitter_process_messages($data->statuses, $type);
	elseif(isset($data->errors))
		echo "twitter error occured ... \n";
	else
		echo "unknown twitter error occured ... \n";
}
