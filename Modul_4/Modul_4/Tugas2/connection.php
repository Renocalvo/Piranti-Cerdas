<?php
// Konfigurasi database
$host = "localhost";
$dbname = "pirdasmod4";  // ganti dengan nama database Anda
$username = "root";         // ganti dengan username database Anda
$password = "";             // ganti dengan password database Anda (jika ada)

// Membuat koneksi ke database
$conn = new mysqli($host, $username, $password, $dbname);

// Memeriksa koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}
?>
