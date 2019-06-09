// INF110 - Trabalho pratico 3
//
// programa para ler, modificar e gravar uma imagem no formato PNM
//
// Autores: Andre Gustavo dos Santos         (criado em 16/06/14)
//          Andre Gustavo dos Santos         (modificado em 22/05/18)
//          Andre Gustavo dos Santos         (modificado em 21/05/19)
//			Renan Lopes Silva				(modificado em 08/06/2019)

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cmath>
#include <time.h>
#include <stdlib.h>


using namespace std;

//tamanho maximo aceito (pode ser alterado)
const int MAXALTURA = 3000; 
const int MAXLARGURA = 3000;
const int RGB = 3; //NAO PODE SER ALTERADO

unsigned char imagem[MAXALTURA][MAXLARGURA];
unsigned char imagemRgb[MAXALTURA][MAXLARGURA][RGB];
unsigned char sobelM[MAXALTURA][MAXLARGURA][RGB];
int sobelX[MAXALTURA][MAXLARGURA][RGB];
int sobelY[MAXALTURA][MAXLARGURA][RGB];



//Funcoes

void escurecimento(bool aleatorio, int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void escurecimento(bool aleatorio, int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);

void clareamento(bool aleatorio, int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void clareamento(bool aleatorio, int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);

void negativo(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void negativo(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);

void espelharVertical(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void espelharVertical(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);

void espelharHorizontal(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void espelharHorizontal(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);

void tonsDeCinza(int altura, int largura,unsigned char imagemRgb[][MAXLARGURA][RGB]);

void sobel(int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void sobel(int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);

void mudancaDeCor(bool aleatorio, int altura, int largura, int valor);

int fatorValidacao();
int random(int limite, bool mudancaDeCorAleatorio);

void aleatorio(bool aleatorioFuncoes, int valor, int altura, int largura, int tipoImagem);


int main() {
	int largura, altura;                  //dimensoes da imagem
	char tipo[4];						  //tipo da imagem
	ifstream arqentrada;				  //arquivo que contem a imagem original
	ofstream arqsaida;					  //arquivo que contera a imagem gerada
	char comentario[200], c;			  //auxiliares
	int i, j, k, tratamento, valor;	      //auxiliares
	int tratamentoNumero;
	string nomeArquivo; 
	bool sair = false, aleatorioFuncoes = false, mudancaDeCorAleatorio = false;

	//*** LEITURA DA IMAGEM ***//
	//inicialmente nao sera necessario entender nem mudar nada nesta parte

	//*** Abertura do arquivo ***//
	while(true){
		cout << "Qual o nome do aqruivo a ser editado?" << endl;
		cin >> nomeArquivo;
		nomeArquivo += ".pnm";
		cout << endl << "informacoes do arquivo:" << endl;

		arqentrada.open(nomeArquivo, ios::in); //Abre arquivo para leitura
		if (!arqentrada){
			cout << "Nao consegui abrir chamado :  " << nomeArquivo << endl << "Tente novamente" << endl;
		}
		else{
			break;
		}
	}

	//***************************//

	//*** Leitura do cabecalho ***//
	
	arqentrada >> tipo; 	//Le o tipo de arquivo
	arqentrada.get();		//Le e descarta o \n do final da 1a. linha
	int tipoImagem;
	cout << tipo;

	if (strcmp(tipo, "P2") == 0){
		cout << "Imagem em tons de cinza\n";
		tipoImagem = 2;
	}

	if (strcmp(tipo, "P3") == 0){
		cout << "Imagem colorida\n";
		tipoImagem = 3;
	}

	else if (strcmp(tipo, "P5") == 0 || strcmp(tipo, "P6") == 0){
		cout << "Imagem no formato RAW\n";
		cout << "Desculpe, mas nao trabalho com esse tipo de imagem.\n";
		arqentrada.close();
		return 0;
	}

	
	while ((c = arqentrada.get()) == '#')	   //Enquanto for comentario
		arqentrada.getline(comentario, 200);   //Le e descarta a linha "inteira"

	arqentrada.putback(c);                     //Devolve o caractere lido para a entrada, pois como nao era comentario, era o primeiro digito da largura

	arqentrada >> largura >> altura;           //Le o numero de pixels da horizontal e vertical
	cout << "Tamanho: " << largura << " x " << altura << endl;
	
	// Aviso de altura e largura maximas
	if (largura > MAXLARGURA){
		cout << "Desculpe, mas ainda nao trabalho com imagens com mais de " << MAXLARGURA << " pixels de largura.\n";
		arqentrada.close();
		return 0;
	}
	if (largura > MAXALTURA){
		cout << "Desculpe, mas ainda nao trabalho com imagens com mais de " << MAXALTURA << " pixels de altura.\n";
		arqentrada.close();
		return 0;
	}
	
	arqentrada >> valor; //Valor maximo do pixel (temos que ler, mas nao sera usado)
	//****************************//
	
	//*** Leitura dos pixels da imagem ***//
	if (tipoImagem == 2)
		for (int i = 0; i < altura; i++)
			for (int j = 0; j < largura; j++){
					arqentrada >> valor;
				imagem[i][j] = (unsigned char)valor;
			}
	
	if (tipoImagem == 3){
		for (int i = 0; i < altura; i++)
			for (int j = 0; j < largura; j++){
				for (int k = 0; k < RGB; k++){
					arqentrada >> valor;
					imagemRgb[i][j][k] = (unsigned char)valor;

				}
			}
	}
	
	//************************************//

	arqentrada.close();

	//*** FIM DA LEITURA DA IMAGEM ***//


	//*** TRATAMENTO DA IMAGEM ***//


	
	//Listagem das	 opcoes
	if (tipoImagem == 3){ 
		cout << endl << "Qual da seguintes acoes voce quer execultar?\n";
		cout << endl << "0 - Sair\n1 - Escurecer\n2 - Clarear \n3 - Negativo\n4 - Espelhar Vertical\n5 - Espelhar Horizontal\n6 - Tons de Cinza\n7 - Sobel\n8 - Mundanca de Cor\n9 - Aleatorio\n";
		cin >> tratamentoNumero;
	}
	if (tipoImagem == 2){
		cout << endl << "Qual da seguintes acoes voce quer execultar?\n";
		cout << endl << "0 - Sair\n1 - Escurecer\n2 - Clarear \n3 - Negativo\n4 - Espelhar Vertical\n5 - Espelhar Horizontal\n6 - Sobel\n7 - Aleatorio\n";
		cin >> tratamentoNumero;
	}
	cout << endl;

	// caso a imagem seja colorida
	if (tipoImagem == 3){ 
		switch (tratamentoNumero){
			case 0:
				break;
			case 1:
				escurecimento(aleatorio, valor, altura, largura, imagemRgb);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 2:
				clareamento(aleatorio, valor, altura, largura, imagemRgb);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 3:
				negativo(valor, altura, largura, imagemRgb);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 4:
				espelharVertical(valor, altura, largura, imagemRgb);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 5:
				espelharHorizontal(valor, altura, largura, imagemRgb);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 6:
				tonsDeCinza(altura, largura, imagemRgb);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 7:
				sobel(altura, largura, imagemRgb);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 8:
				mudancaDeCor(aleatorio, altura, largura, valor);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 9:
				aleatorio(aleatorioFuncoes,valor,altura, largura, tipoImagem);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			default:
				cout << "---- Opcao invalida ----\n";
				break;
		}
	}	

	// caso a imagem nao seja colirida
	if (tipoImagem == 2){ 

		switch (tratamentoNumero){
			case 0:
				break;
			case 1:
				escurecimento(aleatorio, valor, altura, largura, imagem);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 2:
				clareamento(aleatorio, valor, altura, largura, imagem);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 3:
				negativo(valor, altura, largura, imagem);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 4:
				espelharVertical(valor, altura, largura, imagem);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 5:
				espelharHorizontal(valor, altura, largura, imagem);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 6:
				sobel(altura, largura, imagem);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			case 7:
				aleatorio(aleatorioFuncoes,valor,altura, largura, tipoImagem);
				cout << "\n---- Efeito aplicado com sucesso ----\n";
				break;
			default:
				cout << "---- Opcao invalida ----\n";
				break;
		}
	}
		
	//*** FIM DO TRATAMENTO DA IMAGEM ***//

	//*** GRAVACAO DA IMAGEM ***//
	//inicialmente nao sera necessario entender nem mudar nada nesta parte

	//*** Grava a nova imagem ***//

	string novaImagem;
	char salvarNovo = false;
	while (true){
		cout << "Deseja salvar o arquivo como uma nova imagem? (S / N) \n";
		cin >> salvarNovo;

		if (salvarNovo == 'S' || salvarNovo == 's' || salvarNovo == 'N' || salvarNovo == 'n'){
			break;
		}
		else{
			cout << "Valor invalido. Tente novamente\n";
		}
	}

	if (salvarNovo == 'S' || salvarNovo == 's'){
		cout << "Como deseja chamar sua nova imagem?\n";
		cin >> novaImagem;
		novaImagem += ".pnm";
		arqsaida.open(novaImagem, ios::out); //Abre arquivo para escrita
		if (!arqsaida){
			cout << endl << "Nao consegui criar " << novaImagem << endl;
			return 0;
		}
	}

	else {
		arqsaida.open(nomeArquivo, ios::out); //Abre arquivo para escrita
		if (!arqsaida){
			cout << endl << "Nao consegui criar " << nomeArquivo << endl;
			return 0;
		}
	}

	arqsaida << tipo << endl;									  //tipo
	arqsaida << "# Trabalho 3 INF 110 - Renan Lopes\n";			  //comentario
	arqsaida << largura << " " << altura << endl;                 //dimensoes
	arqsaida << 255 << endl;

	
	if (tipoImagem == 2){
		for (i = 0; i < altura; i++)
			for (j = 0; j < largura; j++)
				arqsaida << (int)imagem[i][j] << endl;
	}

	else if (tipoImagem == 3){
		for (i = 0; i < altura; i++)
			for (j = 0; j < largura; j++)
				for (int k = 0; k < RGB ; k++)
					arqsaida << (int)imagemRgb[i][j][k] << endl;
	}


	//fecha o arquivo
	arqsaida.close(); 

	cout << "Imagem salva com sucesso\n";

	//***************************//

	//*** FIM DA GRAVACAO DA IMAGEM ***//

	return 0;
}

void escurecimento(bool aleatorioFuncoes, int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){

	int fator;

	if (aleatorioFuncoes == false){
		cout << "Qual o fator de escurecimento (1-100) ? \n";
		cin >> fator;
	}
	else {
		fator = random(100, false);
	}

	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
	
			valor = (int)imagem[i][j];	          //pega o valor do pixel
			valor -= fator;				          //escurece o pixel
			if (valor < 0)		                  //se der negativo
				valor = 0;			              // deixa preto
			imagem[i][j] = (unsigned char)valor;  //modifica o pixel
		}
	}

}

void escurecimento(bool aleatorioFuncoes, int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){

	int fator;

	if (aleatorioFuncoes == false){
		cout << "Qual o fator de escurecimento (1-100) ? \n";
		cin >> fator;
	}
	else {
		fator = random(100, false);
	}

	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			for (int k = 0; k < RGB; k++){
				valor = (int)imagemRgb[i][j][k];	        //pega o valor do pixel
				valor -= fator;				                //escurece o pixel
				if (valor < 0)		                        //se der negativo
					valor = 0;			                    // deixa preto
				imagemRgb[i][j][k] = (unsigned char)valor;  //modifica o pixel
			}
		}
	}
}

void clareamento(bool aleatorioFuncoes, int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){
	
	int fator;
		
	if (aleatorioFuncoes == false){
		cout << "Qual o fator do clareamento (1-100) ? \n";
		cin >> fator;
	}
	else{
		fator = random(100, false);
	}

	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
	
			valor = (int)imagem[i][j];	          //pega o valor do pixel
			valor += fator;				          //clareare o pixel
			if (valor > 255)		              //se der maior que 255
				valor = 255;			          // deixa branco
			imagem[i][j] = (unsigned char)valor;  //modifica o pixel
		}
	}
}

void clareamento(bool aleatorioFuncoes, int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){

	int fator;

	if (aleatorioFuncoes == false){
		cout << "Qual o fator do clareamento (1-100) ? \n";
		cin >> fator;
	}
	else{
		fator = random(100, false);
	}

	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			for (int k = 0; k < RGB; k++){
				valor = (int)imagemRgb[i][j][k];	        //pega o valor do pixel
				valor += fator;				                //clareare o pixel
				if (valor > 255)		                    //se der maior que 255
					valor = 255;			                // deixa branco
				imagemRgb[i][j][k] = (unsigned char)valor;  //modifica o pixel
			}
		}
	}
}

void negativo(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){
	
	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			valor = (int)imagem[i][j]; //pega o valor do pi
			valor = 255 - valor;
			imagem[i][j] = (unsigned char)valor;
		}
	}
}

void negativo(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){
	
	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			for (int k = 0; k < RGB; k++){
				valor = 255 - (int)imagemRgb[i][j][k]; 
				imagemRgb[i][j][k] = (unsigned char)valor;
			}
		}
	}
}

void espelharVertical(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){
	for (int i = 0; i < altura; i++){
		for (int j = 0; j < (largura/2); j++){
			valor = (int)imagem[i][largura-1-j];
			imagem[i][largura-1-j] = imagem[i][j];
			imagem[i][j] = (unsigned char)valor;
		}
	}
}	

void espelharVertical(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){
	for (int i = 0; i < altura; i++){
		for (int j = 0; j < (largura/2); j++){
			for (int k = 0; k < RGB; k++){
				valor = (int)imagemRgb[i][largura-1-j][k];
				imagemRgb[i][largura-1-j][k] = imagemRgb[i][j][k];
				imagemRgb[i][j][k] = (unsigned char)valor;
			}
		}
	}
}

void espelharHorizontal(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){
	for (int i = 0; i < altura/2; i++){
		for (int j = 0; j < (largura); j++){
			valor = (int)imagem[altura-1-i][j];
			imagem[altura-1-i][j] = imagem[i][j];
			imagem[i][j] = (unsigned char)valor;
		}
	}
}

void espelharHorizontal(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){
	for (int i = 0; i < altura/2; i++){
		for (int j = 0; j < (largura); j++){
			for (int k = 0; k < RGB; k++){
				valor = (int)imagemRgb[altura-1-i][j][k];
				imagemRgb[altura-1-i][j][k] = imagemRgb[i][j][k];
				imagemRgb[i][j][k] = (unsigned char)valor;
			}
		}
	}
}

void tonsDeCinza(int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){
	int corCinza;

	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura ; j++){
			for (int k = 0; k < RGB ; k++){
				corCinza += imagemRgb[i][j][k]; 
			}
			for (int k = 0; k < RGB; k++){
				imagemRgb[i][j][k]= (corCinza/3.0);
			}

			corCinza = 0;
		}
	}
}

void sobel(int altura, int largura, unsigned char imagem[][MAXLARGURA]){

    float kernelX[3][3] =
    { { -1, 0, 1 },
      { -2, 0, 2 },
      { -1, 0, 1 } };

    float kernelY[3][3] =
    { { -1, -2, -1 },
      { 0,  0,  0 },
      { 1,  2,  1 } };

	for(int i = 1; i < altura - 1; i++) { 
		for(int j = 1; j < largura - 1; j++) {
			for(int k = 0; k < RGB; k++) {

				sobelX[i][j][k] = (int)( (imagemRgb[i-1][j-1][k] * kernelX[1][1]) + (imagemRgb[i-1][j+1][k] * kernelX[1][3]) + (imagemRgb[i][j-1][k] * kernelX[2][1]) + (imagemRgb[i][j+1][k] * kernelX[2][3]) + (imagemRgb[i+1][j-1][k] * kernelX[3][1]) + (imagemRgb[i+1][j+1][k] * kernelX[3][3]) );

				if (sobelX[i][j][k] > 225) sobelX[i][j][k] = 255;
				if (sobelX[i][j][k] < 0) sobelX[i][j][k]= 0;
				
				sobelY[i][j][k] = (int)( (imagemRgb[i-1][j-1][k] * kernelY[1][1]) + (imagemRgb[i-1][j][k] * kernelY[1][2]) + (imagemRgb[i-1][j+1][k] * kernelY[1][3]) + (imagemRgb[i+1][j-1][k] * kernelY[3][1]) + (imagemRgb[i+1][j][k] * kernelY[3][2]) + (imagemRgb[i+1][j+1][k] * kernelY[3][3]) );

				if (sobelY[i][j][k] > 225) sobelY[i][j][k] = 255;
				if (sobelY[i][j][k] < 0) sobelY[i][j][k]= 0;

			}			
		}
	}

	for(int i = 1; i < altura -1; i++) {
		for(int j = 1; j < largura -1; j++) {
			for(int k = 0; k < RGB; k++) {
				sobelM[i][j][k] = (sobelX[i][j][k] + sobelY[i][j][k]) / 2;

				//sobelM[i][j][k] = sqrt(sobelX[i][j][k]*sobelX[i][j][k] + sobelY[i][j][k]*sobelY[i][j][k]);
			}
		}
	}

	for(int i = 1; i < altura -1; i++) {
		for(int j = 1; j < largura -1; j++) {
			for(int k = 0; k < RGB; k++) {
				imagemRgb[i][j][k] = (unsigned char)sobelM[i][j][k]; // Sobrescreve a imagem com as mudancas;
			}
		}
	}
	
}

void sobel(int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){

    float kernelX[3][3] =
    { { -1, 0, 1 },
      { -2, 0, 2 },
      { -1, 0, 1 } };

    float kernelY[3][3] =
    { { -1, -2, -1 },
      { 0,  0,  0 },
      { 1,  2,  1 } };

	for(int i = 1; i < altura - 1; i++) { 
		for(int j = 1; j < largura - 1; j++) {
			for(int k = 0; k < RGB; k++) {

				sobelX[i][j][k] = (int)( (imagemRgb[i-1][j-1][k] * kernelX[1][1]) + (imagemRgb[i-1][j+1][k] * kernelX[1][3]) + (imagemRgb[i][j-1][k] * kernelX[2][1]) + (imagemRgb[i][j+1][k] * kernelX[2][3]) + (imagemRgb[i+1][j-1][k] * kernelX[3][1]) + (imagemRgb[i+1][j+1][k] * kernelX[3][3]) );

				if (sobelX[i][j][k] > 225) sobelX[i][j][k] = 255;
				if (sobelX[i][j][k] < 0) sobelX[i][j][k]= 0;
				
				sobelY[i][j][k] = (int)( (imagemRgb[i-1][j-1][k] * kernelY[1][1]) + (imagemRgb[i-1][j][k] * kernelY[1][2]) + (imagemRgb[i-1][j+1][k] * kernelY[1][3]) + (imagemRgb[i+1][j-1][k] * kernelY[3][1]) + (imagemRgb[i+1][j][k] * kernelY[3][2]) + (imagemRgb[i+1][j+1][k] * kernelY[3][3]) );

				if (sobelY[i][j][k] > 225) sobelY[i][j][k] = 255;
				if (sobelY[i][j][k] < 0) sobelY[i][j][k]= 0;

			}			
		}
	}

	for(int i = 1; i < altura -1; i++) {
		for(int j = 1; j < largura -1; j++) {
			for(int k = 0; k < RGB; k++) {
				sobelM[i][j][k] = (sobelX[i][j][k] + sobelY[i][j][k]) / 2;

				//sobelM[i][j][k] = sqrt(sobelX[i][j][k]*sobelX[i][j][k] + sobelY[i][j][k]*sobelY[i][j][k]);
			}
		}
	}

	for(int i = 1; i < altura -1; i++) {
		for(int j = 1; j < largura -1; j++) {
			for(int k = 0; k < RGB; k++) {
				imagemRgb[i][j][k] = (unsigned char)sobelM[i][j][k]; // Sobrescreve a imagem com as mudancas;
			}
		}
	}
	
}

void convolution(int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){

	int kernelAltura = 3, kernelLargura = 3, contador;

	int kernel[3][3]{
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1}
	};


	for (int i = 1; i < altura - 1; i++){
		for (int j = 1; j < largura -1; j++){
			contador = 0;
			for (int k = 0; k < RGB; k++){

				for (int l = 0 ; l < kernelAltura; l++){
					for (int m = 0; m < kernelLargura; m++){
						contador += (int)imagemRgb[i][j][k] * kernel[l][m];
					}
				}

				sobelM[i][j][k] = (unsigned char)contador;
			}
		}
	}

	for(int i = 1; i < altura -1; i++) {
		for(int j = 1; j < largura -1; j++) {
			for(int k = 0; k < RGB; k++) {
				imagemRgb[i][j][k] = (unsigned char)sobelM[i][j][k]; // Sobrescreve a imagem com as mudancas;
			}
		}
	}
}

void mudancaDeCor(bool aleatorioFuncoes, int altura, int largura, int valor){

	//Variaveis e auxiliares mudancaDeCor
	bool red = false, green = false, blue = false;
	int fatorR, fatorG, fatorB;

	// Definicao dos valores a serem a alterados
	if (aleatorioFuncoes == false){
		cout << "Vermelho\n";
		fatorR = fatorValidacao();
		if (fatorR != 0) red = true;
		cout << "Verde\n";
		fatorG = fatorValidacao();
		if (fatorG != 0) green = true;
		cout << "Azul\n";
		fatorB = fatorValidacao();
		if (fatorB != 0) blue = true;
	}
	else{
		fatorR = random(100, true);
		red = true;
		fatorG = random(100, true);
		green = true;
		fatorB = random(100, true);
		blue = true;
	}

	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			for (int k = 0; k < 3; k++){

				if (k == 0 && red == true){

					valor = (int)imagemRgb[i][j][k];
					valor += fatorR;

					if (valor > 255) valor = 255;
					if (valor < 0) valor = 0;

				}

				if (k == 1 && green == true){

					valor = (int)imagemRgb[i][j][k];
					valor += fatorG;

					if (valor > 255) valor = 255;
					if (valor < 0) valor = 0;

				}

				if (k == 2 && blue ==  true){

					valor = (int)imagemRgb[i][j][k];
					valor += fatorB;

					if (valor > 255) valor = 255;
					if (valor < 0) valor = 0;

				}

				imagemRgb[i][j][k] = (unsigned char)valor;


			}
		}
	}

}

int fatorValidacao(){
	int fator;

	cout << "Qual o fator da mudanca (-100 a 100, 0 = sem mudancas) ? \n";
	cin >> fator;
	while (fator < -100 || fator > 100){
		cout << "Valor invalido\n";
		cout << "Qual o fator da mudanca (-110 a 100, 0 = sem mudancas) ? \n";
		cin >> fator;
	}

	return fator;
}

int random(int limite, bool mudancaDeCorAleatorio){
	// Gerar valores para a funcao aleatorio
	int seed;
    srand (time(NULL));
    seed = rand() % limite + 1;

	if (mudancaDeCorAleatorio == true){  // Definicao se o valor sera negativo ou possitivo
		if ((seed % 2) == 0 ){
			seed = seed * -1;
		}
	}
	return seed; 
}

void aleatorio(bool aleatorioFuncoes, int valor, int altura, int largura, int tipoImagem){

	int tratamentoNumero, repetir, contador = 0; 
	aleatorioFuncoes = true; //Ativa o modo aleatorio, nas funcoes

		
		if (tipoImagem == 3){ 

			tratamentoNumero = random(8, false); //Definicao aleatoria do efeito

			switch (tratamentoNumero){
				case 1:
					escurecimento(aleatorioFuncoes, valor, altura, largura, imagemRgb);
					cout << "\nEscurecimento Aplicado\n";
					break;
				case 2:
					clareamento(aleatorioFuncoes, valor, altura, largura, imagemRgb);
					cout << "\n Clareamento Aplicado\n";
					break;
				case 3:
					negativo(valor, altura, largura, imagemRgb);
					cout << "\nNegativo Aplicado\n";
					break;
				case 4:
					espelharVertical(valor, altura, largura, imagemRgb);
					cout << "Espelhamento Vertical Aplicado\n";
					break;
				case 5:
					espelharHorizontal(valor, altura, largura, imagemRgb);
					cout << "\nEspelhamento Horizontal APlicado\n";
					break;
				case 6:
					tonsDeCinza(altura, largura, imagemRgb);
					cout << "\nImagem Convertida a Tons De Cinza\n";
					break;
				case 7:
					sobel(altura, largura, imagemRgb);
					cout << "\nSobel Aplicado\n";
					break;
				case 8:
					mudancaDeCor(aleatorioFuncoes, altura, largura, valor);
					cout << "\nMudanca de Cor Aplicada\n";
					break;
			}

			contador++; //Aumenta contador
		}
		
		if (tipoImagem == 2){ 

			tratamentoNumero = random(6, false); //Definicao aleatoria do efeito

			switch (tratamentoNumero){

				case 1:
					escurecimento(aleatorio, valor, altura, largura, imagem);
					cout << "\nEscurecimento Aplicado\n";
					break;
				case 2:
					clareamento(aleatorio, valor, altura, largura, imagem);
					cout << "\n Clareamento Aplicado\n";
					break;
				case 3:
					negativo(valor, altura, largura, imagem);
					cout << "\nNegativo Aplicado\n";
					break;
				case 4:
					espelharVertical(valor, altura, largura, imagem);
					cout << "Espelhamento Vertical Aplicado\n";
					break;
				case 5:
					espelharHorizontal(valor, altura, largura, imagem);
					cout << "\nEspelhamento Horizontal APlicado\n";
					break;
				case 6:
					sobel(altura, largura, imagem);
					cout << "\nSobel Aplicado\n";
					break;
			}

			contador++; //Aumenta contador
			
		}


}