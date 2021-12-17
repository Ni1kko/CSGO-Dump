<?php
$link = mysqli_connect('localhost','root','');
$database = mysqli_select_db($link,'forums');

$user = $_GET['username'];
$password = $_GET['password'];

$sql = "SELECT * FROM mybb_users WHERE username = '". mysqli_real_escape_string($link,$user) ."'" ;
$result = $link->query($sql);

if ($result->num_rows > 0) {
	// Outputting the rows
	while($row = $result->fetch_assoc()) {
		
		$password = $row['password'];
		$salt = $row['salt'];
		$plain_pass = $_GET['password'];
		$stored_pass = md5(md5($salt).md5($plain_pass));
		
		function Redirect($url, $permanent = false)
		{
			if (headers_sent() === false)
			{
				header('Location: ' . $url, true, ($permanent === true) ? 301 : 302);
			}
		exit();
		}
		
		if($stored_pass == $row['password'])
		{
			Redirect('http://localhost/1.txt', false); // Infomation correct
		}
		else
		{
			Redirect('http://localhost/2.txt', false); // Wrong pass
		}
	}
} 
else
{
	Redirect('http://localhost/3.txt', false); // No user
}
?>

<html>
<head>
<title>Checking account</title>
<meta http-equiv="refresh" content="1" />
</head>

<body>
</body>