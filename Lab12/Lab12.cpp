#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// Función para eliminar espacios al inicio y final de una cadena
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

void cargarConocimiento(map<string, string>& conocimiento, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo de conocimiento." << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        size_t separador = linea.find("|");
        if (separador != string::npos) {
            string pregunta = trim(linea.substr(0, separador));
            string respuesta = trim(linea.substr(separador + 1));
            conocimiento[pregunta] = respuesta;
        }
    }

    archivo.close();
}

string buscarExacto(const map<string, string>& conocimiento, const string& pregunta) {
    string preguntaLower = pregunta;
    transform(preguntaLower.begin(), preguntaLower.end(), preguntaLower.begin(), ::tolower);

    for (const auto& par : conocimiento) {
        string claveLower = par.first;
        transform(claveLower.begin(), claveLower.end(), claveLower.begin(), ::tolower);

        if (preguntaLower == claveLower) {
            return par.second;
        }
    }
    return "";
}

vector<string> dividirPalabras(const string& frase) {
    vector<string> palabras;
    stringstream ss(frase);
    string palabra;
    while (ss >> palabra) {
        transform(palabra.begin(), palabra.end(), palabra.begin(), ::tolower);
        palabras.push_back(palabra);
    }
    return palabras;
}

string buscarporPalabrasClave(const map<string, string>& conocimiento, const string& pregunta) {
    vector<string> palabrasPregunta = dividirPalabras(pregunta);
    string mejorRespuesta = "";
    int maxCoincidencias = 0;

    for (const auto& par : conocimiento) {
        vector<string> palabrasBase = dividirPalabras(par.first);

        int coincidencias = 0;
        for (const string& palabraPregunta : palabrasPregunta) {
            for (const string& palabraBase : palabrasBase) {
                if (palabraPregunta == palabraBase) {
                    coincidencias++;
                }
            }
        }

        if (coincidencias > maxCoincidencias) {
            maxCoincidencias = coincidencias;
            mejorRespuesta = par.second;
        }
    }
    if (maxCoincidencias > 0) {
        return mejorRespuesta;
    }
    else {
        return "";
    }
}

int main() {
    map<string, string> conocimiento;

    string rutaArchivo = "C:/Users/alumno/Desktop/conocimientos.txt";

    cargarConocimiento(conocimiento, rutaArchivo);

    cout << "Bienvenido a mi ChatBot" << endl;
    cout << "Escribe 'adios' para terminar el chat." << endl;

    string preguntaUsuario;
    while (true) {
        cout << "\nTu: ";
        getline(cin, preguntaUsuario);

        if (preguntaUsuario == "adios") {
            cout << "Bot: Hasta luego" << endl;
            break;
        }

        string respuesta = buscarExacto(conocimiento, preguntaUsuario);

        if (respuesta.empty()) {
            respuesta = buscarporPalabrasClave(conocimiento, preguntaUsuario);
        }

        if (respuesta.empty()) {
            respuesta = "Lo siento, no conozco la respuesta a esa pregunta.";
        }

        cout << "Bot: " << respuesta << endl;
    }

    return 0;
}
