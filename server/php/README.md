qaul.net VPN Server hub
========================

This server can bridge qaul.net networks together.
This is the web server part.

* qaulhub directory
  application logic of the web server.
  configure your server here.
* web directory
  Publicly accessible web directory for people accessing the VPN server from the Internet.
  Make this directory the access directory of your web server. 
  Copy everything from the www directory in it.
  Make the index.php file the default access page.
  

Cron Tasks
----------

The following files have to be invoked regularly, to do the background tasks. A good 
interval is once a minute.

    # edit your cron tasks
    crontab -e

    # automatically schedule advertised files in the qaul.net for download to the
    # web server.
    php PATH_TO_YOUR_HOST_DIRECTORY/qaulhub/file2db.php
    # send messages from qaul.net to twitter
    php PATH_TO_YOUR_HOST_DIRECTORY/qaulhub/qaul2twitter.php
    # send twitter messages to qaul.net
    php PATH_TO_YOUR_HOST_DIRECTORY/qaulhub/twitter2qaul.php


Composer
--------

Composer instructions

    # installing libraries for the first time
    php composer.phar install
    # updating libraries and installing new ones
    php composer.phar update


Data Base
---------

Data base instructions

    # create mysql data base
    mysqladmin -u root -p create DbName
    # build model
    vendor/propel/propel/bin/propel model:build
    # build sql
    vendor/propel/propel/bin/propel sql:build
    # load sql into data base
    vendor/propel/propel/bin/propel sql:insert
    # create autoloader for classes
    php composer.phar dump-autoload

