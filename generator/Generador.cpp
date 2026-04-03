#include <iostream>
#include <cmath>
#include <string>
#include <fstream>


using namespace std;

//Generador de archivos binarios con numeros aleatorios
void GenerarArchivo(string Path, long long PesoBytes) {
	fstream archivo(Path, ios::out | ios::binary);
	if (!archivo.is_open()) {
		cout << "Error al abrir archivo";
	}
	srand(time(0)); //Crea una semilla para generar numeros aleatorios
	for (long long i = 0; i < PesoBytes / sizeof(int); i++) {
		int numero = rand();
		archivo.write((char*)&numero, sizeof(int)); //Funcion de escritura de los numeros
	}
	archivo.close();
}
//Validacion de parametros de entrada de linea de comandos
int main(int argc,char* argv[]) {
	if (argc < 5) {
		cout << "Cantidad de parametros incorrectos";
		return 1;
	}
	long long PesoBytes = 0;
	string Tamanho= argv[2];
	string Path = argv[4];
	if (string(argv[1]) != "-size") {
		cout << "Parametro -size incorrecto";
		return 1;
	}
	if (string(argv[3]) != "-output") {
		cout << "Parametro -output incorrecto";
		return 1;
	}
	if (Tamanho == "P1") {
		PesoBytes = 8LL * pow(1024, 2);
	}
	else if (Tamanho == "P2") {
		PesoBytes = 16LL * pow(1024, 2);
	}
	else if (Tamanho == "P3") {
		PesoBytes = 32LL * pow(1024, 2);
	}
	else if (Tamanho == "P4") {
		PesoBytes = 64LL * pow(1024, 2);
	}
	else if (Tamanho == "P5") {
		PesoBytes = 128LL * pow(1024, 2);
	}
	else if (Tamanho== "SMALL") {
		PesoBytes= 256LL*pow(1024,2);
	}
	else if (Tamanho=="MEDIUM") {
		PesoBytes= 512LL *pow(1024, 2);
	}
	else if(Tamanho=="LARGE") {
		PesoBytes= 1LL*pow(1024, 3);
	}
	else {
		cout << "Tamanho invalido";
		return 1;
	}
	GenerarArchivo(Path, PesoBytes);
	return 0;
}





