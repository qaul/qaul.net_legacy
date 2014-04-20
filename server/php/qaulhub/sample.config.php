<?php

/**
 * your server configuration of your data base and twitter account
 */

/// set internal encoding to UTF-8 which is needed for qaul app
mb_internal_encoding("UTF-8");

/// display or hide debug messages
function get_debug()
{
	return true;
}


/// autoloading of all classes
require_once(dirname(__FILE__) .'/../vendor/autoload.php');
use Propel\Runtime\Propel;
use Propel\Runtime\Connection\ConnectionManagerSingle;
$serviceContainer = Propel::getServiceContainer();
$serviceContainer->setAdapterClass('qaulhub', 'mysql');
$manager = new ConnectionManagerSingle();
$manager->setConfiguration(array (
  'dsn'      => 'mysql:host=localhost;dbname=qaulhub',
  'user'     => 'DBuser',
  'password' => 'DBpassword',
  'settings' =>
  array (
    'charset' => 'utf8',
  ),
));
$serviceContainer->setConnectionManager('qaulhub', $manager);


/// twitter account keys
$twitter_settings = array(
    'consumer_key' => "API key",
    'consumer_secret' => "API secret",
    'oauth_access_token' => "Access token",
    'oauth_access_token_secret' => "Access token secret"
);


/// qaul configurations
$qaul_settings = array(
	"ip" => "10.67.33.81",
	"name" => "i_qaul",
	"domain" => "i.qaul.net",
	"download_folder" => "/PATH/TO/YOUR/QAUL/DOWNLOAD/FOLDER/",
	//"twitter_search_language" => "en",
	"twitter_search_term" => "#HASHTAG"
);

function get_qaul_setting($key)
{
	global $qaul_settings;
	if(isset($qaul_settings[$key]))
		return $qaul_settings[$key];
	
	return NULL;
}

