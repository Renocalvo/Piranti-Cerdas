<?php
// Debugging untuk melihat data yang diterima // Menampilkan array GET yang diterima

// Proses penyimpanan data
$host = "localhost";
$dbname = "pirdasmod4";  // Nama database Anda
$username = "root";      // Username database Anda
$password = "";          // Password database Anda

$conn = new mysqli($host, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}

if (isset($_GET['sumbu_x']) && isset($_GET['sumbu_y']) && isset($_GET['status'])) {
    $sumbu_x = $_GET['sumbu_x'];
    $sumbu_y = $_GET['sumbu_y'];
    $status = $_GET['status'];

    // Simpan data ke database
    $sql = "INSERT INTO sensor_bidang (sumbu_x, sumbu_y, status) VALUES ('$sumbu_x', '$sumbu_y', '$status')";
    if ($conn->query($sql) === TRUE) {
        echo "Data berhasil disimpan";
    } else {
        echo "Error saat menyimpan data: " . $conn->error;
    }
} else {
    echo "Data tidak lengkap!";  // Debugging untuk melihat data GET yang diterima
}

$conn->close();
?>
