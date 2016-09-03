<?php
	if(isset($_POST["temperature"]) && isset($_POST["humidity"])) {
		$t = $_POST["temperature"];
		$h = $_POST["humidity"];
		
		$connect = mysql_connect("db.example.com","user","password","db") or die('Could not connect: ' . mysql_error());
		mysql_select_db("db", $connect);

		$query = "INSERT INTO arduino_logger (temperature, humidity) VALUES ('" . $t . "', '" . $h . "')";
		$result = mysql_query($query, $connect) or die("SQL Error 1: " . mysql_error());
		
		mysql_close($connection);		
	};
?>