//  ---                  JSON validator                      ---
//  ---  Este programa recibe un archivo .json de entrada.   ---
//  ---  Verifica que esté bien escrito el formato JSON.     ---
//  ---  En caso de un error, imprime en cosola.             ---
//  ---  Si es correcto crea dos archivos de salida.         ---
//  ------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <string>

using namespace std;

const string NOMBRE_ARCHIVO_ENTRADA = "entrada.json";

enum ModoAnalisis
{
    JSON,
    LLAVE,
    VALOR,
    LISTA
};

class ExpresionJSON
{
private:
    stack<char> pila;
    string buffers;

public:
    void evaluarCaracter(char c, char nc, ModoAnalisis &modo, string buffer);
    void imprimir(ofstream &archivoSalida) const;
};

class ExpresionValor
{
private:
    stack<char> pila;
    string buffers;

public:
    void evaluarCaracter(char c, char nc, ModoAnalisis &modo, string buffer);
    void imprimir(ofstream &archivoSalida) const;
};

class ExpresionLlave
{
private:
    stack<char> pila;
    string buffers;

public:
    void evaluarCaracter(char c, char nc, ModoAnalisis &modo, string buffer);
    void imprimir(ofstream &archivoSalida) const;
};

class ExpresionLista
{
private:
    stack<char> pila;
    string buffers;

public:
    void evaluarCaracter(char c, char nc, ModoAnalisis &modo, string buffer);
    void imprimir(ofstream &archivoSalida) const;
};

void procesarArchivo(const string &nombreArchivo, const string &nombreArchivoSalida, const string &nombreBuffers, ExpresionJSON &expresionJSON, ExpresionValor &expresionValor, ExpresionLlave &expresionLlave, ExpresionLista &expresionLista);

int main()
{
    string archivoEntrada = NOMBRE_ARCHIVO_ENTRADA;
    string archivoSalida = "salida.json";
    string buffers = "buffers.txt";

    ExpresionJSON expresionJSON;
    ExpresionValor expresionValor;
    ExpresionLlave expresionLlave;
    ExpresionLista expresionLista;

    procesarArchivo(archivoEntrada, archivoSalida, buffers, expresionJSON, expresionValor, expresionLlave, expresionLista);

    return 0;
}

void procesarArchivo(const string &nombreArchivo, const string &nombreArchivoSalida, const string &nombreBuffers, ExpresionJSON &expresionJSON, ExpresionValor &expresionValor, ExpresionLlave &expresionLlave, ExpresionLista &expresionLista)
{
    ifstream archivo(nombreArchivo);
    ofstream archivoSalida(nombreArchivoSalida);
    ofstream buffers(nombreBuffers);

    if (!archivo)
    {
        cout << "No se pudo abrir el archivo de entrada: " << nombreArchivo << endl;
        return;
    }

    cout << "Se pudo abrir el archivo de entrada: " << nombreArchivo << endl
         << endl;

    char c;
    char nc;
    string buffer; // Buffer global para almacenar los caracteres
    ModoAnalisis modo = JSON;

    while (archivo.get(c))
    {
        if (!isspace(c)) // omitir espacios
        {
            buffer += c;
            while (isspace(archivo.peek()))
            {
                archivo.get(nc);
            }
            nc = archivo.peek(); // obtener el siguiente caracter sin ser un espacio
            switch (modo)
            {
            case JSON:
                expresionJSON.evaluarCaracter(c, nc, modo, buffer);
                break;
            case VALOR:
                expresionValor.evaluarCaracter(c, nc, modo, buffer);
                break;
            case LISTA:
                expresionLista.evaluarCaracter(c, nc, modo, buffer);
                break;
            case LLAVE:
                expresionLlave.evaluarCaracter(c, nc, modo, buffer);
                break;
            }
        }
    }

    expresionJSON.imprimir(buffers);
    expresionValor.imprimir(buffers);
    expresionLista.imprimir(buffers);
    expresionLlave.imprimir(buffers);

    archivoSalida << buffer; // Imprimir el buffer en el archivo de salida
    archivoSalida.close();
    archivo.close();
    cout << "Archivo JSON verificado con exito!" << endl;
}

void ExpresionJSON::evaluarCaracter(char c, char nc, ModoAnalisis &modo, string buffer)
{
    if (pila.empty())
    {
        if (c == '{')
        {
            pila.push(c);
            buffers += c;
            modo = LLAVE;
        }
        else
        {
            cout << "Error JSON: Carácter inesperado antes de la apertura de llaves. El caracter erroneo fue: " << c << endl;
            cout << buffer << endl;
            exit(1);
        }
    }
    else
    {
        if (c == ',')
        {
            buffers += c;
            if (nc == '"')
            {
                modo = LLAVE;
            }
        }
        else if (c == '{') // varias anidaciones
        {
            pila.push(c);
            buffers += c;
            modo = LLAVE;
        }
        else if (c == '}')
        {
            pila.pop();
            buffers += c;
        }
        else
        {
            cout << "Error JSON: Carácter inesperado, se espera una coma o una llave. El caracter erroneo fue: " << c << endl;
            cout << buffer << endl;
            exit(1);
        }
    }
}

void ExpresionLlave::evaluarCaracter(char c, char nc, ModoAnalisis &modo, string buffer)
{
    if (pila.empty())
    {
        if (c == '"')
        {
            pila.push(c); // inicio de llave
            buffers += c;
        }
        else
        {
            cout << "Error LLAVE: Se esperaba comilla doble para iniciar la llave. El caracter erroneo fue: " << c << endl;
            cout << buffer << endl;
            exit(1);
        }
    }
    else
    {
        if (c == '"')
        {
            buffers += c;
            if (nc == ':')
            {
                pila.pop(); // cierre de llave
                pila.push(nc);
            }
            else
            {
                cout << "Error LLAVE: Se esperaba dos puntos al terminar la llave. El caracter erroneo fue: " << nc << endl;
                cout << buffer << endl;
                exit(1);
            }
        }
        else if (pila.top() == ':') // acá c == ":"
        {
            buffers += c;
            pila.pop();
            switch (nc)
            {
            case '"':
                modo = VALOR;
                break;
            case '{':
                modo = JSON;
                break;
            case '[':
                modo = LISTA;
                break;
            default:
                cout << "Error LLAVE: Se esperaba comilla, llave o corchete. El caracter erroneo fue: " << nc << endl;
                cout << buffer << endl;
                exit(1);
                break;
            }
        }
        else
        {
            buffers += c;
        }
    }
}

void ExpresionValor::evaluarCaracter(char c, char nc, ModoAnalisis &modo, string buffer)
{
    if (pila.empty())
    {
        if (c == '"')
        {
            pila.push(c);
            buffers += c;
        }
        else
        {
            cout << "Error VALOR: Se esperaba comilla doble para iniciar el valor. El caracter erroneo fue: " << c << endl;
            cout << buffer << endl;
            exit(1);
        }
    }
    else
    {
        if (c == '"')
        {
            pila.pop(); // cierre valor
            buffers += c;
            switch (nc)
            {
            case ',':
                modo = JSON;
                break;
            case '}':
                modo = JSON;
                break;
            default:
                cout << "Error VALOR: Se esperaba comilla, llave o corchete. El caracter erroneo fue: " << nc << endl;
                cout << buffer << endl;
                exit(1);
                break;
            }
        }
        else
        {
            buffers += c;
        }
    }
}

void ExpresionLista::evaluarCaracter(char c, char nc, ModoAnalisis &modo, string buffer)
{
    if (pila.empty())
    {
        if (c == '[')
        {
            pila.push(c);
            buffers += c;
        }
        else
        {
            cout << "Error LISTA: Se esperaba corchete para iniciar la lista. El caracter erroneo fue: " << c << endl;
            cout << buffer << endl;
            exit(1);
        }
    }
    else
    {
        if (pila.top() == '[')
        {
            if (c == '"') // empieza el string
            {
                pila.pop();
                pila.push(c); // ahora pila.top() == '"'
                buffers += c;
            }
            else if (c == ',') // para nuevo string
            {
                buffers += c;
            }
            else
            {
                cout << "Error LISTA: Se esperaba comillas para iniciar el string. El caracter erroneo fue: " << c << endl;
                cout << buffer << endl;
                exit(1);
            }
        }
        else if (pila.top() == '"') // termina el string
        {
            buffers += c;
            if (c == '"')
            {
                pila.pop(); // pila vacia
                switch (nc)
                {
                case ',':
                    pila.push('['); // iniciar otro string
                    break;
                case ']':
                    pila.push(']'); // cierro lista
                    break;
                default:
                    cout << "Error LISTA: Se esperaba coma para otro string o corchete para cerrar lista. El caracter erroneo fue: " << c << endl;
                    cout << buffer << endl;
                    exit(1);
                    break;
                }
            }
        }
        else if (pila.top() == ']')
        {
            buffers += c;
            pila.pop();
            modo = JSON;
        }
        else
        {
            cout << "Error LISTA: Se esperaba comilla doble. El caracter erroneo fue: " << c << endl;
            cout << buffer << endl;
            exit(1);
        }
    }
}

void ExpresionJSON::imprimir(ofstream &archivoSalida) const
{
    archivoSalida << "ExpresionJSON:" << endl;
    for (char c : buffers)
    {
        archivoSalida << c;
    }
    archivoSalida << endl
                  << endl;
}

void ExpresionValor::imprimir(ofstream &archivoSalida) const
{
    archivoSalida << "ExpresionValor:" << endl;
    for (char c : buffers)
    {
        archivoSalida << c;
    }
    archivoSalida << endl
                  << endl;
}

void ExpresionLlave::imprimir(ofstream &archivoSalida) const
{
    archivoSalida << "ExpresionLlave:" << endl;
    for (char c : buffers)
    {
        archivoSalida << c;
    }
    archivoSalida << endl
                  << endl;
}

void ExpresionLista::imprimir(ofstream &archivoSalida) const
{
    archivoSalida << "ExpresionLista:" << endl;
    for (char c : buffers)
    {
        archivoSalida << c;
    }
    archivoSalida << endl
                  << endl;
}
