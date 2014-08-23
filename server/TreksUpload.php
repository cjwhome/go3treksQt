<?php

/////////////////////////
// TREKS UPLOAD SCRIPT //
/////////////////////////

// Created by JP on 2014-8-20
// Uploads Treks files to the server.

define('MI_dev_mode', true); define('MI_verbose', true);
define('MI_script_name', 'TreksUpload');
require(__DIR__.'/../MI.php');

define('TREKSCATEGORY', 20);  // Inserted as the "category" field on each blog post
define('TREKSDIRECTORY', '/var/www/treks/');  // Where treks KML is saved

$db->select_db('socialengine');


/////////////////////
// EXECUTION START //
/////////////////////

echo '<?xml version="1.0" encoding="UTF-8"?>';

if( ! isset($_GET['Request']) || strlen($_GET['Request'])==0) die('{"Response":"Failure","Errors":["Empty request"]}');

$request = json_decode($_GET['Request'], true);

if( ! $request) die('{"Response":"Failure","Errors":["Syntax unparsable"]}');

$errors = array();

if( ! isset($request['Username']) || strlen($request['Username'])==0) $errors[] = 'Username not specified';
if( ! isset($request['Password']) || strlen($request['Password'])==0) $errors[] = 'Password not specified';
if( ! isset($request['KML']) || strlen($request['KML'])==0) $errors[] = 'No KML included';
if( ! isset($request['TrekName']) || strlen($request['TrekName'])==0) $errors[] = 'Trek name not specified';
if( ! isset($request['TrekStartTime']) || strlen($request['TrekStartTime'])==0) $errors[] = 'Trek start time not specified';
if( ! isset($request['TrekEndTime']) || strlen($request['TrekEndTime'])==0) $errors[] = 'Trek end time not specified';

if( ! empty($errors)) die('{"Response":"Failure","Errors":["'.implode('","', $errors).'"]}');

$res = $db->query("SELECT `user_id`, `username`, `displayname`, `email` FROM `engine4_users` WHERE `username` = '".MI_sqlsan($request['Username'])."' AND `email`='".MI_sqlsan($request['Password'])."';");

if( ! $res->num_rows) die('{"Response":"Failure","Errors":["No user exists with the specified credentials"]}');

$r = $res->fetch_assoc();

$user = array(
	'ID' => $r['user_id'],
	'Name' => $r['username'],
	'DisplayName' => $r['displayname'],
	'Email' => $r['email']
);

if( ! isset($request['KML']) || strlen($request['KML'])==0) $errors[] = 'No KML included';

if ( ! preg_match('%^[a-zA-Z0-9/+]*={0,2}$%', $request['KML'])) $errors[] = 'KML was not correctly base64-encoded';

// Long strings seem to fail with php's base64decode, so this takes care of that issue.
$kml = '';
$len = strlen($request['KML']);
for ($i=0; $i < ceil(strlen($len)/256); $i++)
	$kml .= base64_decode(substr($request['KML'],$i*256,256));
//$kml = mb_convert_encoding( $request['KML'], 'UTF-8', 'BASE64');

if(strlen($kml)==0) $errors[] = 'KML base64 decoding failed';

if ( ! empty($errors)) die('{"Response":"Failure","Errors":["'.implode('","', $errors).'"]}');

$filename =
	TREKSDIRECTORY .
	preg_replace('$[^a-zA-Z0-9]$', '', $user['Name']) . '-' .
	preg_replace('$[^a-zA-Z0-9]$', '', $request['TrekName']) . '-' .
	date('Y.m.d-H.i', $request['TrekStartTime']) . '.kml';

$i = 0;
while (file_exists($filename)) {
	$i++;
	if ($i == 1) $filename = substr($filename, 0, -4) . '_1.kml';
	elseif ($i < 10) $filename = substr($filename, 0, -6) . "_$i.kml";
	else die('{"Response":"Failure","Errors":["File could not be saved because more than 10 copies exist"]}');
}

echo('FILE NOT SAVED BECAUSE YOU ARE IN DEBUG MODE\n<br>');
//file_put_contents($filename, $kml);





$response = array(
	'Response' => 'Success',
	'Data' => array(
		'BlogURL' => 'google.com'
	)
);

print(json_encode($response));

/////////////////
// END OF FILE //
/////////////////

?>
