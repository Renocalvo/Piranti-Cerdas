<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard Sensor MPU6050</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
    <link rel="stylesheet" href="style.css">
    <style>
        /* Tambahan CSS untuk loader dan jumlah data */
        .loader {
            display: none;
            border: 4px solid #f3f3f3;
            border-radius: 50%;
            border-top: 4px solid #4CAF50;
            width: 30px;
            height: 30px;
            animation: spin 1s linear infinite;
            margin: 20px auto;
        }

        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }

        .data-count {
            margin: 10px 0;
            font-weight: bold;
            color: #333;
        }
    </style>
</head>
<body>

<div class="container">
    <h1>Data Sensor MPU6050</h1>
    <button onclick="loadData()" class="btn-custom">
        <i class="fas fa-sync"></i> Refresh Data
    </button>

    <!-- Loader -->
    <div id="loader" class="loader"></div>

    <!-- Jumlah Data -->
    <div id="data-count" class="data-count" style="margin-top:20px"></div>

    <table>
        <thead>
            <tr>
                <th>ID</th>
                <th>Angka Sumbu</th>
                <th>Arah</th>
                <th>Waktu</th>
            </tr>
        </thead>
        <tbody id="data-table-body">
            <!-- Data akan diisi oleh JavaScript -->
        </tbody>
    </table>
</div>

<script>
    // Fungsi untuk memuat data dari server dan menampilkannya di tabel
    function loadData() {
        // Tampilkan loader dan sembunyikan jumlah data saat memuat data
        document.getElementById("loader").style.display = "block";
        document.getElementById("data-count").innerText = "";

        fetch("fetch_data.php")
        .then(response => response.json())
        .then(data => {
            const tableBody = document.getElementById("data-table-body");
            const dataCount = document.getElementById("data-count");
            tableBody.innerHTML = ""; // Kosongkan isi tabel sebelum menambahkan data baru

            if (data.length > 0) {
                // Menampilkan setiap baris data dalam tabel
                data.forEach(row => {
                    const tr = document.createElement("tr");

                    tr.innerHTML = `
                        <td>${row.id_sensor_gerak}</td>
                        <td>${row.angka_sumbu}</td>
                        <td>${row.arah.charAt(0).toUpperCase() + row.arah.slice(1)}</td>
                        <td>${row.waktu}</td>
                    `;

                    tableBody.appendChild(tr);
                });

                // Tampilkan jumlah data
                dataCount.innerText = `Jumlah data: ${data.length}`;
            } else {
                // Jika tidak ada data, tampilkan pesan
                tableBody.innerHTML = `
                    <tr>
                        <td colspan="4" style="text-align: center; color: #999;">Tidak ada data tersedia</td>
                    </tr>
                `;
            }

            // Sembunyikan loader setelah data dimuat
            document.getElementById("loader").style.display = "none";
        })
        .catch(error => {
            console.error("Error fetching data:", error);
            alert("Gagal memuat data dari server.");
            document.getElementById("loader").style.display = "none";
        });
    }

    // Memuat data saat halaman pertama kali dimuat
    document.addEventListener("DOMContentLoaded", loadData);
</script>
</body>
</html>
