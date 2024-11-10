async function generateKey() {
    const firstPrime = document.getElementById("firstPrime").value;
    const secondPrime = document.getElementById("secondPrime").value;
    const exponent = document.getElementById("exponent").value;
    const response = await fetch("/generate_key", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ firstPrime, secondPrime, exponent })
    });
    document.getElementById("keyOutput").innerText = await response.text();
    //showNotification("Arquivo salvado com sucesso!");
}

async function encryptMessage() {
    const message = document.getElementById("encryptMessage").value;
    const publicKey = document.getElementById("publicKeyEncrypt").value;
    const exponent = document.getElementById("exponentEncrypt").value;
    const response = await fetch("/encrypt", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ message, publicKey, exponent })
    });
    document.getElementById("encryptOutput").innerText = await response.text();
    //showNotification("Arquivo salvado com sucesso!");
}

async function decryptMessage() {
    const message = document.getElementById("decryptMessage").value;
    const firstPrime = document.getElementById("firstPrimeDecrypt").value;
    const secondPrime = document.getElementById("secondPrimeDecrypt").value;
    const exponent = document.getElementById("decryptExponent").value;
    const response = await fetch("/decrypt", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ message, firstPrime, secondPrime, exponent })
    });
    document.getElementById("decryptOutput").innerText = await response.text();
    //showNotification("Arquivo salvado com sucesso!");
}

function navigateTo(page) {
    window.location.href = page;
}

document.addEventListener("DOMContentLoaded", () => {

    const currentTheme = localStorage.getItem("theme");
    if (currentTheme === "dark") {
        document.body.classList.add("dark-mode");
        setIcon("sun");
    } else {
        setIcon("moon");
    }

    const themeToggle = document.getElementById("themeToggle");
    themeToggle.addEventListener("click", toggleTheme);
});

function toggleTheme() {
    document.body.classList.toggle("dark-mode");
    const isDarkMode = document.body.classList.contains("dark-mode");
    localStorage.setItem("theme", isDarkMode ? "dark" : "light");
    setIcon(isDarkMode ? "sun" : "moon");
}

function setIcon(mode) {
    const themeToggle = document.getElementById("themeToggle");
    themeToggle.src = mode === "sun" ? "images/sun.png" : "images/moon.png";
}

function navigateTo(page) {
    window.location.href = page;
}

function showNotification(message) {
    const notification = document.getElementById('notification');
    notification.textContent = message;
    notification.style.display = 'block'; 

    setTimeout(() => {
        notification.classList.add('hide');
        setTimeout(() => {
            notification.style.display = 'none';
            notification.classList.remove('hide');
        }, 600); 
    }, 3000); 
}
