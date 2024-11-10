#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

// funções principais
void decryptMessage(mpz_t firstPrime, mpz_t secondPrime, mpz_t exponent, char* message);
void encryptMessage(char* message, mpz_t publicKey, mpz_t exponent);
void createPublicKey(mpz_t firstPrime, mpz_t secondPrime, mpz_t exponent);
int ehPrimo(unsigned long int n);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: ./RSA <Operação> <parâmetros...>\n");
        printf("Operações: generate_key, encrypt, decrypt\n");
        return 1;
    }

    // gerar chave pública
    if (strcmp(argv[1], "generate_key") == 0) {
        if (argc < 5) {
            printf("Uso: ./RSA generate_key <p> <q> <e>\n");
            return 1;
        }
        mpz_t firstPrime, secondPrime, exponent;
        mpz_inits(firstPrime, secondPrime, exponent, NULL);

        mpz_set_str(firstPrime, argv[2], 10);
        mpz_set_str(secondPrime, argv[3], 10);
        mpz_set_str(exponent, argv[4], 10);

        if (!ehPrimo(mpz_get_ui(firstPrime)) || !ehPrimo(mpz_get_ui(secondPrime))) {
            printf("Numeros devem ser primos.\n");
            fprintf(stderr, "Números devem ser primos!\n");
            return 1;
        }

        createPublicKey(firstPrime, secondPrime, exponent);
        mpz_clears(firstPrime, secondPrime, exponent, NULL);

    // encripta message
    } else if (strcmp(argv[1], "encrypt") == 0) {
        if (argc < 5) {
            printf("Uso: ./RSA encrypt <mensagem> <chave publica> <expoente>\n");
            return 1;
        }
        mpz_t publicKey, exponent;
        mpz_inits(publicKey, exponent, NULL);

        mpz_set_str(publicKey, argv[3], 10);
        mpz_set_str(exponent, argv[4], 10);

        encryptMessage(argv[2], publicKey, exponent);
        mpz_clears(publicKey, exponent, NULL);

    // desencripta mensagem
    } else if (strcmp(argv[1], "decrypt") == 0) {
        if (argc < 6) {
            printf("Uso: ./RSA decrypt <mensagem> <p> <q> <e>\n");
            return 1;
        }
        mpz_t firstPrime, secondPrime, exponent;
        mpz_inits(firstPrime, secondPrime, exponent, NULL);

        mpz_set_str(firstPrime, argv[3], 10);
        mpz_set_str(secondPrime, argv[4], 10);
        mpz_set_str(exponent, argv[5], 10);

        decryptMessage(firstPrime, secondPrime, exponent, argv[2]);
        mpz_clears(firstPrime, secondPrime, exponent, NULL);

    } else {
        printf("Operação inválida\n");
        return 1;
    }

    return 0;
}

// função para criar chave públicaa
void createPublicKey(mpz_t firstPrime, mpz_t secondPrime, mpz_t exponent) {

    mpz_t public_key, totient, gcd;
    mpz_inits(public_key, totient, gcd, NULL);

    // calcula n = p * q
    mpz_mul(public_key, firstPrime, secondPrime);

    // calcula o totiente: (p-1) * (q-1)
    mpz_t p_menos_1, q_menos_1;
    mpz_inits(p_menos_1, q_menos_1, NULL);
    mpz_sub_ui(p_menos_1, firstPrime, 1);
    mpz_sub_ui(q_menos_1, secondPrime, 1);
    mpz_mul(totient, p_menos_1, q_menos_1);

    // verifica se o expoente é coprimo com o totiente
    mpz_gcd(gcd, exponent, totient);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        printf("Erro: O expoente não é coprimo com o totiente. Escolha um expoente diferente.\n");
        mpz_clears(public_key, totient, gcd, p_menos_1, q_menos_1, NULL);
        return;
    }

    mpz_mul(public_key, firstPrime, secondPrime);
    gmp_printf("Chave pública gerada: %Zd\n", public_key);
    gmp_printf("Expoente: %Zd\n", exponent);

    // salvar a chave pública em um arquivo
    FILE *file = fopen("chave_publica.txt", "w");
    FILE *private = fopen("chave_privada.txt", "w");
    if (file != NULL) {
        gmp_fprintf(file, "Chave Pública: %Zd\n", public_key);
        gmp_fprintf(file, "Expoente: %Zd\n", exponent);
        gmp_fprintf(private, "%Zd %Zd", firstPrime, secondPrime);
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo para escrita.\n");
    }

    mpz_clear(public_key);
}

// função de criptografia
void encryptMessage(char* message, mpz_t publicKey, mpz_t exponent) {
    mpz_t value, result;
    mpz_inits(value, result, NULL);


    FILE *file = fopen("mensagem_encriptada.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (size_t i = 0; i < strlen(message); i++) {
        mpz_set_ui(value, (unsigned long) message[i]); // converte caractere para valor ASCII
        mpz_powm(result, value, exponent, publicKey); // calcula diretamente o valor criptografado sem um loop manual
        gmp_printf("%Zd ", result);
        gmp_fprintf(file, "%Zd ", result);
    }

    printf("\n");

    fclose(file);
    mpz_clears(value, result, NULL);
}

// função para descriptografar mensagem
void decryptMessage(mpz_t firstPrime, mpz_t secondPrime, mpz_t exponent, char* message) {
    mpz_t D, totient, N, num, t;
    mpz_inits(D, totient, N, num, t, NULL);

    mpz_sub_ui(totient, firstPrime, 1);
    mpz_t temp;
    mpz_init(temp);
    mpz_sub_ui(temp, secondPrime, 1);
    mpz_mul(totient, totient, temp);
    mpz_clear(temp);

    mpz_mul(N, firstPrime, secondPrime);

    // calcula inverso modular
    if (!mpz_invert(D, exponent, totient)) {
        printf("Erro: Não foi possível encontrar o inverso modular.\n");
        mpz_clears(D, totient, N, num, t, NULL);
        return;
    }

    FILE *file = fopen("mensagem_desencriptada.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        mpz_clears(D, totient, N, num, t, NULL);
        return;
    }

    // lê e descriptografa a mensagem
    char* token = strtok(message, " ");
    while (token != NULL) {
        mpz_set_str(num, token, 10);
        mpz_powm(t, num, D, N);
        printf("%c", (char)mpz_get_ui(t));
        token = strtok(NULL, " ");
        fprintf(file, "%c", (char)mpz_get_ui(t));
    }
    printf("\n");
    //fprintf(file, "\n");

    fclose(file);
    mpz_clears(D, totient, N, num, t, NULL);
}

// 
int ehPrimo(unsigned long int n) {
    if (n == 2 || n == 3) return 1;
    if (n < 2 || n % 2 == 0) return 0;
    for (unsigned long int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}