// INF110 - Trabalho pratico 3
//
// programa para ler, modificar e gravar uma imagem no formato PNM
//
// Autores: Andre Gustavo dos Santos         (criado em 16/06/14)
//          Andre Gustavo dos Santos         (modificado em 22/05/18)
//          Andre Gustavo dos Santos         (modificado em 21/05/19)

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

const int MAXALTURA = 500; //tamanho maximo aceito (pode ser alterado)
const int MAXLARGURA = 500;
const int RGB = 3;

unsigned char imagem[MAXALTURA][MAXLARGURA]; //a imagem propriamente dita
unsigned char imagemRgb[MAXALTURA][MAXLARGURA][RGB];

using namespace std;

// FUNCOES
void escurecimento(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void clareamento(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void negativo(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void espelharVertical(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void espelharHorizontal(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]);
void tonsDeCinza(int tratamento, int altura, int largura, unsigned char imagem[MAXALTURA][MAXLARGURA] ,unsigned char imagemRgb[][MAXLARGURA][RGB]);
void escurecimento(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);
void clareamento(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);
void negativo(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);
void espelharVertical(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]);

int main() {
	int largura, altura;				 //dimensoes da imagem
	char tipo[4];						 //tipo da imagem
	ifstream arqentrada;				 //arquivo que contem a imagem original
	ofstream arqsaida;					 //arquivo que contera a imagem gerada
	char comentario[200], c;			//auxiliares
	int i, j, k, tratamento, valor;	//auxiliares
	int tratamentoNumero;
	string nomeArquivo; 

	//*** LEITURA DA IMAGEM ***//
	//inicialmente nao sera necessario entender nem mudar nada nesta parte

	//*** Abertura do arquivo ***//
	cout << "Qual o nome do aqruivo a ser editado?" << endl;
 	cin >> nomeArquivo;
	nomeArquivo += ".pnm";
	cout << endl << "informacoes do arquivo:" << endl;

	arqentrada.open(nomeArquivo, ios::in); //Abre arquivo para leitura
	if (!arqentrada){
		cout << "Nao consegui abrir chamado :  " << nomeArquivo << endl << "Tente novamente" << endl;
		return 0;
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

	
	while ((c = arqentrada.get()) == '#')	//Enquanto for comentario
		arqentrada.getline(comentario, 200); //Le e descarta a linha "inteira"

	arqentrada.putback(c); //Devolve o caractere lido para a entrada, pois como
												 //nao era comentario, era o primeiro digito da largura

	arqentrada >> largura >> altura; //Le o numero de pixels da horizontal e vertical
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
	// EXCLUIR DEPOIS //
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

	cout << endl << "Qual da seguintes acoes voce quer execultar?\n";
	cout << endl << "0 - Sair\n1 - Escurecer\n2 - Clarear \n3 - Negativo\n4 - Espelhar Vertical\n5 - Espelhar Horizontal\n6 - Tons de Cinza\n";
	cin >> tratamentoNumero;


	if (tipoImagem == 3){ // caso a imagem seja colorida
		switch (tratamentoNumero){
			case 0:
				break;
			case 1:
				escurecimento(valor, altura, largura, imagemRgb);
				break;
			case 2:
				clareamento(valor, altura, largura, imagemRgb);
				break;
			case 3:
				negativo(valor, altura, largura, imagemRgb);
			case 4:
				espelharVertical(valor, altura, largura, imagemRgb);
			case 5:
				espelharHorizontal(valor, altura, largura, imagem);
			case 6:
				tonsDeCinza(tratamento, altura, largura, imagem, imagemRgb);
			default:
				break;
		}
	}	
	else{ // caso a imagem nao seja colirida

		switch (tratamentoNumero){
			case 0:
				break;
			case 1:
				escurecimento(valor, altura, largura, imagem);
				break;
			case 2:
				clareamento(valor, altura, largura, imagem);
				break;
			case 3:
				negativo(valor, altura, largura, imagem);
			case 4:
				espelharVertical(valor, altura, largura, imagem);
			case 5:
				espelharHorizontal(valor, altura, largura, imagem);
			default:
				break;
		}
	}
	
	//*** FIM DO TRATAMENTO DA IMAGEM ***//

	//*** GRAVACAO DA IMAGEM ***//
	//inicialmente nao sera necessario entender nem mudar nada nesta parte

	//*** Grava a nova imagem ***//

	string novaImagem;
	char salvarNovo = false;
	cout << "Deseja salvar o arquivo como uma nova imagem? (S / N) \n";
	cin >> salvarNovo;

	if (salvarNovo == 'S' || salvarNovo == 's'){
		cout << "Como deseja chamar sua nova imagem?\n";
		cin >> novaImagem;
		novaImagem += ".pnm";
		arqsaida.open(novaImagem, ios::out); //Abre arquivo para escrita
		if (!arqsaida){
			cout << "Nao consegui criar " << novaImagem << endl;
			return 0;
		}
	}

	else {
		arqsaida.open(nomeArquivo, ios::out); //Abre arquivo para escrita
		if (!arqsaida){
			cout << "Nao consegui criar " << nomeArquivo << endl;
			return 0;
		}
	}

	arqsaida << tipo << endl;											//tipo
	arqsaida << "# Trabalho 3 INF 110 - Renan Lopes\n";											//comentario
	arqsaida << largura << " " << altura << endl; //dimensoes
	arqsaida << 255 << endl;

	
	if (tipoImagem == 2){//maior valor
		for (i = 0; i < altura; i++)
			for (j = 0; j < largura; j++)
				arqsaida << (int)imagem[i][j] << endl; //pixels
	}

	else if (tipoImagem == 3){
		for (i = 0; i < altura; i++)
			for (j = 0; j < largura; j++)
				for (int k = 0; k < RGB ; k++)
					arqsaida << (int)imagemRgb[i][j][k] << endl; //pixels
	}


	arqsaida.close(); //fecha o arquivo

	cout << "Imagem salva com sucesso\n";

	//***************************//

	//*** FIM DA GRAVACAO DA IMAGEM ***//

	return 0;
}


//Funcao para imagens em tons de cinza
void escurecimento(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){

	int fator;
	cout << "Qual o fator de escurecimento (1-100) ? ";
	cin >> fator;

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

//Funcao para imagens coloridas
void escurecimento(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){

	int fator;
	cout << "Qual o fator de escurecimento (1-100) ? ";
	cin >> fator;

	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			for (int k = 0; k < RGB; k++){
				valor = (int)imagemRgb[i][j][k];	          //pega o valor do pixel
				valor -= fator;				          //escurece o pixel
				if (valor < 0)		                  //se der negativo
					valor = 0;			              // deixa preto
				imagemRgb[i][j][k] = (unsigned char)valor;  //modifica o pixel
			}
		}
	}
}

//Funcao para imagens em tons de cinza
void clareamento(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){

	int fator;
	cout << "Qual o fator de clarear (1-100) ? ";
	cin >> fator;

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

//Funcao para imagens coloridas
void clareamento(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){

	int fator;
	cout << "Qual o fator de clarear (1-100) ? ";
	cin >> fator;

	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			for (int k = 0; k < RGB; k++){
				valor = (int)imagemRgb[i][j][k];	          //pega o valor do pixel
				valor += fator;				          //clareare o pixel
				if (valor > 255)		              //se der maior que 255
					valor = 255;			          // deixa branco
				imagemRgb[i][j][k] = (unsigned char)valor;  //modifica o pixel
			}
		}
	}
}

// Funcao para imagem em tons de cinza
void negativo(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){
	
	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			valor = (int)imagem[i][j]; //pega o valor do pi
			valor = 255 - valor;
			imagem[i][j] = (unsigned char)valor;
		}
	}
}

//Funcao para imagens coloridas
void negativo(int valor, int altura, int largura, unsigned char imagemRgb[][MAXLARGURA][RGB]){
	
	for (int i = 0; i < altura; i++){
		for (int j = 0; j < largura; j++){
			for (int k = 0; k < RGB; k++){
				valor = (int)imagemRgb[i][j][k]; //pega o valor do pi
				valor = 255 - valor;
				imagemRgb[i][j][k] = (unsigned char)valor;
			}
		}
	}
}

//Funcao para imagens em tons de cinza
void espelharVertical(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){
	for (int i = 0; i < altura; i++){
		for (int j = 0; j < (largura/2); j++){
			valor = (int)imagem[i][largura-1-j];
			imagem[i][largura-1-j] = imagem[i][j];
			imagem[i][j] = (unsigned char)valor;
		}
	}
}	

//Funcao para imagens coloridas
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

//funcao 
void espelharHorizontal(int valor, int altura, int largura, unsigned char imagem[][MAXLARGURA]){
	for (int i = 0; i < altura/2; i++){
		for (int j = 0; j < (largura); j++){
			valor = (int)imagem[altura-1-i][j];
			imagem[altura-1-i][j] = imagem[i][j];
			imagem[i][j] = (unsigned char)valor;
		}
	}
}

void tonsDeCinza(int tratamento, int altura, int largura, unsigned char imagem[][MAXLARGURA] ,unsigned char imagemRgb[][MAXLARGURA][RGB]){
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
