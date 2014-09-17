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
define('TREKSDIRURL', 'http://go3project.com/treks/');
define('BLOGURLPREFIX', 'http://go3project.com/network2/index.php/blogs/1/');

$db->select_db('socialengine');


/////////////////////
// EXECUTION START //
/////////////////////


//if( ! isset($_POST['Request']) die('{"Response":"Failure","Errors":["Nothing Sent"]}');

//if(strlen($_POST['Request'])==0) die('{"Response":"Failure","Errors":["Empty request"]}');

$request = json_decode($_POST['Request'], true);

if( ! $request) die('{"Response":"Failure","Errors":["Syntax unparsable"]}');

$errors = array();

if( ! isset($request['Email']) || strlen($request['Email'])==0) $errors[] = 'Email not specified';
//if( ! isset($request['Password']) || strlen($request['Password'])==0) $errors[] = 'Password not specified';
if( ! isset($request['KML']) || strlen($request['KML'])==0) $errors[] = 'No KML included';
if( ! isset($request['TrekName']) || strlen($request['TrekName'])==0) $errors[] = 'Trek name not specified';
if( ! isset($request['TrekCity']) || strlen($request['TrekCity'])==0) $errors[] = 'Trek city not specified';
if( ! isset($request['TrekDescription']) || strlen($request['TrekDescription'])==0) $errors[] = 'Trek description not specified';
if( ! isset($request['TrekStartTime']) || strlen($request['TrekStartTime'])==0) $errors[] = 'Trek start time not specified';
if( ! isset($request['TrekEndTime']) || strlen($request['TrekEndTime'])==0) $errors[] = 'Trek end time not specified';

if( ! empty($errors)) die('{"Response":"Failure","Errors":["'.implode('","', $errors).'"]}');		
//if( ! empty($errors)) die('{"Response":"Failure","Errors":["Errors in individual"]}');		//cjw

$res = $db->query("SELECT `user_id`, `displayname`, `email` FROM `engine4_users` WHERE `email`='".MI_sqlsan($request['Email'])."';");

if( ! $res->num_rows) die('{"Response":"Failure","Errors":["No user exists with the specified credentials"]}');

$r = $res->fetch_assoc();

$user = array(
	'ID' => $r['user_id'],
	'Email' => $r['email'],
	'DisplayName' => $r['displayname']
);

if ( ! isset($request['KML']) || strlen($request['KML'])==0) $errors[] = 'No KML included';

//if ( ! preg_match('%^[a-zA-Z0-9/+]*={0,2}$%', $request['KML'])) $errors[] = 'KML was not correctly base64-encoded';
//$str = substr($request['KML'],0,256);
//die('{"Response":"Failures","Errors":["'.$str.'"]}');

// Long strings seem to fail with php's base64decode, so this takes care of that issue.
$kml = '';
$len = strlen($request['KML']);

$str = str_replace(" ", "+", $request['KML'], $count);		//replace spaces with '+' signs (not sure why they aren't comming through!)
for ($i=0; $i < ceil($len/256); $i++) {
	$temp = base64_decode(substr($str,$i*256,256));
	if ($temp == false) die('{"Response":"Failure","Errors":["KML base64 decoding failed"]}');
	$kml .= $temp;
}


if(strlen($kml)==0) $errors[] = 'KML base64 decoding failed (empty return)';

if ( ! empty($errors)) die('{"Response":"Failure","Errors":["'.implode('","', $errors).'"]}');


$filename = 
	TREKSDIRECTORY .
	preg_replace('$[^a-zA-Z0-9]$', '', $user['Name']) . '-' .
	preg_replace('$[^a-zA-Z0-9]$', '', $request['TrekName']) . '-' .
	date('Y.m.d-H.i', $request['TrekStartTime']) . '.kml';

$i = 0;
/*while (file_exists($filename)) {
	$i++;
	if ($i == 1) $filename = substr($filename, 0, -4) . '_1.kml';
	elseif ($i < 10) $filename = substr($filename, 0, -6) . "_$i.kml";
	else die('{"Response":"Failure","Errors":["File could not be saved because more than 10 copies exist"]}');
}*/
//die('{"Response":"Failure","Errors":["water"]}');		//this is just to see what is happening for me to test...
//file_put_contents($filename, $kml);	//jason's
 

if(!file_put_contents($filename, $kml))
	die('{"Response":"Failure","Errors":["Could not write file to treks folder"]}');		//this is just to see what is happening for me to test...

//$title = MI_sqlsan(htmlentities("{$request['TrekName']} ({$request['TrekCity']})"));
/*$body = MI_sqlsan('<p>'.nl2br(htmlentities($request['TrekDescription'])) .
	"</p>\n<p>&nbsp;</p>\n" .
	"<p>Start time: THIS NEEDS TO BE FILLED WITH THE TIME, BUT WE'LL HAVE TO DEAL WITH TIMEZONE OFFSETS</p>\n" .
	"<p>End time: THIS NEEDS TO BE FILLED WITH THE TIME, BUT WE'LL HAVE TO DEAL WITH TIMEZONE OFFSETS</p>\n<p>&nbsp;</p>\n" .
	'<script src="http://www.gmodules.com/ig/ifr?url=http://dl.google.com/developers/maps/embedkmlgadget.xml&amp;up_kml_url=' .
	urlencode(TREKSDIRURL.substr($filename, strlen(TREKSDIRECTORY))) .
	'&amp;up_view_mode=maps&amp;up_earth_2d_fallback=0&amp;up_earth_fly_from_space=1&amp;up_earth_show_nav_controls=1&amp;up_earth_show_buildings=1&amp;up_earth_show_terrain=1&amp;up_earth_show_roads=1&amp;up_earth_show_borders=1&amp;up_earth_sphere=earth&amp;up_maps_zoom_out=0&amp;up_maps_default_type=map&amp;synd=open&amp;w=500&amp;h=400&amp;title=Trek%20Map&amp;border=%23ffffff%7C3px%2C1px+solid+%23999999&amp;output=js"></script>');
$date = date('Y-m-d H:i:s');*/
$title = MI_sqlsan(htmlentities("{$request['TrekName']}"));
$body = MI_sqlsan('<p>'.nl2br(htmlentities($request['TrekDescription'])) .
	"</p>\n<p>&nbsp;</p>\n" .
	//"<p>Start time:".$request['TrekStartTime']." </p>\n" .
	//"<p>End time: </p>\n<p>&nbsp;</p>\n" .
	'<iframe src="http://www.nearby.org.uk/google/fake-kmlgadget.html?up_kml_url='.			
	urlencode(TREKSDIRURL.substr($filename, strlen(TREKSDIRECTORY))) .
	'&up_view_mode=earth&up_lat=&up_lng=&up_zoom=&up_earth_fly_from_space=1&up_earth_show_nav_controls=1&up_earth_show_buildings=1&up_earth_show_terrain=1&up_earth_show_roads=1&up_earth_show_borders=1&up_earth_sphere=earth&up_maps_default_type=map;output=js" width="800" height="500"></iframe>');
	//'&amp;up_view_mode=maps&amp;up_earth_2d_fallback=0&amp;up_earth_fly_from_space=1&amp;up_earth_show_nav_controls=1&amp;up_earth_show_buildings=1&amp;up_earth_show_terrain=1&amp;up_earth_show_roads=1&amp;up_earth_show_borders=1&amp;up_earth_sphere=earth&amp;up_maps_zoom_out=0&amp;up_maps_default_type=map&amp;synd=open&amp;w=500&amp;h=400&amp;title=Trek%20Map&amp;border=%23ffffff%7C3px%2C1px+solid+%23999999"></iframe>');
$date = date('Y-m-d H:i:s');


$db->query(
	'INSERT INTO `engine4_blog_blogs`
		 (title,    body,    owner_type, owner_id,      category_id,       creation_date, modified_date)
	VALUES' .
		"('$title', '$body', 'user',     {$user['ID']}, ".TREKSCATEGORY.", '$date',       '$date');")
or die('{"Response":"Failure","Errors":["Insertion error: '.htmlentities($db->error).'"]}');

$blog_id = $db->insert_id;

// Make these blog posts visible to everyone (JP 2014/9/6)
$db->query(
	'INSERT INTO `engine4_authorization_allow`
		(resource_type, resource_id, action,    role,              value)
	VALUES'.
		"('blog',       $blog_id,    'view',    'registered',          1),".
		"('blog',       $blog_id,    'view',    'owner_network',       1),".
		"('blog',       $blog_id,    'view',    'owner_member_member', 1),".
		"('blog',       $blog_id,    'view',    'owner_member',        1),".
		"('blog',       $blog_id,    'view',    'everyone',            1),".
		"('blog',       $blog_id,    'comment', 'registered',          1),".
		"('blog',       $blog_id,    'comment', 'owner_network',       1),".
		"('blog',       $blog_id,    'comment', 'owner_member_member', 1),".
		"('blog',       $blog_id,    'comment', 'owner_member',        1),".
		"('blog',       $blog_id,    'comment', 'everyone',            1);"
);

$response = array(
	'Response' => 'Success',
	'Data' => array(
		'BlogURL' => BLOGURLPREFIX.$blog_id
	)
);

print(json_encode($response));

/////////////////
// END OF FILE //
/////////////////

?>
