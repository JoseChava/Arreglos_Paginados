#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//Funcion para copiar el archivo creado con generator binario para poder manipularlo
void CopiarArchivo(string PathOriginal, string PathSalida) {
	ifstream archivoOriginal(PathOriginal, ios::binary);
	ofstream archivoSalida(PathSalida, ios::binary);
	int numero;
	while (archivoOriginal.read((char*)&numero, sizeof(int))) { //El ciclo para una vez que NO se pueda leer otros 4 bytes (un int)
		archivoSalida.write((char*)&numero, sizeof(int));
	}
	archivoOriginal.close();
	archivoSalida.close();
}

bool ArchivoExiste(string PathOriginal) {
	ifstream archivo(PathOriginal);
	return archivo.good();
}

int main(int argc, char* argv[]) {
	//Se definen las validaciones de la linea de comandos
	if (argc < 11) {
		cout << "Cantidad de parametros incorrectos";
		return 1;
	}
	string PathOriginal= argv[2];
	string PathSalida= argv[4];
	string AlgoritmoOrdenamiento= argv[6];
	int  TamanhoPagina = stoi(argv[8]);
	int PaginasEnRam = stoi(argv[10]);
	if (string(argv[0]) != "sorter") {
		cout << "Nombre del proyecto incorrecto";
		return 1;
	}
	if (string(argv[1]) != "-input") {
		cout << "Parametro -input incorrecto";
		return 1;
	}
	if (!ArchivoExiste(PathOriginal)) {
		cout << "No se encuentra el archivo";
		return 1;
	}
	if (string(argv[3]) != "-output") {
		cout << "Parametro -output incorrecto";
		return 1;
	}
	if (string(argv[5]) != "-alg") {
		cout << "Parametro -alg incorrecto";
		return 1;
	}
	if (string(argv[7]) != "-pageSize") {
		cout << "Parametro -pageSize incorrecto";
		return 1;
	}
	if (string(argv[9]) != "-pageCount") {
		cout << "Parametro -pageCount incorrecto";
		return 1;
	}
	CopiarArchivo(PathOriginal, PathSalida);
	return 0;
}
