//***********************************************************************************
// ********************* ALUNO : LUIS FERNANDO FURTADO DE ARAUJO ********************
// **********************************************************************************
// ********************* MATRICULA : 180042661 **************************************
//***********************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Calcula distância euclidiana e verifica de qual grupo de vetor feature teste a imagem está mais próxima
int distanciaEuclidiana(double *vet_testado, double *vet_asphalt, double *vet_grass, int tipo) {
	double distEuclidianaAsphalt=0, distEuclidianaGrass=0;

	//calculando distância euclidiana para ASPHALT
	for(int k=0; k<536; k++) {
		//capturando soma da distancia euclidiana (não usei função pow pois estava aumentando muito o tempo de processamento do programa)
		distEuclidianaAsphalt += ((*(vet_testado+k))-(*(vet_asphalt+k))) * ((*(vet_testado+k))-(*(vet_asphalt+k)));
		distEuclidianaGrass += ((*(vet_testado+k))-(*(vet_grass+k))) * ((*(vet_testado+k))-(*(vet_grass+k)));
	}
	//tirando a raiz para achar finalmente a distancia Euclidiana
	distEuclidianaAsphalt = sqrt(distEuclidianaAsphalt);
	distEuclidianaGrass = sqrt(distEuclidianaGrass);

	//Analizando em qual grupo de testes a imagem ficará mais próxima

	if(distEuclidianaAsphalt>distEuclidianaGrass) {
		if(tipo==1) {
			//Retornando um valor associado a taxa de falsa aceitação
			return 2;
		}
		else if(tipo==2) {
			//Retornando um valor associado a taxa de acerto
			return 1;
		}
	}
	else {
		if(tipo==1) {
			//Retornando um valor associado a taxa de acerto
			return 1;
		}
		else if(tipo==2) {
			//Retornando um valor associado a taxa de falsa rejeição
			return 3;
		}
	}

}

void geradorILBP(int *vet, double *vet_saida) {
	double media = 0;
	int vet_aux[9], num_convertido, decimal[9], decimal_menor, aux;
	char palavra_binaria[10];
	//desconsiderando primeira/última linha e primeiro/última coluna
	for(int i=1; i<1024; i++) {
		for(int j=1; j<1024; j++) {
					//calculando media da vizinhança de 8
					media = (double)( (*(vet+(1025*(i-1))+(j-1))) + (*(vet+(1025*(i-1))+j)) + (*(vet+(1025*(i-1))+(j+1))) + (*(vet+(1025*(i))+(j-1))) + (*(vet+(1025*i)+j)) + (*(vet+(1025*(i))+(j+1))) + (*(vet+(1025*(i+1))+(j-1))) + (*(vet+(1025*(i+1))+j)) + (*(vet+(1025*(i+1))+(j+1))) ) / 9;
					//preenchendo vetor auxiliar
					if((*(vet+(1025*(i-1)+(j-1))))>=media) {
						vet_aux[0] = 1;
					}	
					else {
						vet_aux[0] = 0;
					}
					if((*(vet+(1025*(i-1)+j)))>=media) {
						vet_aux[1] = 1;
					}
					else {
						vet_aux[1] = 0;
					}
					if((*(vet+(1025*(i-1)+(j+1))))>=media) {
						vet_aux[2] = 1;
					}
					else {
						vet_aux[2] = 0;
					}
					if((*(vet+(1025*i)+(j-1)))>=media) {
						vet_aux[3] = 1;
					}
					else {
						vet_aux[3] = 0;
					}
					if((*(vet+(1025*i)+j))>=media) {
						vet_aux[4] = 1;
					}
					else {
						vet_aux[4] = 0;
					}
					if((*(vet+(1025*(i))+(j+1)))>=media) {
						vet_aux[5] = 1;
					}
					else {
						vet_aux[5] = 0;
					}
					if((*(vet+(1025*(i+1))+(j-1)))>=media) {
						vet_aux[6] = 1;
					}
					else {
						vet_aux[6] = 0;
					}
					if((*(vet+(1025*(i+1))+j))>=media) {
						vet_aux[7] = 1;
					}
					else {
						vet_aux[7] = 0;
					}
					if((*(vet+(1025*(i+1))+(j+1)))>=media) {
						vet_aux[8] = 1;
					}
					else {
						vet_aux[8] = 0;
					}
					// coletando valores do vetor auxiliar em decimal e rotacionando vetor auxiliar
					for(int k=0; k<9; k++) {
						// coletando valores do vetor auxiliar em decimal
						decimal[k] = (vet_aux[0]*(pow(2.0,8.0))) + (vet_aux[1]*pow(2.0,7.0)) + (vet_aux[2]*pow(2.0,6.0)) + (vet_aux[3]*pow(2.0,5.0)) + (vet_aux[4]*pow(2.0,4.0)) + (vet_aux[5]*pow(2.0,3.0)) + (vet_aux[6]*pow(2.0,2.0)) + (vet_aux[7]*pow(2.0,1.0)) + (vet_aux[8]*pow(2,0));
						//rotacionando vetor auxiliar
						aux = vet_aux[0];
						for(int l=0; l<8; l++) {
							vet_aux[l] = vet_aux[l+1];
						}
						vet_aux[8] = aux;
					}
					decimal_menor = 10000;
					//encontrando o menor decimal (código ILBP pro pixel)
					for(int k=0; k<9; k++) {
						if(decimal_menor>decimal[k]) {
							decimal_menor = decimal[k];
						}
					}
					// incrementando +1 na matriz saida de 536 elementos no valor do menor decimal encontrado no pixel
					(*(vet_saida+decimal_menor)) += 1;
		}
	}

}


void geradorGLCM(int *vet, double *vet_saida) {
	int saida_glcm[8][256][256];
	double contraste[8], homogeneidade[8], energia[8];
	//preenchendo o vetor saida da GLCM com 0
	for(int i=0; i<8; i++) {
		for(int j=0; j<256; j++) {
			for(int k=0; k<256; k++) {
				saida_glcm[i][j][k] = 0;
			}
		}
	}
	//preenchendo os elementos de saida constraste, homogeniedade e energia com 0
	for(int i=0; i<8; i++) {
		contraste[i] = 0;
		homogeneidade[i] = 0;
		energia[i] = 0;
	}

	//preenchendo o vetor saida_glcm
	for(int i=0; i<1025; i++) {
		for(int j=0; j<1025; j++) {
			//vizinhança da diagonal superior esquerda
			if(i!=0&&j!=0) {
				(saida_glcm[0][(*(vet+(1025*i)+j))][(*(vet+(1025*(i-1))+(j-1)))])++;
			}
			//vizinhança de cima
			if(i!=0) {
				(saida_glcm[1][(*(vet+(1025*i)+j))][(*(vet+(1025*(i-1))+j))])++;
			}
			//vizinhança da diagonal superior direita
			if(i!=0&&j!=1024) {
				(saida_glcm[2][(*(vet+(1025*i)+j))][(*(vet+(1025*(i-1))+(j+1)))])++;
			}
			//vizinhança da esquerda
			if(j!=0) {
				(saida_glcm[3][(*(vet+(1025*i)+j))][(*(vet+(1025*(i))+(j-1)))])++;
			}
			//vizinhança da direita
			if(j!=1024) {
				(saida_glcm[4][(*(vet+(1025*i)+j))][(*(vet+(1025*(i))+(j+1)))])++;
			}
			//vizinhança da diagonal inferior esquerda
			if(i!=1024&&j!=0) {
				(saida_glcm[5][(*(vet+(1025*i)+j))][(*(vet+(1025*(i+1))+(j-1)))])++;
			}
			//vizinhança de baixo
			if(i!=1024) {
				(saida_glcm[6][(*(vet+(1025*i)+j))][(*(vet+(1025*(i+1))+j))])++;
			}
			//vizinhança da diagonal inferior direita
			if(i!=1024&&j!=1024) {
				(saida_glcm[7][(*(vet+(1025*i)+j))][(*(vet+(1025*(i+1))+(j+1)))])++;
			}
		}
	}
	for(int k=0; k<8; k++) {
		for(int i=0; i<256; i++) {
			for(int j=0; j<256; j++) {
				//calculando contraste
				if(k==0) {
					contraste[k] +=  fabs(pow(i-j,2))*((double)saida_glcm[k][i][j]/(double)(1024*1024));
					energia[k] +=  pow((double)saida_glcm[k][i][j]/(double)(1024*1024),2);
					homogeneidade[k] += ((double)saida_glcm[k][i][j]/(double)(1024*1024)) / (double)( 1 + (abs(i-j)));
				}
				else if(k==1) {
					contraste[k] +=  fabs(pow(i-j,2))*((double)saida_glcm[k][i][j]/(double)(1024*1025));
					energia[k] +=  pow((double)saida_glcm[k][i][j]/(double)(1024*1025),2);
					homogeneidade[k] += ((double)saida_glcm[k][i][j]/(double)(1024*1025)) / (double)( 1 + (abs(i-j)));
				}
				else if(k==2) {
					contraste[k] +=  fabs(pow(i-j,2))*((double)saida_glcm[k][i][j]/(double)(1024*1024));
					energia[k] +=  pow((double)saida_glcm[k][i][j]/(double)(1024*1024),2);
					homogeneidade[k] += ((double)saida_glcm[k][i][j]/(double)(1024*1024)) / (double)( 1 + (abs(i-j)));
				}
				else if(k==3) {
					contraste[k] +=  fabs(pow(i-j,2))*((double)saida_glcm[k][i][j]/(double)(1025*1024));
					energia[k] +=  pow((double)saida_glcm[k][i][j]/(double)(1025*1024),2);
					homogeneidade[k] += ((double)saida_glcm[k][i][j]/(double)(1025*1024)) / (double)( 1 + (abs(i-j)));
				}
				else if(k==4) {
					contraste[k] +=  fabs(pow(i-j,2))*((double)saida_glcm[k][i][j]/(double)(1025*1024));
					energia[k] +=  pow((double)saida_glcm[k][i][j]/(double)(1025*1024),2);
					homogeneidade[k] += ((double)saida_glcm[k][i][j]/(double)(1025*1024)) / (double)( 1 + (abs(i-j)));
				}
				else if(k==5) {
					contraste[k] +=  fabs(pow(i-j,2))*((double)saida_glcm[k][i][j]/(double)(1024*1024));
					energia[k] +=  pow((double)saida_glcm[k][i][j]/(double)(1024*1024),2);
					homogeneidade[k] += ((double)saida_glcm[k][i][j]/(double)(1024*1024)) / (double)( 1 + (abs(i-j)));
				}
				else if(k==6) {
					contraste[k] +=  fabs(pow(i-j,2))*((double)saida_glcm[k][i][j]/(double)(1024*1025));
					energia[k] +=  pow((double)saida_glcm[k][i][j]/(double)(1024*1025),2);
					homogeneidade[k] += ((double)saida_glcm[k][i][j]/(double)(1024*1025)) / (double)( 1 + (abs(i-j)));
				}
				else if(k==7) {
					contraste[k] +=  fabs(pow(i-j,2))*((double)saida_glcm[k][i][j]/(double)(1024*1024));
					energia[k] +=  pow((double)saida_glcm[k][i][j]/(double)(1024*1024),2);
					homogeneidade[k] += ((double)saida_glcm[k][i][j]/(double)(1024*1024)) / (double)( 1 + (abs(i-j)));
				}
			}
		}
	}
	//Preenchendo features do GLCM no vetor de 536, apartir da posição 512 até a 536
	for(int i=0; i<8; i++) {
			*(vet_saida+512+(3*i)+0) += contraste[i];
			*(vet_saida+512+(3*i)+1) += homogeneidade[i];
			*(vet_saida+512+(3*i)+2) += energia[i];
	}

}

void normalizaVetor(double *vet) {
	double min = 10000.0, max = 0.0;
	//Encontrando elemento com valor minimo e elemento com valor maximo de cada vetor
	for(int k=0; k<536; k++) {
		if(min>(*(vet+k))) {
			min = (*(vet+k));
		}
		if(max<(*(vet+k))) {
			max = (*(vet+k));
		}
	}
	//Normalizando vetor
	for(int k=0; k<536; k++) {
		*(vet+k) = ((*(vet+k)) - min) / (max - min);
	}
}

int main() {

	FILE *file1, *file2;

	//vetores para ler as imagens
	int *vet_asphalt = (int*) malloc(sizeof(int)*1025*1025);
	int *vet_grass = (int*) malloc(sizeof(int)*1025*1025);
	//vetores para armazenar as features do conjunto treino
	double *vet_asphalt_saida = (double*) calloc(536, sizeof(double));
	double *vet_grass_saida = (double*) calloc(536, sizeof(double));
	//vetores para armazenar a feature de cada imagem a ser julgada como teste
	double *vet_asphalt_aux = (double*) calloc(536, sizeof(double));
	double *vet_grass_aux = (double*) calloc(536, sizeof(double));

	if(vet_asphalt==NULL||vet_grass==NULL||vet_asphalt_saida==NULL||vet_grass_saida==NULL) {
		printf("Alocacao falhou. O programa será encerrado!\n");
		exit(1);
	}

	//criação do vetor associativo aos 25 casos de testes e 25 casos de treinamento
	int testes[50], randomico, aux2;

	srand( (unsigned)time(NULL) );

	for(int i=0; i<50; i++) {
		testes[i] = 0;
	}

	char arquivo1[50], arquivo2[50], aux;

	int random=0;

	for(int i=0;i<25;i++) {
		// procurando um valor randomico que NÃO SEJA REPETIDO entre 0 e 49
		aux2=1;
		while(aux2!=0) {
			randomico = (rand()%50);
			//testando se randomico já foi sorteado
			if(testes[randomico]==1) {
				continue;
			}
			else {
				testes[randomico] = 1;
				aux2=0;
			}
		}
		//Abrindo arquivo sorteado no Diretório da pasta
		sprintf(arquivo1,"DataSet/asphalt/asphalt_%02d.txt",randomico+1);
		sprintf(arquivo2,"DataSet/grass/grass_%02d.txt",randomico+1);
		file1 = fopen(arquivo1,"r");
		file2 = fopen(arquivo2,"r");
		for(int j=0;j<1025;j++) {
			for(int k=0;k<1025;k++) {
				//lendo numero de cada elemento do vetor ASPHALT e jogando char ';' fora
				fscanf(file1,"%d",vet_asphalt+(1025*j)+k);
				fscanf(file1,"%c",&aux);
				//lendo numero de cada elemento do vetor GRASS e jogando char ';' fora
				fscanf(file2,"%d",vet_grass+(1025*j)+k);
				fscanf(file2,"%c",&aux);
			}
		}

	//calculando os vetores de features das 50 imagens de treinamento ( 25 de cada )
	printf("Gerando vetor de Features de treinamento das imagens %d Asphalt e Grass\n",randomico+1);
	geradorILBP(vet_asphalt,vet_asphalt_saida);
	geradorILBP(vet_grass,vet_grass_saida);
	geradorGLCM(vet_asphalt,vet_asphalt_saida);
	geradorGLCM(vet_grass,vet_grass_saida);

	}
	
	//calculando a média da soma dos vetores features de cada imagem (não fiz função porque só precisa ser calculada uma vez)
	for(int k=0; k<536; k++) {
		//apenas dividindo para calcular a média, pois as funções de cálculo de ILBP e GLCM acrescentam as 25 imagens no mesmo vetor
		*(vet_asphalt_saida+k) = *(vet_asphalt_saida+k)/25;
		*(vet_grass_saida+k) = *(vet_grass_saida+k)/25;
	}
	
	//Normalizando os vetores de treino
	normalizaVetor(vet_asphalt_saida);
	normalizaVetor(vet_grass_saida);

	printf("Conjunto de treinamento finalizado!\nIniciando conjunto das imagens de testes.\n\n");

	//Entrando nos casos de TESTES

	double acertos=0, falsaAceitacao=0, falsaRejeicao=0;
	int retorno=0;

	for(int i=0;i<25;i++) {
		// procurando um valor randomico que NÃO SEJA REPETIDO entre 0 e 49
		aux2=1;
		while(aux2!=0) {
			randomico = (rand()%50);
			//testando se randomico já foi sorteado
			if(testes[randomico]==1) {
				continue;
			}
			else {
				testes[randomico] = 1;
				aux2=0;
			}
		}
		//Abrindo arquivo sorteado no Diretório da pasta
		sprintf(arquivo1,"DataSet/asphalt/asphalt_%02d.txt",randomico+1);
		sprintf(arquivo2,"DataSet/grass/grass_%02d.txt",randomico+1);
		file1 = fopen(arquivo1,"r");
		file2 = fopen(arquivo2,"r");
		for(int j=0;j<1025;j++) {
			for(int k=0;k<1025;k++) {
				//lendo numero de cada elemento do vetor ASPHALT e jogando char ';' fora
				fscanf(file1,"%d",vet_asphalt+(1025*j)+k);
				fscanf(file1,"%c",&aux);
				//lendo numero de cada elemento do vetor GRASS e jogando char ';' fora
				fscanf(file2,"%d",vet_grass+(1025*j)+k);
				fscanf(file2,"%c",&aux);
				//testando se ta certo kkkk
				//printf("%d ",*(vet_grass+(1025*j)+k));
			}
		}

		//calculando os vetores de features da imagem de teste
		printf("Gerando vetor de Features de teste das imagens %d Asphalt e Grass\n",randomico+1);
		geradorILBP(vet_asphalt,vet_asphalt_aux);
		geradorILBP(vet_grass,vet_grass_aux);
		geradorGLCM(vet_asphalt,vet_asphalt_aux);
		geradorGLCM(vet_grass,vet_grass_aux);

		//Normalizando os 2 vetores para serem testados
		normalizaVetor(vet_asphalt_aux);
		normalizaVetor(vet_grass_aux);

		//Calculando distância euclidiana da imagem teste ASPHALT
		printf("Verificando em qual grupo a imagem %d Asphalt ficou mais próximo.\n",randomico+1);
		retorno = distanciaEuclidiana(vet_asphalt_aux, vet_asphalt_saida, vet_grass_saida, 1);

		if(retorno==1) {
			acertos++;
		}
		else if(retorno==2) {
			falsaAceitacao++;
		}
		else if(retorno==3) {
			falsaRejeicao++;
		}

		//calculando distância euclidiana da imagem de teste GRASS
		printf("Verificando em qual grupo a imagem %d Grass ficou mais próximo.\n",randomico+1);
		retorno = distanciaEuclidiana(vet_grass_aux, vet_asphalt_saida, vet_grass_saida, 2);

		if(retorno==1) {
			acertos++;
		}
		else if(retorno==2) {
			falsaAceitacao++;
		}
		else if(retorno==3) {
			falsaRejeicao++;
		}
		printf("\n");

	}

	printf("Porcentagem de imagens que foram classificadas corretamente:\t%.2lf%%\n",(acertos/50)*100);
	printf("Porcentagem de falsa aceitação:\t%.2lf%%\n",(falsaAceitacao/50)*100);
	printf("Porcentagem de falsa rejeição:\t%.2lf%%\n",(falsaRejeicao/50)*100);
	
	fclose(file1);
	fclose(file2);
	free(vet_asphalt);
	free(vet_grass);
	free(vet_asphalt_saida);
	free(vet_grass_saida);
	free(vet_asphalt_aux);
	free(vet_grass_aux);

}


