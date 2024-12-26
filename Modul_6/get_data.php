<?php
$host = "localhost";
$dbname = "pirdasmod4";  // Nama database Anda
$username = "root";      // Username database Anda
$password = "";          // Password database Anda (jika ada)

// Membuat koneksi ke database
$conn = new mysqli($host, $username, $password, $dbname);


// Ambil data terbaru
$sql = "SELECT sumbu_x, sumbu_y, status, created_at FROM sensor_bidang ORDER BY id DESC LIMIT 1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    echo json_encode($row); // Kirim data dalam format JSON
} else {
    echo json_encode(["error" => "Tidak ada data"]);
}

$conn->close();
?>
