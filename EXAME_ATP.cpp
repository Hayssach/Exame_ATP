#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_NOMES 100
#define NUM_MEMBROS 8

// --------------- Torneio de Jo-Ken-Po -----------------//

// Iten 4 - Struct com as identifica��es dos participantes/membros
typedef struct {
    int id;
    char nome[MAX_NOMES];
    int vitorias;
    int derrotas;
    int movimento_favorito; // nesse caso 0- pedra, 1- papel, 2 - tesoura
} Membros;

Membros* membros[NUM_MEMBROS];

// ITENS 2 E 3
int primeira_jogada(int jogar1, int jogar2){
    if(jogar1 == jogar2)
       return 0;
       if((jogar1 == 0 && jogar2 == 2)|| 
          (jogar1 == 1 && jogar2 == 0)||
          (jogar1 == 2 && jogar2 == 1))
           return 1;
           return 2;
}

int jogar_ate_o_desempate(){
  int jog1 = rand() % 3;
  int jog2 = rand() % 3;
  int resultado = primeira_jogada(jog1, jog2);
  if(resultado == 0)
     return jogar_ate_o_desempate();
  return resultado;   
}

// Iten 5 - Inicializar o Participante/membros
Membros* inicializar_membro(int id, const char* nome){
  Membros* m = (Membros*) malloc (sizeof(Membros));
  if(m!= NULL){
    int j;
    for(j = 0; j < MAX_NOMES - 1 && nome[j] != '\0'; j++){
       m->nome[j] = nome[j];
    }
    m->nome[j] = '\0'; // termina com '\0'
    m->id = id;
    m->vitorias = 0;
    m->derrotas = 0;
    m->movimento_favorito = rand() % 3;
  }
  return m;
}

// Iten 6 - Fun��o para imprimir os dados do membro/Participante
void imprime_membros(Membros* m){
  if (!m) return;
  printf("ID: %s\nNome: %d\nVitorias: %d\nDerrotas: %d\nMovimento favorito: ",
         m->id, m->nome, m->vitorias, m->derrotas);
  if(m->movimento_favorito == 0)
     printf("Pedra\n");
     else if (m->movimento_favorito == 1)
              printf("Papel\n");
              else
                 printf("Tesoura\n");
}

// Iten 7 - Fun�ao para liberar o espa�o alocado
void libere_membros(Membros* m){
  if(m){
    free(m);
  }
}

//Iten 8 - Fun��o de simula��o entre dois jogadores
void confronto(Membros* m1, Membros* m2){
  int p1 = rand() % 3;
  int p2 = rand() % 3;
  int resultado = primeira_jogada(p1, p2);
  
  if(resultado == 1){
     m1->vitorias++;
     m2->derrotas++;
  }else if (resultado == 2){
     m2->vitorias++;
     m1->derrotas++;
  }  
  m1->movimento_favorito = p1;
  m2->movimento_favorito = p2;
}

//Iten 9 - Fun��o para a prepara��o dos participantes
void pegar_nomes(const char* arquivo_de_entrada){
  FILE* k = fopen(arquivo_de_entrada, "r");
  if(!k){
    perror("Erro ao abrir ficha.txt");
    exit(1);
  }
  char buffer[MAX_NOMES];
  int id = 0;
  
  while(fgets(buffer, MAX_NOMES, k) && id < NUM_MEMBROS){
    buffer[strcspn(buffer, "\n")] = '\0';
    membros[id] = inicializar_membro(id, buffer);
    id++;
  }
  fclose(k);
}

// Iten 10 - Rodadas Classificat�rias
void fase_classificatoria(const char* arquivo_resultado){
  int resultados[NUM_MEMBROS][NUM_MEMBROS] = {0};
  
  for(int i = 0; i <NUM_MEMBROS; i++){
    for(int j = 0; j < NUM_MEMBROS; j++){
      if(i != j){
        int ganhador = jogar_ate_o_desempate();
        if(ganhador == 1){
          membros[i]->vitorias++;
          membros[j]->derrotas++;
          resultados[i][j] = 1;
        }else{
          membros[j]->vitorias++;
          membros[i]->derrotas++;
          resultados[i][j] = 2;
        }
      }
    }
  }
  FILE* k = fopen(arquivo_resultado, "w");
  if(!k){
    perror("Erro ao criar classificatoria.txt");
    exit(1);
  }
  fprintf(k, "Matriz de Resultados (1 = linha venceu, 2 = coluna venceu)\n\n");
  for(int i = 0; i < NUM_MEMBROS; i++){
    for(int j = 0; j < NUM_MEMBROS; j++){
      fprintf(k, "%d", resultados[i][j]);
    }
    fprintf(k, "\n");
  }
  fclose(k);
}

//Iten 11 - Rodadas eliminat�rias
int ordenar_vitorias(const void* a, const void* b){
  Membros* m1 = *(Membros**)a;
  Membros* m2 = *(Membros**)b;
  return m2->vitorias - m1->vitorias;
}
Membros* eliminatorias_final(const char* nome_arquivo){
  qsort(membros, NUM_MEMBROS, sizeof(Membros*), ordenar_vitorias); // Usei o qsort para uma melhor compara��o entre o dados
  
  FILE* k = fopen(nome_arquivo, "w");
  if(!k){
    perror("Erro ao abrir eliminatorias.txt");
    exit(1);
  }
  fprintf(k, "=== Eliminatorias ===\n\n");
  
  Membros* quartas[4];
  quartas[0] = jogar_ate_o_desempate() == 1 ? membros[0] : membros[7];
  quartas[1] = jogar_ate_o_desempate() == 1 ? membros[1] : membros[6];
  quartas[2] = jogar_ate_o_desempate() == 1 ? membros[2] : membros[5];
  quartas[3] = jogar_ate_o_desempate() == 1 ? membros[3] : membros[4];
  
  Membros* semis[2];
  semis[0] = jogar_ate_o_desempate() == 1 ? quartas[0] : quartas[3];
  semis[1] = jogar_ate_o_desempate() == 1 ? quartas[1] : quartas[2];
  
  Membros* campeao = jogar_ate_o_desempate() == 1 ? semis[0] : semis[1];
  
  fprintf(k, "Campe�o: %s\n", campeao->nome);
  fclose(k);
  return campeao;
}

// Iten 12 - Registrar as estat�sticas
void registrar_estatisticas(const char* arquivo_saida){
  FILE* k = fopen(arquivo_saida, "w");
  if(!k){
    perror("Erro ao abrir estatistica.txt");
    exit(1);
  }
  
  fprintf(k, "=== Estat�sticas dos Participantes ===\n\n");
  for(int i = 0; i < NUM_MEMBROS; i++){
    fprintf(k, "ID: %d\n", membros[i]->id);
    fprintf(k, "Nome: %s\n", membros[i]->nome);
    fprintf(k, "Vit�rias: %d\n", membros[i]->vitorias);
    fprintf(k, "Derrotas: %d\n", membros[i]->derrotas);
    fprintf(k, "Movimento Favorito: %s\n\n", 
            membros[i]->movimento_favorito == 0 ? "Pedra" :
            membros[i]->movimento_favorito == 1 ? "Papel" : "Tesoura");
  }
  fclose(k);
}

// Item 13 - Encerramento do torneio
void encerrar_torneio(){
  for(int i = 0; i < NUM_MEMBROS; i++){
    libere_membros(membros[i]);
  }
}

int main(){
  srand(time(NULL));
  printf("Resultado do Campeonato de Jo-Ken-Po!\n");
  
  pegar_nomes("ficha.txt");
  fase_classificatoria("classificatoria.txt");
  Membros* campeao = eliminatorias_final("eliminatoria.txt");
  registrar_estatisticas("estatistica.txt");
  
  printf("Campe�o: %s\n", campeao->nome);
  
  encerrar_torneio();
  getchar();
  return 0;
}
