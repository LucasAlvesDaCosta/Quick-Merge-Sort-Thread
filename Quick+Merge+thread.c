#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <pthread.h>
#define size_vet  1000
   
	int vetorQuick[size_vet];
	int vetorMerge[size_vet];// vetores que serão ordenados posteriormente
	int send;
	clock_t inicio, fim;
	clock_t inicio2, fim2;// variaveis de tempo
	
	double tempo,tempo2;// receberão o tempo final
	
typedef struct NO_q{
	int dir;
	int esq;	
}no_q;

typedef struct node {
    int i;
    int j;
} NODE;// estruturas que serão utilizadas como parametro para as funçõeS de ordenaãção.

void *q_sort(void *parametro){
	    printf("*");
	    pthread_t td1,td2;
	    
        no_q *param;
	    param = (no_q*)parametro;// conversão do parametro para um tipo de estrutura(no_q)
	    
	    no_q *no1 = (no_q*)malloc(sizeof(no_q));
	    no_q *no2 = (no_q*)malloc(sizeof(no_q));
	    int *vetor = vetorQuick;// recebo o vetor correspondente ao algoritmo.
		int esquerda = param->esq; // passando valores do parametro para as variaves
		int direita = param->dir;
		int tamanho = direita;
    	int i, j, meio, y;
    	int execute;
		  	
    	i= esquerda;// 'i' representa o ultimo valor a esquerda do vetor
    	
    	j= direita;  // 'j' representa o primeiro valor a direita do vetor
    	
    	meio = vetor[(esquerda+direita)/2]; // 'meio' recebe a posição central"MEIO" do vetor 
    	 while(i<=j){// enquanto a direita for maior que a esquerda
         
    	 	while(vetor[i] < meio && i < direita){// enquanto o vetor na primeira posicao a esquerda for MENOR q o meio do vetor E o ultimo elemento a direita 
                                                  // do vetor for maior que o primeiro a esquerda(que vai ate o meio) eu avanço a direita 'i'
    	 		i++;
			}
			  while(vetor[j] > meio && j > esquerda){ // enquanto o fim do vetor a esquerda for MAIOR que o meio do vetor E a variavel 'J -> ultima posição'
			                                         //for maior que a esquerda(que vai ate no meio do vetor)
			  	j--;
			  }
			  
			  if(i <= j){  // se a esquerda atual for menorOUigual a direita atual faço as trocas
			  	
			  	y= vetor[i];  // recebo essa posiçao da esquerda
			  	vetor[i]=vetor[j]; // troco com a posicao do vetor da direita
			  	vetor[j]=y;  // agora a copia do valor que estava na posiçao do vetor da 'direita atual' vai para a esqueda atual
			  	 i++;   // avanço para o meio do vetor
			  	 j--;   // retroceço da posiçao até o meio do vetor
			  }
		}
    	
    	 if(j > esquerda){ // parte recursciva onde eu verifico se a direita ATUAL é maior que a esquerda passa por parametro
    	  
    	  no1->esq = esquerda;
    	  no1->dir = j;// parametros que serão passados como argumento do quicksort dentro da thread
    	  
    	execute = pthread_create(&td1, NULL, q_sort, (void *)no1);// executa uma thread para primeira parte do vetor
               if (execute){
                 printf("%d %s - unable to create thread - ret - %d\n", __LINE__, __FUNCTION__, execute);    
                 exit(1);
                }
		 }
		 
		  if(i < direita){ // se a  esquerda atual for maior que a direita passada por parametro
		  
		  no2->esq = i;
		  no2->dir = direita;// parametros da funçao que será executada dentro da thread
	      execute = pthread_create(&td2, NULL, q_sort, (void *)no2);// executando a thread para segunda parte do vetor
                if (execute){
                printf("%d %s - unable to create thread - ret - %d\n", __LINE__, __FUNCTION__, execute);    
                exit(1);
               }
               pthread_join(td1,NULL);
               pthread_join(td2,NULL);
        
		}
		pthread_exit(NULL);
}
/****************************************************************************************************************************/
void merge(int inicio, int fim){
	    int meio = (inicio+fim)/2;
	    int i, j, k;
	    int n1 = meio - inicio + 1;// recebe o tamanho de uma metade do array
	    int n2 =  fim - meio;// recebe o tamanho da outra metade do array
	    int *vetor = vetorMerge;
	    
	    /*Dois vetores auxiliares definidos de forma global.
		 Cada um dos vetores recebe o tamanho exato de cada metade de forma dinâmica*/
	    int *temp_L = (int*)malloc(n1*sizeof(int));
	    int *temp_R = (int*)malloc(n2*sizeof(int));
	      if(!temp_L || !temp_R){
	      	printf("SEM MEMORIA DISPONIVEL!\n\n");
	      	exit(1);
		  }
	    	temp_L[n1], temp_R[n2];
	 
	    /* copiando os dados da primeira parte do vetor para o vetor auxiliar*/
	    for (i = 0; i < n1; i++){
	    	temp_L[i] = vetor[inicio + i];
		}
	        /*copiando a segunda metade do vetor para o segundo vetor auxiliar*/
	    for (j = 0; j < n2; j++){
	    	 temp_R[j] = vetor[meio + 1+ j];
		}
	    /* reiniciando as variaveis*/
	    i = 0; 
	    j = 0;
	    k = inicio; 
	    
	    /*Momento da troca de valores onde comparo se o valor de um lado do vetor é menor ou igual
		ao valor do outro lado do vetor*/
	    while (i < n1 && j < n2) {
	        if (temp_L[i] <= temp_R[j]){
	            vetor[k] = temp_L[i];// depois da comparação jogo os valores menores dentro do vetor original
	            i++;// avanço para o meio
	        }
	        else
	        {
	            vetor[k] = temp_R[j];// jogo sempre o valor menor dentro do vetor original
	            j++; // avanço para o fim 
	        }
	        k++;// sobe uma posição
	    }
	 
	    /* copio o restante dos elementos "caso houver" do primeiro vetor auxiliar para o vetor original */
	    while (i < n1){
	        vetor[k] = temp_L[i];
	        i++;
	        k++;
	    }
	 
	    /* copiando valores do outro vetor auxiliar para o vetor original */
	    while (j < n2){
	        vetor[k] = temp_R[j];
	        j++;
	        k++;
	    }
	 free(temp_L);
	 free(temp_R);
}

void *mergesort(void *a){
	   printf("+");
        NODE *p = (NODE *)a;// convertendo o parametro 
        NODE n1, n2;
        int mid = (p->i+p->j)/2;// calculando a metade do vetor
        pthread_t tid1, tid2;
        int ret;

        n1.i = p->i;
        n1.j = mid;// parametros da funçao dentro do thread

        n2.i = mid+1;
        n2.j = p->j;// parametros da funçao dentro do thread

        if (p->i >= p->j) return;

        ret = pthread_create(&tid1, NULL, mergesort, &n1);// executa a thread para um lado do vetor
        if (ret) {
                printf("%d %s - unable to create thread - ret - %d\n", __LINE__, __FUNCTION__, ret);    
                exit(1);
        }


        ret = pthread_create(&tid2, NULL, mergesort, &n2);// executa a thead para o outro lado do vetor
        if (ret) {
                printf("%d %s - unable to create thread - ret - %d\n", __LINE__, __FUNCTION__, ret);    
                exit(1);
        }

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        merge(p->i, p->j);// faz a troca entre as posições
        pthread_exit(NULL);// encerra a thread
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
// funcão para imprimir um vetor. Entrada= vetor, tamanho do vetor. Saida = vetor ordenado
void imprime(int vetor[], int size){
	int i;
	printf("vetor ordenado-> ");
	for(i=0; i<=size-1; i++){
		printf("[%d] " ,vetor[i]);
	}printf("\nEND\n");
}
//-----------------------------------------------------------------------//
/*Funcão de busca binária, Entra o valor que será procurado e o vetor no qual a busca sera feita*/
void seek_bin(int valor, int size_m, int *MATRIZ){ 

    int found = 0; 
    int high = size_m; 
    int low = 0; 
    int i=0;

    int middle = (high + low)/2; // meio do vetor

     while((!found) && (high >= low)){ 
           if(valor == MATRIZ[middle]){
           	 found = 1; 
           	 printf("\nvalor <%d> encontrado na posicao[%d]\n",valor,middle);
           	 printf("_________________________________________________________________\n");
           	 sleep(1);
		   } 
            else
              if(valor < MATRIZ[middle]){// se o for menor que a posiçao central do vetor a busca sera feita novamente na parte esquerda do vetor
                high = middle -1;
				} 	
			else{
                	low = middle + 1; // caso contrário o valor será procurado na parte direita do vetor.
				} 
                     

           middle = (high + low) /2; // calculando o meio da parte que corresponde a especificação.
           i++;
      } 
      if(found ==0){
      	printf("\nValor <%d> nao encontrado neste Vetor[...]\n-----------------------------------------------------------------\n",valor);
      	sleep(1);
	  }
}

void inicializa(){
	
	no_q *no1 = (no_q*)malloc(sizeof(no_q));
	NODE no2;
	pthread_t t_quick, t_merge;
	
	int execute;
	int i;
	int aleatorio;
     
	 srand(time(NULL));	// funçao para impedir que os valores aleatorios se repitam
	 for(i=0; i<=size_vet-1; i++){
	 	aleatorio = (rand() % 2000);// gerando valores aleatórios
	 	vetorQuick[i] = aleatorio;// gardando cada valor no vetor
	 	vetorMerge[i] = aleatorio+1;// guardando o valor gerado acrescido de 1
	 }
	 // parametro do quicksort dentro do thread
	 no1->esq = 0;
	 no1->dir = size_vet-1;
	// parametro do mergesort dentro do thread
	 no2.i = 0;
	 no2.j = size_vet-1;
	 
	  inicio = clock();// disparo o cronometro de tempo
	  execute = pthread_create(&t_quick, NULL, q_sort, (void *)no1);// executo o primeiro thread para o quickort
	  pthread_join(t_quick,NULL); 
	  
	  if (execute!=0){
                printf("%d %s - unable to create thread - ret - %d\n", __LINE__, __FUNCTION__, execute);    
                exit(1);
      } 
       seek_bin(send,size_vet,vetorQuick);// faço a busca do elemento após a ordenação
       fim = clock();// recebo o tempo corrente do cronometro
       tempo = (fim-inicio/CLOCKS_PER_SEC);// calcula o tempo de execuçao.
       
       inicio2 = clock()-fim; // recebendo o valor de tempo menos o tempo da outra funçao para que essa variavel tenha um valor inicial = 0
       execute = pthread_create(&t_merge, NULL, mergesort, (void *)&no2);// executa o segund thread para o mergesort
	   pthread_join(t_merge,NULL);
	   
       if(execute!=0){
       	        printf("%d %s - unable to create thread - ret - %d\n", __LINE__, __FUNCTION__, execute);    
                exit(1);
	   }  
	   seek_bin(send,size_vet,vetorMerge);// faço a busca do valor após a ordenaçao.
	   fim2 = clock()-fim;// recebendo o tempo corrente do cronometro menos o tempo final da primeira funçao, isso é necessario para sabermos o tempo apenas desta função
	   tempo2 = (fim2-inicio2/CLOCKS_PER_SEC);//calculando o tempo de execuçao desta funçao
}

void entrada(){// função responsavel por receber o elemento de busca
	printf("\tDIGITE UM VALOR ALEATORIO ENTRE: ZER0->[0]_&_DOIS MIL->[2000]!\nVALOR->.");
	scanf("%d",&send);// recebendo o valor da busca antes da ondenação
	sleep(1,5);	
}

int main(int argc, char *argv[]) {
	    entrada();
	    inicializa();
	   printf("\n\n\t>>>ORDENACAO CONCLUIDA!!\n\n"); 
	   printf("\nTempo de ordenacao do QUICKSORT() = [%lf] M.segundos..\n",tempo);
       seek_bin(send,size_vet,vetorQuick);
        printf("\nTempo de ordenacao do MERGESORT() = [%lf] M.segundos..\n",tempo2);
	    seek_bin(send,size_vet,vetorMerge);
	    printf("A diferenca de tempo entre O Quick & Merge foi [%lf] M.segundos...\n",difftime(tempo, tempo2));
	         if(tempo<tempo2){
	         	printf("\n(O QuickSort foi mais rapido na ordenacao).\n");
			 }
			   if(tempo2<tempo){
			   	printf("\n(O MergeSort foi mais rapido na ordenacao).\n");
			   }else printf("\n[QuickSort]<time>(=)[MergeSort]\n");
	    
	    system("pause");
	    sleep(1);
	    printf("\n\t\t\t*****VETOR MERGE*****\n\n");
	    imprime(vetorMerge,size_vet);
	    sleep(3);
	    printf("\n\t\t\t*****VETOR QUICK*****\n\n");
	    imprime(vetorQuick,size_vet);
	return 0;
}

