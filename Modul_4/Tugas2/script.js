
function sendSensorData() {
    const data = {
        angka_sumbu: 15.2,  // contoh data; sesuaikan ini dengan data aktual dari sensor
        arah: "kanan"       // contoh arah; sesuaikan juga dengan data aktual
    };

    fetch("save_sensor_data.php", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
    })
    .then(response => response.json())
    .then(result => {
        // Menampilkan respons dalam div pesan
        const messageDiv = document.getElementById("message");
        messageDiv.style.display = "block";

        if (result.status === "success") {
            messageDiv.style.color = "green";
            messageDiv.innerHTML = result.message;
        } else {
            messageDiv.style.color = "red";
            messageDiv.innerHTML = result.message;
        }

        // Hilangkan pesan setelah beberapa detik
        setTimeout(() => {
            messageDiv.style.display = "none";
        }, 3000); // 3 detik
    })
    .catch(error => {
        console.error("Error:", error);
        alert("Gagal mengirim data ke server.");
    });
}
