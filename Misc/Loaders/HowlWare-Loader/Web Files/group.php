<?php
$link = mysqli_connect("localhost", "krystalz_loader","password");
$database = mysqli_select_db($link, "krystalz_forums");

$user = $_GET['username'];
$tables = "mybb_users";

$sql = "SELECT * FROM ". $tables ." WHERE username = '". mysqli_real_escape_string($link,$user) ."'" ;
$result = $link->query($sql);
if ($result->num_rows > 0) {
    // Outputting the rows
    while($row = $result->fetch_assoc())
    {
        echo $row['usergroup'] . ",";
        echo $row['additionalgroups'];
    }
}  
?>