<?php
include 'connection.php'; // Hubungkan ke database

// Query untuk mendapatkan data dari tabel sensor_gerak dalam urutan waktu ASCENDING (dari yang paling lama ke terbaru)
$sql = "SELECT id_sensor_gerak, angka_sumbu, arah, waktu FROM sensor_gerak ORDER BY waktu ASC";
$result = $conn->query($sql);

$data = []; // Array untuk menyimpan data

if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $data[] = $row; // Menyimpan setiap baris hasil query ke dalam array
    }
}

// Menutup koneksi database
$conn->close();

// Mengembalikan data dalam format JSON
header('Content-Type: application/json');
echo json_encode($data);
?>
