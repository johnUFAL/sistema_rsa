#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>

const port = process.env.PORT || 5500;

int ehPrimo(mpz_t valor);

void generateExponent(mpz_t totient, int variedade, int menor_valor);

void euclidesExtendido(mpz_t div01, mpz_t div02, mpz_t coeficiente, int zero, int um, mpz_t result_mdc);

void inverso(mpz_t div01, mpz_t div02, mpz_t coeficiente);

void euclides_mdc(mpz_t div01, mpz_t div02, mpz_t result_mdc);

void exponenciacao(mpz_t base, mpz_t e, mpz_t mod, mpz_t result);

void gerarChavePublica();

void criptografiaEmBloco(mpz_t modulo, mpz_t expoente, char *mensagem, mpz_t *criptografado);

void criptografarGeral();

void descriptografiaEmBloco(mpz_t modulo, mpz_t inverso, mpz_t *criptografado, char *descriptografado, long long int tamanho);

void descriptografarGeral();


int main()
{
    while (1)
    {
        int opcao;

        printf("\n------------------Menu------------------\n\n");
        printf(" [1] - Gerar chave pública.\n");
        printf(" [2] - Criptografar mensagem.\n");
        printf(" [3] - Descriptografar mensagem.\n");
        printf(" [4] - Para finalizar o programa.\n\n");
        printf(" Digite a sua opção: ");
 
        scanf("%d", &opcao);
        
        switch (opcao)
        {
            case 1:
                gerarChavePublica();
                break;

            case 2:
                criptografarGeral();
                break;

            case 3:
                descriptografarGeral();
                break;
            
            case 4:
                printf("\n Encerrando o programa...\n\n");
                return 0;
                
            default:
                printf(" Digite um número de 1 a 4 SOMENTE.");
                scanf("%d", &opcao);
                break;
        }
    }

    return 0;
}

//Função para checar se o valor é primo
int ehPrimo(mpz_t valor)
{
    //mpz_get_ui converte o valor mpz em um long int
    //essa função não irá manipular o valor original
    if (mpz_get_ui(valor) <= 1)
    {
        return 0;
    }
    else if (mpz_get_ui(valor) == 2)
    {
        return 1;
    }
    else if(mpz_get_ui(valor) % 2 == 0)
    {
        return 0;
    }
    else
    {
        for (int i = 3; i <= sqrt(mpz_get_ui(valor)); i += 2)
        {
            if (mpz_get_ui(valor) % i == 0)
            {
                return 0;
            }
        }
    }
    
    return 1;
}

//Função para gerar um intervalo de números coprimos ao totiente
void generateExponent(mpz_t totient, int variedade, int menor_valor) 
{
    mpz_t exponent;
    mpz_init_set_ui(exponent, menor_valor);

    printf(" Suas opções: {");
    while (variedade) 
    {
        //mpz_gcd_ui retorna o mdc de dois valores
        //mpz_get_ui(totient) pega o valor do totiente e o converte para lon int impedindo assim alteração no valor original
        //mpz_gcd_ui(NULL, exponent, mpz_get_ui(totient) como o resultado da operação foi colocado um "NULL"
        //isso quer dizer que o resultado não será armazenado, mas sim retornado para a função, e depois, checado
        while (mpz_gcd_ui(NULL, exponent, mpz_get_ui(totient)) != 1) 
        {
            mpz_add_ui(exponent, exponent, 1);
        }
        
        gmp_printf("%Zd", exponent);
        
        mpz_add_ui(exponent, exponent, 1);
        
        if (--variedade != 0) 
        {
            printf(", ");
        } 
        else 
        {
            printf("}\n");
        }
    }
    
    mpz_clear(exponent);
}

// Função para encontrar o 's' e o 't' de uma congruência linear
void euclidesExtendido(mpz_t div01, mpz_t div02, mpz_t coeficiente, int zero, int um, mpz_t result_mdc)
{   
    mpz_t aux, aux_n, resto, prox_resto, temporario, n1, n2;
    mpz_init(n1);
    mpz_init(n2);
    mpz_init(aux);
    mpz_init(aux_n);
    mpz_init(resto);
    mpz_init(prox_resto);
    mpz_init(temporario);
    
    mpz_set_ui(n1, zero); //n1 = zero
    mpz_set_ui(n2, um); //n2 = um

    mpz_tdiv_r(prox_resto, div01, div02); // prox_resto = div01 % div02
    
    while (mpz_cmp_ui(prox_resto, 0) != 0)
    {
        //parte responsável pelo coeficiente de menor valor
        mpz_set(aux_n, n1); //aux_n = n1
        mpz_set(n1, n2); //n1 = n2
        mpz_tdiv_q(temporario, div01, div02); //temporario = div01 / div02
        mpz_mul(temporario, temporario, n2); //temporario = temporario * n2;
        mpz_sub(temporario, aux_n, temporario); //temporario = aux_n - temporario;
        mpz_set(n2, temporario); //n2 = temporario
        ///gmp_printf("%Zd\n", n2);
        //n2 = aux_n - (n2 * (div01 / div02));
        
        //parte responsável pelo mdc e pela condição de parada
        mpz_set(aux, div01); //aux = div01
        mpz_set(div01, div02); //div01 = div02
        mpz_tdiv_r(resto, aux, div02); //resto = aux % div02
        mpz_set(div02, resto); //div02 = resto
        mpz_tdiv_r(prox_resto, div01, div02); //prox_resto = div01 % div02
        //
    }
    
    mpz_set(result_mdc, div02); //result_mdc = div02

    //o coeficiente do segundo fator será atribuido a variável coeficiente
    mpz_set(coeficiente, n2); //coeficiente = n2;
    ///gmp_printf("Coeficiente -> %Zd\n", coeficiente);
    
    mpz_clear(n1);
    mpz_clear(n2);
    mpz_clear(aux);
    mpz_clear(aux_n);
    mpz_clear(resto);
    mpz_clear(prox_resto);
    mpz_clear(temporario);

    return;
}

// Função para encontrar o inverso de determinado número que ao ser dividido pelo módulo resta 1
void inverso(mpz_t div01, mpz_t div02, mpz_t coeficiente)
{
    ///gmp_printf("Div01 = %Zd, Div02 = %Zd\n", div01, div02);
    
    mpz_t n1, n2, aux, result_mdc, maior;
    mpz_init(n1);
    mpz_init(n2);
    mpz_init(aux);
    mpz_init(maior);
    mpz_init(result_mdc);
    
    mpz_set(n1, div01);
    mpz_set(n2, div02);
    
    ///gmp_printf("Div01 = %Zd, Div02 = %Zd\n", n1, n2);
    
    if (mpz_cmp(n1, n2) < 0)
    {
        mpz_set(aux, n1);
        mpz_set(n1, n2);
        mpz_set(n2, aux);
    }
    
    mpz_set(maior, n1); //maior = n1
    
    ///gmp_printf("n1 = %Zd, n2 = %Zd, Maior = %Zd\n", n1, n2, maior);
    
    // coef = coeficientes que seguem a e b
    // mdc = (a * s) + (b * t)
    // coef iniciais de m: 1 e 0
    // coef iniciais de n: 0 e 1  
                   
    euclidesExtendido(n1, n2, coeficiente, 0, 1, result_mdc);
    
    if (mpz_cmp_ui(result_mdc, 1) != 0)
    {
        printf("\n Tal congruência não dispõe de inverso! Isso ocorre devido ao fato do MDC do totiente[(p -1) * (q - 1)] e do 'e' ser diferente de 1\n");
        
        mpz_clear(n1);
        mpz_clear(n2);
        mpz_clear(aux);
        mpz_clear(result_mdc);
        
        main();
    }
            
    if (mpz_cmp_ui(coeficiente, 0) < 0)
    {
        mpz_add(coeficiente, coeficiente, maior); //coeficiente = coeficiente + maior
    } 
    
    mpz_clear(n1);
    mpz_clear(n2);
    mpz_clear(aux);
    mpz_clear(result_mdc);

    return;
}

// Essa função retorna o MDC de dois números dados
void euclides_mdc(mpz_t valor01, mpz_t valor02, mpz_t result_mdc)
{
    mpz_t aux, resto, div01, div02, prox_resto;
    mpz_init(aux);
    mpz_init(resto);
    mpz_init(div01);
    mpz_init(div02);
    mpz_init(prox_resto);
    
    mpz_set(div01, valor01);
    mpz_set(div02, valor02);
    
    mpz_tdiv_r(prox_resto, div01, div02); // prox_resto = div01 % div02
    
    while (mpz_cmp_ui(prox_resto, 0) != 0)
    {
        mpz_set(aux, div01); //aux = div01
        mpz_set(div01, div02); //div01 = div02
        
        mpz_tdiv_r(resto, aux, div02); //resto = aux % div02
        
        mpz_set(div02, resto); //div02 = resto
        
        mpz_tdiv_r(prox_resto, div01, div02); //prox_resto = div01 % div02
    }
    
    mpz_set(result_mdc, div02); //mdc = div02

    mpz_clear(aux);
    mpz_clear(resto);
    mpz_clear(div01);
    mpz_clear(div02);
    mpz_clear(prox_resto);
    
    return;
}

void exponenciacao(mpz_t base, mpz_t e, mpz_t mod, mpz_t result) 
{
    mpz_t temporario;
    mpz_init(temporario);
    
    mpz_tdiv_r(base, base, mod); //base = base % mod
    
    while (mpz_cmp_ui(e, 0) > 0) //e > 0
    {
        mpz_tdiv_r_ui(temporario, e, 2); //temporario = e % 2
        
        if (mpz_cmp_ui(temporario, 1) == 0) //e % 2 == 1
        {
            //result = (result * base) % mod;
            mpz_mul(result, result, base); //result = result * base
            mpz_tdiv_r(result, result, mod); //result = result % mod
        }

        //e = e >> 1; Desloca em uma posição para à direita
        mpz_fdiv_q_2exp(e, e, 1); //deslocamento de 1 bit
        
        
        //base = (base * base) % mod;
        mpz_mul(base, base, base); //base = base * base
        mpz_tdiv_r(base, base, mod); //base = base % mod
    }
    
    mpz_clear(temporario);

    return;
}

void gerarChavePublica()
{
    mpz_t p, q, e, n, totiente, p2, q2, result_mdc, coeficiente;
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(n);
    mpz_init(p2);
    mpz_init(q2);
    mpz_init(totiente);
    mpz_init(result_mdc);
    mpz_init(coeficiente);
    
    printf("\n-------------------Criação da Chave Pública-------------------\n");
    printf("\n Digite dois números primos para a chave 'p' e a chave 'q': ");
    gmp_scanf("%Zd %Zd", p, q);
    
        while (ehPrimo(p) == 0)
        {
            gmp_printf(" O valor de 'p' = %Zd não é um número primo. Por favor, digite novamente: ", p);
            gmp_scanf("%Zd", p); 
        }

        while (ehPrimo(q) == 0)
        {
           gmp_printf(" O valor de 'q' = %Zd não é um número primo. Por favor, digite novamente: ", q);
           gmp_scanf("%Zd", q); 
        }

    gmp_printf(" Seus valores para 'p' e 'q' foram respectivamente %Zd e %Zd\n", p, q);
    
    mpz_mul(n, p, q); //n = p * q
    mpz_sub_ui(p2, p, 1); //p2 = p - 1 
    mpz_sub_ui(q2, q, 1); //q2 = q - 1
    // o "ui" no final permite realizar uma operação de um valor int com um mpz
    mpz_mul(totiente, p2, q2); //totiente = p2 * q2
    
    int repetir = 1; //1 -> repete o while, 0 -> não repete
    
    while (repetir)
    {
        char resquicio[50];
        
        printf(" Quer alternativas de números para 'e' [Y/N]: ");
        char resposta;
        scanf(" %c", &resposta);
        
            switch (resposta)
            {
                case 'Y':
                
                    int menor_valor, variedade;
                    
                    printf(" Gostaria de quantas opções de números: ");
                    scanf("%d", &variedade);
                    printf(" E qual seria o menor valor do intervalo: ");
                    scanf("%d", &menor_valor);
                    
                    generateExponent(totiente, variedade, menor_valor);
                    
                    repetir = 0;
                    break;
                
                case 'N':
                    repetir = 0;
                    break;
            
                default:
                    fgets(resquicio, 50, stdin);
                    printf(" Não entendi =(\n");
                    repetir = 1;
            }
    }
    
    printf(" Digite um número para a chave 'e': ");
    gmp_scanf("%Zd", e);
    
    euclides_mdc(totiente, e, result_mdc);
    
    ///gmp_printf("totiente = %Zd, MDC = %Zd\n", totiente, result_mdc);
    
    while (mpz_cmp_ui(result_mdc, 1) != 0)
    {
        printf(" O mdc entre o totiente = [(p - 1) * (q - 1)] e a chave digitada não é 1, portanto você deve digitar outro número para 'e': ");
        gmp_scanf("%Zd", e);
        
        euclides_mdc(totiente, e, result_mdc);
    }
    
    ///gmp_printf("Coeficiente = %Zd\n", coeficiente);
    
    inverso(e, totiente, coeficiente);
    
    printf("\n Chaves públicas utilizadas:\n");
    gmp_printf(" n = %Zd e e = %Zd\n\n", n, e);
    gmp_printf(" Chaves privadas utilizadas:\n");
    gmp_printf(" p = %Zd, q = %Zd e d = %Zd\n\n", p, q, coeficiente);
    
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(p2);
    mpz_clear(q2);
    mpz_clear(totiente);
    mpz_clear(result_mdc);
    mpz_clear(coeficiente);
    
    return;
}

void criptografiaEmBloco(mpz_t modulo, mpz_t expoente, char *mensagem, mpz_t *criptografado)
{
    mpz_t result, base, e, mod;
    mpz_init(result);
    mpz_init(base);
    mpz_init(mod);
    mpz_init(e);
    
    for (int i = 0; i < strlen(mensagem); i++)
    {
        mpz_set_ui(result, 1); //result = 1
        mpz_set(mod, modulo); //mod = modulo
        mpz_set(e, expoente); //e = expoente
        
        int posicaoASCII = (int) mensagem[i]; //casting de char para inteiro
        printf("Valor caractere: %d\n", posicaoASCII);
        
        mpz_set_ui(base, posicaoASCII); //base = posicaoASCII

        //criptografado[i] = exponenciacao(base, expoente, modulo, result);
        exponenciacao(base, e, mod, result);
        mpz_set(criptografado[i], result); //criptografado[i] = result
        
        gmp_printf("Valor criptografado: %Zd\n", criptografado[i]);
    }

    printf("\n Mensagem criptografada [em valores inteiros]:\n");

    printf(" [");

    for (int i = 0; i < strlen(mensagem); i++)
    {
        gmp_printf(" %Zd", criptografado[i]);
    }

    printf(" ]\n");
    
    mpz_clear(result);
    mpz_clear(base);
    mpz_clear(mod);
    mpz_clear(e);

    return;
}

void criptografarGeral()
{
    mpz_t p, q, e, n, totiente, coeficiente, result_mdc, p2, q2;
    
    char *mensagem = (char *) malloc(500 * sizeof(char));
    //long long int *criptografado = (long long int *) malloc(500 * sizeof(long long int));
    mpz_t *criptografado = (mpz_t *) malloc(500 * sizeof(mpz_t));
    
    if(criptografado == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória\n");
        return;
    }
    
    for (int i = 0; i < 500; ++i)
    {
        mpz_init(criptografado[i]);
    }
    
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(n);
    mpz_init(p2);
    mpz_init(q2);
    mpz_init(totiente);
    mpz_init(coeficiente);
    mpz_init(result_mdc);

    getchar();
    printf("\n------------------Criptografia------------------\n");
    printf("\n Digite uma mensagem para ser encriptada: ");
    fgets(mensagem, 500, stdin);
    mensagem[strcspn(mensagem, "\n")] = 0; // Remove newline character
    printf(" Digite dois números primos para a chave 'p' e a chave 'q': ");
    gmp_scanf("%Zd %Zd", p, q);
    
    while (ehPrimo(p) == 0)
        {
            gmp_printf(" O valor de 'p' = %Zd não é um número primo. Por favor, digite novamente: ", p);
            gmp_scanf("%Zd", p); 
        }

        while (ehPrimo(q) == 0)
        {
           gmp_printf(" O valor de 'q' = %Zd não é um número primo. Por favor, digite novamente: ", q);
           gmp_scanf("%Zd", q); 
        }

    gmp_printf(" Seus valores para 'p' e 'q' foram respectivamente %Zd e %Zd\n", p, q);
    
    mpz_mul(n, p, q); //n = p * q
    mpz_sub_ui(p2, p, 1); //p2 = p - 1 
    mpz_sub_ui(q2, q, 1); //q2 = q - 1
    // o "ui" no final permite realizar uma operação de um valor int com um mpz
    
    mpz_mul(totiente, p2, q2); //totiente = p2 * q2
    
    int repetir = 1; //1 -> repete o while, 0 -> não repete
    
    while (repetir)
    {
        char resquicio[50];
        
        printf(" Quer alternativas de números para 'e' [Y/N]: ");
        char resposta;
        scanf(" %c", &resposta);
        
            switch (resposta)
            {
                case 'Y':
                
                    int menor_valor, variedade;
                    
                    printf(" Gostaria de quantas opções de números: ");
                    scanf("%d", &variedade);
                    printf(" E qual seria o menor valor do intervalo: ");
                    scanf("%d", &menor_valor);
                    
                    generateExponent(totiente, variedade, menor_valor);
                    
                    repetir = 0;
                    break;
                
                case 'N':
                    repetir = 0;
                    break;
            
                default:
                    fgets(resquicio, 50, stdin);
                    printf(" Não entendi =(\n");
                    repetir = 1;
            }
    }
    
    printf(" Digite um número para a chave 'e': ");
    gmp_scanf("%Zd", e);
    
    euclides_mdc(totiente, e, result_mdc);
    
    while (mpz_cmp_ui(result_mdc, 1) != 0)
    {
        printf(" O mdc entre o totiente = [(p - 1) * (q - 1)] e a chave digitada não é 1, portanto você deve digitar outro número para 'e': ");
        gmp_scanf("%Zd", e);
        
        euclides_mdc(totiente, e, result_mdc);
    }
    
    criptografiaEmBloco(n, e, mensagem, criptografado);

    free(mensagem);
    //free(criptografado);
    
    for (int i = 0; i < 500; ++i)
    {
        mpz_clear(criptografado[i]);
    }
    
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(p2);
    mpz_clear(q2);
    mpz_clear(totiente);
    mpz_clear(coeficiente);
    mpz_clear(result_mdc);
    
    return;
}

void descriptografiaEmBloco(mpz_t modulo, mpz_t inverso, mpz_t *criptografado, char *descriptografado, long long int tamanho)
{
    mpz_t result, e, mod;
    mpz_init(result);
    mpz_init(mod);
    mpz_init(e);
    
    for (int i = 0; i < tamanho; i++)
    {
        mpz_set_ui(result, 1); //result = 1
        mpz_set(mod, modulo); //mod = modulo
        mpz_set(e, inverso); //e = inverso
        
        gmp_printf("Base: %Zd, expoente: %Zd, modulo: %Zd\n", criptografado[i], e, mod);
        
        //criptografado[i] = exponenciacao(criptografado[i], inverso, modulo);
        exponenciacao(criptografado[i], e, mod, result);
        mpz_set(criptografado[i], result); //criptografado[i] = result

        gmp_printf("%Zd\n", criptografado[i]);
        
        if (mpz_fits_ulong_p(criptografado[i])) //será verificado se criptografado[i] cabe em um unsigned long
        {
            unsigned long int valor = mpz_get_ui(criptografado[i]); //obtenção do valor para unsignedlong
        
            descriptografado[i] = (char) valor;  
        }
        else
        {
            printf("%d\n", i);
        }
    }

    printf("\n Mensagem descriptografada:\n");

    printf(" [ "); 
    
    for (int x = 0; x < tamanho; x++)
    {
        printf("%c", descriptografado[x]);
    }
    
    printf(" ]\n");
    
    mpz_clear(result);
    mpz_init(mod);
    mpz_init(e);
     
    return;
}

void descriptografarGeral()
{
    mpz_t p, q, p2, q2, e, n, totiente, coeficiente, result_mdc;
    char *mensagemdescripto = (char *) malloc(500 * sizeof(char));
    mpz_t *criptografado = (mpz_t *) malloc(500 * sizeof(mpz_t));
    int continuar = 0;
    long long int i = 0;
    
    for (int i = 0; i < 500; ++i)
    {
        mpz_init(criptografado[i]);
    }
    
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(n);
    mpz_init(p2);
    mpz_init(q2);
    mpz_init(totiente);
    mpz_init(coeficiente);
    mpz_init(result_mdc);

    getchar();
    printf("\n----------------------Descriptografia----------------------\n");
    printf("\n Digite a mensagem criptografada (digite -1 para finalizar): ");
    
    while (1)
    {
        continuar = gmp_scanf("%Zd", criptografado[i]);
        
        if (mpz_cmp_si(criptografado[i], -1) == 0 || continuar == EOF || continuar == 0) 
        {
            break;
        }

        gmp_printf(" %Zd\n", criptografado[i]);

        i++;
    }

    printf(" Digite dois números primos para a chave 'p' e a chave 'q': ");
    gmp_scanf("%Zd %Zd", p, q);
    
    while (ehPrimo(p) == 0)
        {
            gmp_printf(" O valor de 'p' = %Zd não é um número primo. Por favor, digite novamente: ", p);
            gmp_scanf("%Zd", p); 
        }

        while (ehPrimo(q) == 0)
        {
           gmp_printf(" O valor de 'q' = %Zd não é um número primo. Por favor, digite novamente: ", q);
           gmp_scanf("%Zd", q); 
        }

    gmp_printf(" Seus valores para 'p' e 'q' foram respectivamente %Zd e %Zd. Pode prosseguir!\n", p, q);
    
    printf(" Digite a chave 'e' fornecida: ");
    gmp_scanf("%Zd", e);
    
    mpz_mul(n, p, q); //n = p * q
    mpz_sub_ui(p2, p, 1); //p2 = p - 1 
    mpz_sub_ui(q2, q, 1); //q2 = q - 1
    // o "ui" no final permite realizar uma operação de um valor int com um mpz
    mpz_mul(totiente, p2, q2); //totiente = p2 * q2
    
    euclides_mdc(totiente, e, result_mdc);
    
    ///gmp_printf("Coeficiente = %Zd\n", coeficiente);
    
    while (mpz_cmp_ui(result_mdc, 1) != 0)
    {
        printf(" O mdc entre o totiente = [(p - 1) * (q - 1)] e a chave digitada não é 1, portanto você deve digitar outro número: ");
        gmp_scanf("%Zd", e);
        
        euclides_mdc(totiente, e, result_mdc);
    }
    
    //a partir da função inverso iremos checar os dados obtidos para que estes cumpram as condições para funcionarem corretamente
    //ex.: a existência de um inverso e se o 'e' e o totiente são coprimos
    inverso(e, totiente, coeficiente);

    descriptografiaEmBloco(n, coeficiente, criptografado, mensagemdescripto, i);

    free(mensagemdescripto);
    //free(criptografado);
    
    for (int i = 0; i < 500; ++i)
    {
        mpz_clear(criptografado[i]);
    }
    
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(p2);
    mpz_clear(q2);
    mpz_clear(totiente);
    mpz_clear(coeficiente);
    mpz_clear(result_mdc);
    
    return;
}