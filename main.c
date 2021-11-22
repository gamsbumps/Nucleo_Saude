#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
typedef struct{
    int dia, mes, ano;
}  Data;
//T após nome, cpf e nascimento indica terceiro.
typedef struct{
    char nomeT[20];
    char cpfT[11];
    int sexoT;
    Data nascimentoT;
} Terceiro;

typedef struct{
    int sexo, tipoPlano;
    char nome[20],  email[50], fone[14];
    char cpf[11];
    float valorTotal;
    Data nascimentoTitular, vencimentoPlano;
    Terceiro terceiro[8];
} Titular;

FILE *ptitular;
Titular titular_aux, titular_nulo;

void linha()
{
    int i;
    for (i=1; i<=120; i++){
        printf("_");}
    printf("\n");
}

void pulaLinha()
{
    printf("\n");
}

void cabec()
{
    system("cls");
    printf("Nucleo seguros");
    pulaLinha();
    pulaLinha();
}

void abre_arquivo()
{
    ptitular = fopen("nucleo_saude.txt", "r+b");
    if (ptitular == NULL)
        ptitular = fopen("nucleo_saude.txt", "w+b");
}

float calcularPlano(Titular titular)
{
    SYSTEMTIME time;
    GetSystemTime(&time);

    int anoAtual = time.wYear;
    int idade = anoAtual - titular.nascimentoTitular.ano;
    float valorTotal = 0.00;

    int qtdT;
    for (qtdT = 0; qtdT < sizeof(titular_aux.terceiro) / sizeof(titular_aux.terceiro[0]);){
        if (strcmp(titular_aux.terceiro[qtdT].cpfT, "")){
            qtdT++;
        }else{
            break;
        }
    }

    switch (titular.tipoPlano){
        case 1:
            valorTotal = 300.00;
            break;
        case 2:
            valorTotal = 400.00;
            break;
        case 3:
            valorTotal = 200.00;
            break;
        case 4:
            valorTotal = 500.00;
            break;
        default:
            break;
    }

    if (titular.sexo == 1 && (idade >= 13 && idade <= 35)){
        valorTotal *= (qtdT + 1) * 1.3;
    }else if (idade < 13){
        valorTotal *= (qtdT + 1) * 0.7;
    }else if (idade >= 60){
        valorTotal *= (qtdT + 1) * 1.4;
    }else{
        valorTotal *= (qtdT + 1);
    }
    int idadeT;
    for(int i = 0; i < qtdT; i++){
        idadeT = time.wYear - titular_aux.terceiro[i].nascimentoT.ano;
        if (titular_aux.terceiro[i].sexoT == 1 && (idadeT >= 13 && idadeT <= 35)){
            valorTotal *= 1.3;
        }else if (idade < 13){
            valorTotal *= 0.7;
        }else if (idade >= 60){
            valorTotal *= 1.4;
        }
    }
    if (qtdT > 1){
        valorTotal *= 0.8;
    }

    return valorTotal;
}

Terceiro inserirTerceiro()
{
    cabec();
    Terceiro terceiros;
    linha();
    printf("\n\nCadastrar dependente\n\n");
    printf("\nCPF: ");
    scanf("%s", &terceiros.cpfT);
    printf("\nNome.....: ");
    fflush(stdin);
    gets(terceiros.nomeT);
    printf("\nSexo: (1 - Feminino / 2 - Masculino) ");
    scanf("%d", &terceiros.sexoT);
    printf("\nData de Nascimento: ");
    printf("\nDia: ");
    scanf("%d", &terceiros.nascimentoT.dia);
    printf("\nMes: ");
    scanf("%d", &terceiros.nascimentoT.mes);
    printf("\nAno: ");
    scanf("%d", &terceiros.nascimentoT.ano);
    return terceiros;
}

void inserir()
{
    int resp;
    int respT;
    SYSTEMTIME time;
    GetSystemTime(&time);

    do {
        cabec();
        printf("\n\nCadastrar novo titular\n\n");
        printf("\nCPF: ");
        scanf("%s", &titular_aux.cpf);
        printf("\nNome.....: ");
        fflush(stdin);
        gets(titular_aux.nome);
        printf("\nTipo do plano\n(1 - Ouro / 2 - Diamante / 3 - Prata / 4 - Esmeralda: ");
        scanf("%d", &titular_aux.tipoPlano);
        printf("\nSexo: (1 - Feminino / 2 - Masculino) ");
        scanf("%d", &titular_aux.sexo);
        printf("\nData de Nascimento: ");
        printf("\nDia: ");
        scanf("%d", &titular_aux.nascimentoTitular.dia);
        printf("\nMes: ");
        scanf("%d", &titular_aux.nascimentoTitular.mes);
        printf("\nAno: ");
        scanf("%d", &titular_aux.nascimentoTitular.ano);
        printf("\nE-mail: ");
        fflush(stdin);
        gets(titular_aux.email);
        printf("\nTelefone: ");
        scanf("%s", &titular_aux.fone);


        int i = 0;
        do{
            printf("\nDeseja cadastrar um dependente? (1-sim/0-nao) ");
            scanf("%d", &respT);
            if (respT == 1){
                titular_aux.terceiro[i] = inserirTerceiro();
            }
            i++;
        } while (respT == 1);

        titular_aux.valorTotal = calcularPlano(titular_aux);
        if (titular_aux.tipoPlano == 1 || titular_aux.tipoPlano == 3){
            titular_aux.vencimentoPlano.dia = time.wDay;
            titular_aux.vencimentoPlano.mes = time.wMonth;
            titular_aux.vencimentoPlano.ano = (time.wYear + 1);
        }else if (titular_aux.tipoPlano == 2 || titular_aux.tipoPlano == 4){
            titular_aux.vencimentoPlano.dia = time.wDay;
            titular_aux.vencimentoPlano.mes = time.wMonth;
            titular_aux.vencimentoPlano.ano = (time.wYear + 2);
        }

        fseek(ptitular, 0, SEEK_END);
        fwrite(&titular_aux, sizeof(Titular), 1, ptitular);
        printf("\n\nTitular cadastrado com sucesso!\n\n");
        printf("\nDeseja cadastrar outro: (1-sim/0-nao)? ");
        scanf("%d", &resp);
        if (resp == 1) {
            memset(titular_aux.terceiro, 0, sizeof(Terceiro));
        }
    }while (resp ==1);
}

int procura(char cpf[][11])
{
    int p;
    p = 0;
    rewind(ptitular);
    fread(&titular_aux, sizeof(Titular), 1, ptitular);
    while (feof(ptitular)==0){
        if (strcmp(titular_aux.cpf, cpf[0]) == 0)
            return p;
        else{
            fread(&titular_aux, sizeof(Titular), 1,ptitular);
            p++;
        }
    }
    return -1;
}
void mostre(int pos)
{
    fseek(ptitular, pos*sizeof(Titular), SEEK_SET);
    fread(&titular_aux, sizeof(Titular), 1, ptitular);

    SYSTEMTIME time;
    GetSystemTime(&time);


    printf("\n\n");
    linha();
    printf("CPF          Nome            Sexo  Fone         Email                Idade  Plano Dep.    Valor Plano  Vencimento Plano");
    linha();
    int anoAtual = time.wYear;
    int idade = anoAtual - titular_aux.nascimentoTitular.ano;

    int qtdT;
    for (qtdT = 0; qtdT < sizeof(titular_aux.terceiro) / sizeof(titular_aux.terceiro[0]);) {
        if (strcmp(titular_aux.terceiro[qtdT].cpfT, "")) {
            qtdT++;
        }else {
            break;
        }
    }

    printf("%11s  %-15s %-4d  %-9s %20s  %d %5d %5d %12.2f %8d/%d/%d\n", titular_aux.cpf, titular_aux.nome,  titular_aux.sexo,
           titular_aux.fone, titular_aux.email,idade, titular_aux.tipoPlano, qtdT, titular_aux.valorTotal, titular_aux.vencimentoPlano.dia,
           titular_aux.vencimentoPlano.mes, titular_aux.vencimentoPlano.ano);
    linha();
}

/*
 * Autor: Guilherme
 * E-mail: guilherme.cavalcanti@aluno.uniaeso.edu.br
 */
void remover()
{
    int conf, resp, posicao;
    char cpf[11];
    do{
        cabec();
        printf("\n\nRemover titular\n\n\n");
        printf("\n\nCPF: ");
        scanf("%s", &cpf);
        posicao = procura(&cpf);
        if (posicao == -1)
            printf("\nTitular nao encontrado!!");
        else{
            mostre(posicao);
            printf("\n\nDeseja remover o titular(1-sim/0-nao)? ");
            scanf("%d", &conf);
            if (conf == 1){ fseek(ptitular,posicao*sizeof(Titular),SEEK_SET);
                fwrite (&titular_nulo,sizeof(Titular), 1, ptitular);
                printf("\n\nTitular removido com sucesso!");
            }
            else
                printf("\nRemocao cancelada!");
        }
        printf("\n\n\nDeseja remover outro(1-sim/0-nao)? ");
        scanf("%d", &resp);
    } while (resp ==1);
}
void alterar()
{
    SYSTEMTIME time;
    GetSystemTime(&time);
    int conf, resp, posicao;
    char cpf[11];

    do {
        cabec();
        printf("\n\nAlterar plano do titular\n\n\n");
        printf("\nDigite CPF: ");
        scanf("%s", &cpf);
        posicao = procura(&cpf);
        if (posicao == -1)
            printf("\nTitular,nao encontrado!!\a");
        else{
            mostre(posicao);
            printf("\n\nAlterar a plano do titular(1-sim/0-nao)? ");
            scanf("%d", &conf);
            if (conf == 1){
                printf("\nNovo plano: ");
                scanf("%d", &titular_aux.tipoPlano);
                titular_aux.valorTotal = calcularPlano(titular_aux);
                if (titular_aux.tipoPlano == 1 || titular_aux.tipoPlano == 3){
                    titular_aux.vencimentoPlano.dia = time.wDay;
                    titular_aux.vencimentoPlano.mes = time.wMonth;
                    titular_aux.vencimentoPlano.ano = (time.wYear + 1);
                }else if (titular_aux.tipoPlano == 2 || titular_aux.tipoPlano == 4){
                    titular_aux.vencimentoPlano.dia = time.wDay;
                    titular_aux.vencimentoPlano.mes = time.wMonth;
                    titular_aux.vencimentoPlano.ano = (time.wYear + 2);
                }
                fseek(ptitular,posicao*sizeof(Titular),SEEK_SET);
                fwrite(&titular_aux,sizeof(Titular), 1,ptitular);

                mostre(posicao);
                printf("\nPlano do titular alterado com sucesso!\n");
            }
            else
                printf("\n\nAlteracao cancelada!\n\n");
        }
        printf("\n\nDeseja alterar outro (1-sim/0-nao)? ");
        scanf("%d", &resp);
    }while (resp ==1);
}
void listagem()
{
    SYSTEMTIME time;
    GetSystemTime(&time);
    int anoAtual = time.wYear;

    cabec();
    linha();
    printf("\n                                                 Listagem Geral");
    pulaLinha();
    linha();
    pulaLinha();
    printf("CPF          Nome            Sexo  Fone         Email                Idade  Plano Dep.    Valor Plano  Vencimento Plano");
    pulaLinha();
    linha();
    pulaLinha();

    rewind(ptitular);
    fread(&titular_aux, sizeof(Titular), 1, ptitular);
    while (feof(ptitular)==0){
        if (titular_aux.tipoPlano != 0){
            int idade = anoAtual - titular_aux.nascimentoTitular.ano;
            int qtdT;
            for (qtdT = 0; qtdT < sizeof(titular_aux.terceiro) / sizeof(titular_aux.terceiro[0]);) {
                if (strcmp(titular_aux.terceiro[qtdT].cpfT, "")) {
                    qtdT++;
                } else {
                    break;
                }
            }
            printf("%11s  %-15s %-4d  %-9s %20s  %d %5d %5d %12.2f %8d/%d/%d\n", titular_aux.cpf, titular_aux.nome,  titular_aux.sexo,
                   titular_aux.fone, titular_aux.email,idade, titular_aux.tipoPlano, qtdT, titular_aux.valorTotal, titular_aux.vencimentoPlano.dia,
                   titular_aux.vencimentoPlano.mes, titular_aux.vencimentoPlano.ano);
        }

        fread(&titular_aux, sizeof(Titular), 1, ptitular);
    }
    linha();
    printf("tecle enter para voltar ao menu...");
    getche();
}

void listagemPorPlano()
{
    SYSTEMTIME time;
    GetSystemTime(&time);
    int ano = time.wYear, plano;

    cabec();
    linha();
    printf("\n                                                  Listagem Por Plano");
    pulaLinha();
    linha();
    pulaLinha();

    printf("Informe o Plano:");
    scanf("%d", &plano);

    linha();
    pulaLinha();
    printf("CPF          Nome            Sexo  Fone         Email                Idade  Plano Dep.    Valor Plano  Vencimento Plano");
    pulaLinha();
    linha();
    pulaLinha();

    rewind(ptitular);
    fread(&titular_aux, sizeof(Titular), 1, ptitular);
    while (feof(ptitular)==0){
        if (titular_aux.tipoPlano != 0){
            if(titular_aux.tipoPlano == plano){
                int idade = ano - titular_aux.nascimentoTitular.ano;
                int qtdT;
                for (qtdT = 0; qtdT < sizeof(titular_aux.terceiro) / sizeof(titular_aux.terceiro[0]);) {
                    if (strcmp(titular_aux.terceiro[qtdT].cpfT, "")){
                        qtdT++;
                    }else{
                        break;
                    }
                }
                printf("%11s  %-15s %-4d  %-9s %20s  %d %5d %5d %12.2f %8d/%d/%d\n",
                       titular_aux.cpf, titular_aux.nome, titular_aux.sexo,
                       titular_aux.fone, titular_aux.email,idade, titular_aux.tipoPlano, qtdT,
                       titular_aux.valorTotal, titular_aux.vencimentoPlano.dia,
                       titular_aux.vencimentoPlano.mes, titular_aux.vencimentoPlano.ano);
            }
        }
        fread(&titular_aux, sizeof(Titular), 1, ptitular);
    }
    linha();
    printf("tecle enter para voltar ao menu...");
    getche();
}

void listagemVencimentoPlano()
{
    SYSTEMTIME time;
    GetSystemTime(&time);
    int dia = time.wDay;
    int mes = time.wMonth;
    int ano = time.wYear;

    cabec();
    linha();
    pulaLinha();
    printf("                                      Listagem dos Vencimentos do Plano de Saude do Mes");
    pulaLinha();
    linha();
    pulaLinha();
    printf("Plano    Data adquirido  Vencimento do Plano\n");
    linha();
    pulaLinha();

    rewind(ptitular);
    fread(&titular_aux, sizeof(Titular), 1, ptitular);

    while (feof(ptitular) == 0){
        if (titular_aux.tipoPlano != 0){
            if (titular_aux.tipoPlano == 1 && titular_aux.vencimentoPlano.mes == mes){
                printf("Ouro      %d/%d/%d    %4d/%d/%d\n", dia, mes, ano, titular_aux.vencimentoPlano.dia, titular_aux.vencimentoPlano.mes,
                       titular_aux.vencimentoPlano.ano);
            }
        }
        fread(&titular_aux, sizeof(Titular), 1, ptitular);
    }

    rewind(ptitular);
    fread(&titular_aux, sizeof(Titular), 1, ptitular);

    while (feof(ptitular) == 0){
        if (titular_aux.tipoPlano != 0){
            if (titular_aux.tipoPlano == 2 && titular_aux.vencimentoPlano.mes == mes){
                printf("Diamante %3d/%d/%d    %4d/%d/%d\n", dia, mes, ano, titular_aux.vencimentoPlano.dia, titular_aux.vencimentoPlano.mes,
                       titular_aux.vencimentoPlano.ano);
            }
        }
        fread(&titular_aux, sizeof(Titular), 1, ptitular);
    }

    rewind(ptitular);
    fread(&titular_aux, sizeof(Titular), 1, ptitular);

    while (feof(ptitular) == 0){
        if (titular_aux.tipoPlano != 0){
            if (titular_aux.tipoPlano == 3 && titular_aux.vencimentoPlano.mes == mes){
                printf("Prata     %d/%d/%d    %4d/%d/%d\n", dia, mes, ano, titular_aux.vencimentoPlano.dia, titular_aux.vencimentoPlano.mes,
                       titular_aux.vencimentoPlano.ano);
            }
        }
        fread(&titular_aux, sizeof(Titular), 1, ptitular);
    }

    rewind(ptitular);
    fread(&titular_aux, sizeof(Titular), 1, ptitular);

    while (feof(ptitular) == 0){
        if (titular_aux.tipoPlano != 0){
            if (titular_aux.tipoPlano == 4 && titular_aux.vencimentoPlano.mes == mes){
                printf("Esmeralda %d/%d/%d    %4d/%d/%d\n", dia, mes, ano, titular_aux.vencimentoPlano.dia, titular_aux.vencimentoPlano.mes,
                       titular_aux.vencimentoPlano.ano);
            }
        }
        fread(&titular_aux, sizeof(Titular), 1, ptitular);
    }

    printf("\n\nTecle enter para voltar ao menu...");
    getche();
}
main()
{

    int op;
    abre_arquivo();
    do{
        cabec();
        printf("\n\nOpcoes: \n\n\n");
        printf(" 1- Cadastrar novo titular\n\n");
        printf(" 2- Alterar plano do titular\n\n");
        printf(" 3- Remover titular\n\n");
        printf(" 4- Listagem geral\n\n");
        printf(" 5- Listagem por plano\n\n");
        printf(" 6 - Listagem dos Vencimentos do Plano de Saude do Mes\n\n");
        printf(" 0- Sair\n\n");
        linha();
        printf("Informe a opcao desejada: ");
        scanf("%d", &op);
        switch(op){
            case 1:
                inserir();
                break;
            case 2:
                alterar();
                break;
            case 3:
                remover();
                break;
            case 4:
                listagem();
                break;
            case 5:
                listagemPorPlano();
                break;
            case 6:
                listagemVencimentoPlano();
                break;
            case 0:
                fclose(ptitular);
                break;
            default:
                printf("\n\n\aOpcao invalida!");
                break;
        }
    } while (op != 0);
}