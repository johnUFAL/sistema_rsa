//Apenas estilo
function LimpaTexto1(){
    document.getElementById("texto_encriptar").value = ""; 
}
function LimpaTexto() {
    document.getElementById("texto_desencriptar").value = "";

}
function VoltarTxt1(){
    document.getElementById("texto_encriptar").value = "Ex: Eu amo Mat. Discreta";}

function VoltarTxt() {
    document.getElementById("texto_desencriptar").value = "Ex: Eu amo Mat. Discreta";
}

//Download dos resultados
function SalvarTexto() {
    const desencrip_resultado = document.getElementById("desencrip_resultado").value;

    const blob = new Blob([desencrip_resultado], { type: 'text/plain' });

    const link = document.createElement('a');
    link.download = 'texto_desencriptado.txt';
    link.href = URL.createObjectURL(blob);
    link.click();

    URL.revokeObjectURL(link.href);
}

function SalvarTexto() {
    const desencrip_resultado = document.getElementById("encrip_resultado").value;

    const blob = new Blob([desencrip_resultado], { type: 'text/plain' });

    const link = document.createElement('a');
    link.download = 'texto_encriptado.txt';
    link.href = URL.createObjectURL(blob);
    link.click();

    URL.revokeObjectURL(link.href);
}

function SalvarTexto() {
    const desencrip_resultado = document.getElementById("chave_resultado").value;

    const blob = new Blob([desencrip_resultado], { type: 'text/plain' });

    const link = document.createElement('a');
    link.download = 'chave.txt';
    link.href = URL.createObjectURL(blob);
    link.click();

    URL.revokeObjectURL(link.href);
}

//Obtendos os inputs da chave 
function chaveP() {
    let input = document.getElementById("p").value;
    console.log(input);
}
function chaveQ() {
    let input = document.getElementById("q").value;
    console.log(input);
}
function chaveE() {
    let input = document.getElementById("e").value;
    console.log(input);
    //Gerar alert se a chave n for compaivel(ainda precisa se feito)
    let mdc = document.getElementById("e").value;
    if (mdc != 1) {
        alert("A chave n é compaivel")
    }
}

//Obtendo inputs para encriptar
function TextoEncriptar(){
    let input = document.getElementById("texto_encriptar").value;
    console.log(input);
}
function chavePQ() {
    let input = document.getElementById("pq").value;
    console.log(input);
}
function chaveE() {
    let input = document.getElementById("e").value;
    console.log(input);
}

//Obtendo inputs para desencriptar
function TextoDesencriptar(){
    let input = document.getElementById("texto_desencriptar").value;
    console.log(input);
}
function chavePQ() {
    let input = document.getElementById("pq_desencriptar").value;
    console.log(input);
}
function chaveE() {
    let input = document.getElementById("e_desencriptar").value;
    console.log(input);
}

//Menu inicial 
function menu(opcao) {
    switch (opcao) {
    case 1:
        window.location.href = "chave.html";
        console.log(1);
        break;
    case 2:
        window.location.href = "encriptar.html";
        console.log(2);
        break;
    case 3:
        window.location.href = "desencriptar.html";
        console.log(3);
        break;
    }
}
