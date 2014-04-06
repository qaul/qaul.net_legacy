qaul.net PHP Server Services
============================

This is the qaul web server part.

* qaulhub directory
  application logic of the web server.
  configure your server here.
* web directory
  Publicly accessible web directory for people accessing the VPN server from the Internet.
  Make this directory the access directory of your web server. 
  Copy everything from the www directory in it.
  Make the index.php file the default access page.


This software has run successfully on
* PHP 5.3, 5.4


Configure
---------

* Create and configure the following two files with your twitter key and data base 
  information: 
  qaulhub/config.php ,
  qaulhub/build.properties
* Create a symbolic link to your qaul app files download folder in the web directory:

    cd web
    ln -s PATH-TO-QAUL-APP/www/files files


Cron Tasks
----------

The following files have to be invoked regularly, to do the background tasks. A good 
interval is once a minute.

    # edit your cron tasks
    crontab -e

    # send twitter messages with links to downloadable files from qaul.net.
    php PATH_TO_YOUR_HOST_DIRECTORY/qaulhub/file2twitter.php
    # send messages from qaul.net to twitter and schedule advertised files for download.
    php PATH_TO_YOUR_HOST_DIRECTORY/qaulhub/msg2twitter.php
    # send twitter messages to qaul.net
    php PATH_TO_YOUR_HOST_DIRECTORY/qaulhub/twitter2qaul.php


Composer
--------

Composer instructions

    # installing libraries for the first time
    php composer.phar install
    # updating libraries and installing new ones
    php composer.phar update
    # create autoloader for classes
    php composer.phar dump-autoload


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

