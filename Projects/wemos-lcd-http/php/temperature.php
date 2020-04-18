<?php
$servername = "localhost";
$username = "grafana";
$password = "Isayenko!1";
$dbname = "zigbee2mqtt";

$degreeChar = chr(223);

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Bedroom
$sql = "select Temperature from Xiaomi_Temperature_Sensors where device like '%cae5' ORDER BY id DESC LIMIT 1;";
$temperature = $conn->query($sql)->fetch_assoc()["Temperature"];
$sql = "select Humidity from Xiaomi_Temperature_Sensors where device like '%cae5' ORDER BY id DESC LIMIT 1;";
$humidity = $conn->query($sql)->fetch_assoc()["Humidity"];
echo $temperature . $degreeChar . $humidity . "%";

echo " ";

// Balcony
$sql = "select Temperature from Xiaomi_Temperature_Sensors where device like '%ef30' ORDER BY id DESC LIMIT 1;";
$temperature = $conn->query($sql)->fetch_assoc()["Temperature"];
$sql = "select Humidity from Xiaomi_Temperature_Sensors where device like '%ef30' ORDER BY id DESC LIMIT 1;";
$humidity = $conn->query($sql)->fetch_assoc()["Humidity"];
echo $temperature . $degreeChar . $humidity . "%";

$conn->close();
?>