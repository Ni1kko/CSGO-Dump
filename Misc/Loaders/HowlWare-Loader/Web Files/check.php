<?php
$link = mysqli_connect("localhost", "krystalz_loader","password");
$database = mysqli_select_db($link, "krystalz_forums");

$user = $_GET['username'];
$password = $_GET['password'];
$hwid = $_GET['hwid'];
$tables = "mybb_users";
$token = $_GET['token'];

$sql = "SELECT * FROM ". $tables ." WHERE username = '". mysqli_real_escape_string($link,$user) ."'" ;
$result = $link->query($sql);
if ($result->num_rows > 0) {
    // Outputting the rows
    while($row = $result->fetch_assoc())
    {
        // Password encryption and check
        $password = $row['password'];
        $salt = $row['salt'];
        $plain_pass = $_GET['password'];
        $stored_pass = md5(md5($salt).md5($plain_pass));
        
		// Status Holders
		$passwordstatus;
		$hwidstatus;

        echo "{<br>";

        if($stored_pass != $row['password'])
        {
            echo "password: false<br>";
        }
        else
        {
            echo "password: true<br>";
        }

        echo "group: " . $row['usergroup'] . "<br>";
        echo "additionalgroups: " . $row['additionalgroups'] . "<br>";

        if (strlen($row['hwid']) > 1)
        {
            if ($hwid != $row['hwid'])
            {
                echo "hwid: false<br>";
            }
            else
            {
                echo "hwid: true<br>";
            }
        }
        else
        {
            $sql = "UPDATE ". $tables ." SET hwid='$hwid' WHERE username='$user'";
            if(mysqli_query($link, $sql))
            {
                echo "hwid: true<br>";
            }
            else
            {
                echo "hwid: false<br>";
            }
        }
		
		echo "token: " . $_GET['token'] . "<br>";
		
		echo "}";
		
    }
}
else
{
   echo "password: false";
}
?>