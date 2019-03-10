<?php
$server = '98.250.3.81';
$username   = 'webuser';
$password   = 'A2FGH76B90JC756';
$database   = 'cs341';
 
if(!mysql_connect($server, $username,  $password))
{
	echo("Failed to select");
    exit('Error: could not establish database connection');
}
//if(!mysql_select_db($database)
//{
	//echo("Could not select");)
    //exit('Error: could not select the database');
//}
?>