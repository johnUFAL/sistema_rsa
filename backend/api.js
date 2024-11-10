// api.js
const express = require("express");
const bodyParser = require("body-parser");
const { execFile } = require("child_process");
const path = require("path");
const app = express();

app.use(bodyParser.json());
const file = path.join(__dirname, 'RSA');

app.use(express.static(path.join(__dirname, '../frontend')));

// gerar chave
app.post("/generate_key", (req, res) => {
    const { firstPrime, secondPrime, exponent } = req.body;
    execFile(file, ["generate_key", firstPrime, secondPrime, exponent], (error, stdout, stderr) => {
        if (error || stderr) {
            // use stderr ou error.message se ambos estiverem disponíveis
            const message = stderr || error.message || "An unknown error occurred.";

            // remover a quebra de linha (\r\n) e enviar como string simples
            const cleanedMessage = message.replace(/[\r\n]+/g, '');

            return res.status(400).send(cleanedMessage); // envia a string simples como resposta
        }
        res.send(stdout); // caso não haja erro envia o stdout
    });
});


// encriptar mensagem
app.post("/encrypt", (req, res) => {
    const { message, publicKey, exponent } = req.body;
    execFile(file, ["encrypt", message, publicKey, exponent], (error, stdout, stderr) => {
        if (error || stderr) {
            const message = stderr || error.message || "An unknown error occurred.";
            return res.status(400).json({ error: message });
        }
        res.send(stdout);
    });
});

// desencriptar mensagem
app.post("/decrypt", (req, res) => {
    const { message, firstPrime, secondPrime, exponent } = req.body;
    execFile(file, ["decrypt", message, firstPrime, secondPrime, exponent], (error, stdout, stderr) => {
        if (error || stderr) {
            const message = stderr || error.message || "An unknown error occurred.";
            return res.status(400).json({ error: message });
        }
        res.send(stdout);
    });
});

// server
const PORT = 3001;
app.listen(PORT, () => {
    console.log(`Servidor online em http://localhost:${PORT}`);
});