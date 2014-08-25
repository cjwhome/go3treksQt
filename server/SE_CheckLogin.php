<?php

////////////////////////
// LOGIN CHECK SCRIPT //
////////////////////////

// Created by JP on 2014-7-8
// Used by the DDA to determine whether or not login information is correct.

//define('MI_dev_mode', true); define('MI_verbose', true);
define('MI_script_name', 'SE_CheckLogin');
require(__DIR__.'/../MI.php');

$db->select_db('socialengine');

define('MIN_REQ_VERSION', 1.0);


/////////////////////
// EXECUTION START //
/////////////////////

if( ! isset($_POST['Request']) || strlen($_POST['Request'])==0) die('{"Response":"Failure","Errors":["Empty request"]}');

$request = json_decode($_POST['Request'], true);

if ( ! $request) die('{"Response":"Failure","Errors":["Syntax unparsable"]}');

$errors = array();

if ( ! isset($request['Username']) || strlen($request['Username'])==0) $errors[] = 'Username not specified';
if ( ! isset($request['Password']) || strlen($request['Password'])==0) $errors[] = 'Password not specified';

if ( ! empty($errors)) die('{"Response":"Failure","Errors":["'.implode('","', $errors).'"]}');

$res = $db->query("SELECT `user_id`, `username`, `displayname`, `email` FROM `engine4_users` WHERE `username` = '".MI_sqlsan($request['Username'])."' AND `email`='".MI_sqlsan($request['Password'])."';");

if( ! $res->num_rows) die('{"Response":"Failure","Errors":["No user exists with the specified credentials"]}');

$r = $res->fetch_assoc();

$response = array(
	'Response' => 'Success',
	'Data' => array(
		'UserID' => $r['user_id'],
		'Username' => $r['username'],
		'UserEmail' => $r['email'],
		'UserDisplayName' => $r['displayname'],
		'MinVersion' => MIN_REQ_VERSION,
		'CurrentTime' => time()
	)
);

print(json_encode($response));

/////////////////
// END OF FILE //
/////////////////

?>