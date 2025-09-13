#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int codigo;
    char nome[100];
    char endereco[200];
} Cliente;

typedef struct {
    int codigo;
    int ano_lancamento;
    char titulo[100];
    int status; 
} DVD;

typedef struct {
    int codigo;
    DVD* dvd_locado;
    float valor_diaria;
} Locacao;

typedef struct {
    int codigo;
    Cliente* cliente;
    Locacao locacoes[5];
    int num_locacoes;
    Data data_locacao;
    Data data_devolucao;
    float valor_total;
} Reserva;

Cliente clientes[50];
DVD dvds[50];
Locacao locacoes[100];
Reserva* reservas = NULL;

int num_clientes = 4;
int num_dvds = 4;
int num_locacoes = 6;
int num_reservas = 0;

void inicializar_dados();
void mostrar_menu();
Cliente* buscar_cliente(int codigo);
DVD* buscar_dvd(int codigo);
int calcular_dias(Data inicio, Data fim);

int CadastraCliente();
int CadastraDVD();
void CadastraLocacao();
void FazReserva();
void MostraCliente(Cliente c);
void MostraDVD(DVD d);
void MostraLocacao(Locacao l);
void MostraReserva(Reserva r);
int DevolveReserva();

void listar_clientes();
void listar_dvds();
void listar_reservas_cliente();

int main() {
    inicializar_dados();
    
    int opcao;
    do {
        mostrar_menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                if(CadastraCliente()) {
                    printf("Cliente foi cadastrado com sucesso\n");
                } else {
                    printf("Opa erro o cliente ja foi cadastrado ou limite atingido\n");
                }
                break;
            case 2:
                if(CadastraDVD()) {
                    printf("DVD cadastrado com sucesso\n");
                } else {
                    printf("Opa erro o DVD ja foi cadastrado ou limite atingido\n");
                }
                break;
            case 3:
                CadastraLocacao();
                break;
            case 4:
                FazReserva();
                break;
            case 5:
                if(DevolveReserva()) {
                    printf("Reserva foi devolvida com sucesso\n");
                } else {
                    printf("Opa erro a reserva nao encontrada\n");
                }
                break;
            case 6:
                listar_clientes();
                break;
            case 7:
                listar_dvds();
                break;
            case 8:
                listar_reservas_cliente();
                break;
            case 9:
                printf("Saindo do sistema..\n");
                break;
            default:
                printf("Opcao invalida\n");
        }
        printf("\nPressione Enter para continuar..");
        getchar();
        getchar();
    } while(opcao != 9);
    
    if(reservas != NULL) {
        free(reservas);
    }
    
    return 0;
}

void inicializar_dados() {

    clientes[0] = (Cliente){1, "Kethelem Socoowski", "Rua Visc de Maua, 817"};
    clientes[1] = (Cliente){2, "Elisa Reis", "Rua Visco de Maua, 819"};
    clientes[2] = (Cliente){3, "Willian dos Reis Braga", "Rua Rio Branco, 789"};
    clientes[3] = (Cliente){4, "Elem Cruz", "Rua Domingos de almeira, 328"};
    
    dvds[0] = (DVD){1, 2006, "Barbie e as 12 bailarinas", 1};
    dvds[1] = (DVD){2, 2000, "O massacre da serra eletrica", 1};
    dvds[2] = (DVD){3, 1998, "Titanic", 1};
    dvds[3] = (DVD){4, 2018, "Superman", 1};
    
    locacoes[0] = (Locacao){1, &dvds[0], 5.00};
    locacoes[1] = (Locacao){2, &dvds[1], 4.50};
    locacoes[2] = (Locacao){3, &dvds[2], 6.00};
    locacoes[3] = (Locacao){4, &dvds[3], 5.50};
    locacoes[4] = (Locacao){5, &dvds[0], 5.00};
    locacoes[5] = (Locacao){6, &dvds[1], 4.50};
  
    reservas = (Reserva*)malloc(10 * sizeof(Reserva));
}

void mostrar_menu() {
    printf("\n--- LOCADORA DE DVDS ---\n");
    printf("1. Cadastrar Cliente\n");
    printf("2. Cadastrar DVD\n");
    printf("3. Cadastrar Locacao\n");
    printf("4. Cadastrar Reserva\n");
    printf("5. Devolver Reserva\n");
    printf("6. Listar Clientes\n");
    printf("7. Listar DVDs\n");
    printf("8. Listar Locacoes de um Cliente - Reserva\n");
    printf("9. Sair\n");
}

int CadastraCliente() {
    if(num_clientes >= 50) return 0;
    
    int codigo;
    printf("Digite o codigo do cliente: ");
    scanf("%d", &codigo);

    for(int i = 0; i < num_clientes; i++) {
        if(clientes[i].codigo == codigo) {
            return 0; 
        }
    }
    
    clientes[num_clientes].codigo = codigo;
    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", clientes[num_clientes].nome);
    
    printf("Digite o endereco do cliente: ");
    scanf(" %[^\n]", clientes[num_clientes].endereco);
    
    num_clientes++;
    return 1;
}

int CadastraDVD() {
    if(num_dvds >= 50) return 0;
    
    int codigo;
    printf("Digite o codigo do DVD: ");
    scanf("%d", &codigo);
   
    for(int i = 0; i < num_dvds; i++) {
        if(dvds[i].codigo == codigo) {
            return 0;
        }
    }
    
    dvds[num_dvds].codigo = codigo;
    printf("Digite o titulo do DVD: ");
    scanf(" %[^\n]", dvds[num_dvds].titulo);
    
    printf("Digite o ano de lancamento: ");
    scanf("%d", &dvds[num_dvds].ano_lancamento);
    
    dvds[num_dvds].status = 1; 
    
    num_dvds++;
    return 1;
}

void CadastraLocacao() {
    if(num_locacoes >= 100) {
        printf("Limite de locacoes foi atingido\n");
        return;
    }
    
    int codigo_dvd, codigo_locacao;
    printf("Digite o codigo da locacao: ");
    scanf("%d", &codigo_locacao);
    
    printf("Digite o codigo do DVD: ");
    scanf("%d", &codigo_dvd);
    
    DVD* dvd = buscar_dvd(codigo_dvd);
    if(dvd == NULL) {
        printf("DVD nao encontrado!\n");
        return;
    }
    
    locacoes[num_locacoes].codigo = codigo_locacao;
    locacoes[num_locacoes].dvd_locado = dvd;
    
    printf("Digite o valor da diaria: ");
    scanf("%f", &locacoes[num_locacoes].valor_diaria);
    
    num_locacoes++;
    printf("Locacao cadastrada com sucesso\n");
}

void FazReserva() {
    if(num_reservas >= 10) {
        printf("Opa o limite de ja foi reservas atingido\n");
        return;
    }
    
    int codigo_cliente, codigo_reserva;
    printf("Digite o codigo da reserva: ");
    scanf("%d", &codigo_reserva);
    
    printf("Digite o codigo do cliente: ");
    scanf("%d", &codigo_cliente);
    
    Cliente* cliente = buscar_cliente(codigo_cliente);
    if(cliente == NULL) {
        printf("Cliente nao foi encontrado\n");
        return;
    }
    
    reservas[num_reservas].codigo = codigo_reserva;
    reservas[num_reservas].cliente = cliente;
    reservas[num_reservas].num_locacoes = 0;
    reservas[num_reservas].valor_total = 0;
  
    printf("Digite a data de locacao (dd mm aaaa): ");
    scanf("%d %d %d", &reservas[num_reservas].data_locacao.dia,
          &reservas[num_reservas].data_locacao.mes,
          &reservas[num_reservas].data_locacao.ano);

    int continuar = 1;
    while(continuar && reservas[num_reservas].num_locacoes < 5) {
        int codigo_locacao;
        printf("Digite o codigo da locacao (0 para parar): ");
        scanf("%d", &codigo_locacao);
        
        if(codigo_locacao == 0) {
            continuar = 0;
        } else {
            int encontrado = 0;
            for(int i = 0; i < num_locacoes; i++) {
                if(locacoes[i].codigo == codigo_locacao) {
                    if(locacoes[i].dvd_locado->status == 1) { 
                        reservas[num_reservas].locacoes[reservas[num_reservas].num_locacoes] = locacoes[i];
                        locacoes[i].dvd_locado->status = 0;
                        reservas[num_reservas].valor_total += locacoes[i].valor_diaria;
                        reservas[num_reservas].num_locacoes++;
                        encontrado = 1;
                        printf("DVD '%s' adicionado a reserva\n", locacoes[i].dvd_locado->titulo);
                    } else {
                        printf("DVD ja esta locado\n");
                    }
                    break;
                }
            }
            if(!encontrado) {
                printf("Locacao nao encontrada!\n");
            }
        }
    }
    
    if(reservas[num_reservas].num_locacoes > 0) {
        num_reservas++;
        printf("A reserva foi criada com sucesso\n");
    } else {
        printf("Nenhuma locacao foi adicionada. A reserva foi cancelada.\n");
    }
}

int DevolveReserva() {
    int codigo_reserva;
    printf("Digite o codigo da reserva: ");
    scanf("%d", &codigo_reserva);

    for(int i = 0; i < num_reservas; i++) {
        if(reservas[i].codigo == codigo_reserva) {

            printf("Digite a data da devolucao (dd mm aaaa): ");
            scanf("%d %d %d", &reservas[i].data_devolucao.dia,
                  &reservas[i].data_devolucao.mes,
                  &reservas[i].data_devolucao.ano);
     
            int dias = calcular_dias(reservas[i].data_locacao, reservas[i].data_devolucao);
            if(dias <= 0) dias = 1;
            
            reservas[i].valor_total = 0;
            for(int j = 0; j < reservas[i].num_locacoes; j++) {
                reservas[i].valor_total += reservas[i].locacoes[j].valor_diaria * dias;
                reservas[i].locacoes[j].dvd_locado->status = 1; 
            }
            
            printf("Valor total a pagar é: R$ %.2f\n", reservas[i].valor_total);
            return 1;
        }
    }
    return 0;
}

void MostraCliente(Cliente c) {
    printf("Codigo: %d\n", c.codigo);
    printf("Nome: %s\n", c.nome);
    printf("Endereco: %s\n", c.endereco);
}

void MostraDVD(DVD d) {
    printf("Codigo: %d\n", d.codigo);
    printf("Titulo: %s\n", d.titulo);
    printf("Ano: %d\n", d.ano_lancamento);
    printf("Status: %s\n", d.status ? "Disponivel" : "Locado");
}

void MostraLocacao(Locacao l) {
    printf("Codigo: %d\n", l.codigo);
    printf("DVD: %s\n", l.dvd_locado->titulo);
    printf("Valor diaria: R$ %.2f\n", l.valor_diaria);
}

void MostraReserva(Reserva r) {
    printf("-- RESERVA %d --\n", r.codigo);
    printf("Cliente: %s\n", r.cliente->nome);
    printf("Data locacao: %02d/%02d/%04d\n", r.data_locacao.dia, r.data_locacao.mes, r.data_locacao.ano);
    printf("Data devolucao: %02d/%02d/%04d\n", r.data_devolucao.dia, r.data_devolucao.mes, r.data_devolucao.ano);
    printf("Locacoes:\n");
    for(int i = 0; i < r.num_locacoes; i++) {
        printf("  - %s (R$ %.2f/dia)\n", r.locacoes[i].dvd_locado->titulo, r.locacoes[i].valor_diaria);
    }
    printf("Valor total: R$ %.2f\n", r.valor_total);
}

void listar_clientes() {
    printf("\n-- LISTA DE CLIENTES --\n");
    for(int i = 0; i < num_clientes; i++) {
        printf("\n--Cliente %d --\n", i+1);
        MostraCliente(clientes[i]);
    }
}

void listar_dvds() {
    printf("\n--LISTA DE DVDS --\n");
    for(int i = 0; i < num_dvds; i++) {
        printf("\n-- DVD %d --\n", i+1);
        MostraDVD(dvds[i]);
    }
}

void listar_reservas_cliente() {
    int codigo_cliente;
    printf("Digite o codigo do cliente: ");
    scanf("%d", &codigo_cliente);
    
    Cliente* cliente = buscar_cliente(codigo_cliente);
    if(cliente == NULL) {
        printf("Cliente nao encontrado!\n");
        return;
    }
    
    printf("\n=== RESERVAS DE %s ===\n", cliente->nome);
    int encontrou = 0;
    for(int i = 0; i < num_reservas; i++) {
        if(reservas[i].cliente->codigo == codigo_cliente) {
            MostraReserva(reservas[i]);
            printf("\n");
            encontrou = 1;
        }
    }
    
    if(!encontrou) {
        printf("Nenhuma reserva encontrada para este cliente.\n");
    }
}

Cliente* buscar_cliente(int codigo) {
    for(int i = 0; i < num_clientes; i++) {
        if(clientes[i].codigo == codigo) {
            return &clientes[i];
        }
    }
    return NULL;
}

DVD* buscar_dvd(int codigo) {
    for(int i = 0; i < num_dvds; i++) {
        if(dvds[i].codigo == codigo) {
            return &dvds[i];
        }
    }
    return NULL;
}

int calcular_dias(Data inicio, Data fim) {
    int dias_inicio = inicio.ano * 365 + inicio.mes * 30 + inicio.dia;
    int dias_fim = fim.ano * 365 + fim.mes * 30 + fim.dia;
    return dias_fim - dias_inicio;
}