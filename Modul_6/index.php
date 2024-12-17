<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MPU6050 Data Visualization</title>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@400;500&display=swap" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body {
            font-family: 'Roboto', sans-serif;
            background: #f4f7fc;
            color: #333;
            margin: 0;
            padding: 20px;
            display: flex;
            flex-direction: column;
            align-items: center;
        }
        h1 {
            margin-bottom: 20px;
            color: #34495e;
        }
        .container {
            display: flex;
            flex-wrap: wrap;
            gap: 20px;
            justify-content: center;
        }
        .chart-container {
            width: 500px;
            background: #fff;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            padding: 20px;
        }
        .chart-container h3 {
            text-align: center;
            margin-bottom: 10px;
            color: #2c3e50;
        }
        canvas {
            max-width: 100%;
            height: 300px;
        }
        .status-card {
            width: 300px;
            height: 150px;
            background: #fff;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            margin-top: 30px;
        }
        .status-card h3 {
            margin: 0;
            color: #2c3e50;
        }
        .status-card p {
            margin: 5px 0 0;
            font-size: 24px;
            font-weight: bold;
            color: #3498db;
        }
    </style>
</head>
<body>
    <h1>MPU6050 Data Visualization</h1>

    <div class="container">
        <!-- Grafik Sumbu X -->
        <div class="chart-container">
            <h3>Grafik Sumbu X</h3>
            <canvas id="chartX"></canvas>
        </div>

        <!-- Grafik Sumbu Y -->
        <div class="chart-container">
            <h3>Grafik Sumbu Y</h3>
            <canvas id="chartY"></canvas>
        </div>
    </div>

    <div class="status-card">
        <h3>Status Sensor</h3>
        <p id="status">Loading...</p>
    </div>

    <script>
        // Inisialisasi data kosong untuk grafik
        const labels = [];
        const dataX = [];
        const dataY = [];

        // Grafik Sumbu X
        const ctxX = document.getElementById('chartX').getContext('2d');
        const chartX = new Chart(ctxX, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Sumbu X',
                    data: dataX,
                    borderColor: 'rgb(75, 192, 192)',
                    backgroundColor: 'rgba(75, 192, 192, 0.2)',
                    fill: true,
                    tension: 0.4
                }]
            },
            options: {
                scales: {
                    x: { title: { display: true, text: 'Time' } },
                    y: { title: { display: true, text: 'Value' } }
                }
            }
        });

        // Grafik Sumbu Y
        const ctxY = document.getElementById('chartY').getContext('2d');
        const chartY = new Chart(ctxY, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Sumbu Y',
                    data: dataY,
                    borderColor: 'rgb(255, 99, 132)',
                    backgroundColor: 'rgba(255, 99, 132, 0.2)',
                    fill: true,
                    tension: 0.4
                }]
            },
            options: {
                scales: {
                    x: { title: { display: true, text: 'Time' } },
                    y: { title: { display: true, text: 'Value' } }
                }
            }
        });

        // Fungsi untuk memperbarui data
        function updateData() {
    const xhr = new XMLHttpRequest();
    xhr.open("GET", "get_data.php", true); // Ambil data terbaru dari PHP
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
            const response = JSON.parse(xhr.responseText);

            if (!response.error) {
                // Gunakan Date.parse() untuk memvalidasi dan mengonversi tanggal
                const time = new Date(response.created_at);  // Format yang diterima: YYYY-MM-DD HH:mm:ss
                if (isNaN(time)) {
                    console.error("Invalid date received:", response.created_at);
                    return;
                }

                const timeString = time.toLocaleTimeString();  // Mengubah ke format waktu yang sesuai

                // Tambahkan data baru ke grafik
                if (labels.length > 10) {
                    labels.shift();
                    dataX.shift();
                    dataY.shift();
                }
                labels.push(timeString);
                dataX.push(response.sumbu_x);
                dataY.push(response.sumbu_y);

                // Perbarui grafik
                chartX.update();
                chartY.update();

                // Perbarui status
                const statusElement = document.getElementById('status');
                statusElement.textContent = response.status;
                statusElement.style.color = response.status === "Miring" ? "#e74c3c" : "#2ecc71";
            }
        }
    }; 

        xhr.send();
    }

    // Update data setiap 1 detik
    setInterval(updateData, 1000);

    </script>
</body>
</html>
