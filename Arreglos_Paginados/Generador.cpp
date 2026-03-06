#include <iostream>
#include <cmath>
#include <string>

using namespace std;

int main(int argc,char* argv[]) {
	if (argc < 5) {
		cout << "Cantidad de parametros incorrectos";
		return 1;
	}
	long long PesoBytes = 0;
	string Tamanho= argv[2];
	string Path = argv[4];
	if (string(argv[0]) != "generator") {
		cout << "Nombre del proyecto incorrecto";
		return 1;
	}
	if (string(argv[1]) != "-size") {
		cout << "Parametro -size incorrecto";
		return 1;
	}
	if (string(argv[3]) != "-output") {
		cout << "Parametro -output incorrecto";
		return 1;
	}
	if (Tamanho== "SMALL") {
		PesoBytes= 512LL*pow(1024,2);
	}
	else if (Tamanho=="MEDIUM") {
		PesoBytes= 1LL *pow(1024, 3);
	}
	else if(Tamanho=="LARGE") {
		PesoBytes= 2LL*pow(1024, 3);
	}
	else {
		cout << "Tamanho invalido";
		return 1;
	}
	return 0;
}

