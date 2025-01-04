#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>

using namespace std;

// Estructuras
struct Pelicula {
    string titulo;
    string genero;
    int duracion;
    int asientosDisponibles;
    Pelicula* siguiente;
};

struct Cliente {
    string nombre;
    int numBoletos;
    Cliente* siguiente;
};

struct NodoBoleto {
    Pelicula pelicula;
    Cliente cliente;
    NodoBoleto* siguiente;
};

// Funciones para la lista enlazada de películas
void agregarPelicula(Pelicula*& cabeza, Pelicula nuevaPelicula) {
    Pelicula* nuevoNodo = new Pelicula(nuevaPelicula);
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
}

void imprimirPeliculas(Pelicula* cabeza) {
    if (cabeza == nullptr) {
        cout << "No hay peliculas en el catalogo." << endl;
        return;
    }
    cout << "\n--- Catalogo de Peliculas ---\n";
    Pelicula* actual = cabeza;
    int i = 1;
    while (actual != nullptr) {
        cout << "**********************************************************\n";
        cout << "* Pelicula " << i <<"                                    * \n";
        cout << "**********************************************************\n";
        cout << "* Titulo: " << setw(47) << left << actual->titulo <<   "*\n"; // setw para alinear
        cout << "* Genero: " << setw(47) << left << actual->genero <<   "*\n";
        cout << "* Duracion: " << setw(36) << left << actual->duracion << " minutos *\n";
        cout << "* Asientos disponibles: " << setw(33) << left << actual->asientosDisponibles << "*\n";
        cout << "**********************************************************\n";
        actual = actual->siguiente;
        i++;
    }
}

Pelicula* buscarPelicula(Pelicula* cabeza) {
    if (cabeza == nullptr) {
        cout << "\033[1;33mNo hay peliculas en el catalogo.\033[1;36m\n";
        return nullptr;
    }

    string tituloBuscar;
    cout << "Ingrese el titulo de la pelicula que desea buscar: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer
    getline(cin, tituloBuscar);

    Pelicula* actual = cabeza;
    while (actual != nullptr) {
        if (actual->titulo == tituloBuscar) {
            return actual; // Se encontro la pelicula
        }
        actual = actual->siguiente;
    }

    cout << "\033[1;33mNo se encontro una pelicula con ese titulo.\033[1;36m\n";
    return nullptr; // No se encontro la pelicula
}

void eliminarPelicula(Pelicula*& cabeza) {
    if (cabeza == nullptr) {
        cout << "\033[1;33mNo hay peliculas en el catalogo.\033[1;36m\n";
        return;
    }

    string tituloEliminar;
    cout << "Ingrese el titulo de la pelicula que desea eliminar: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    getline(cin, tituloEliminar);

    Pelicula* actual = cabeza;
    Pelicula* anterior = nullptr;

    while (actual != nullptr && actual->titulo != tituloEliminar) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == nullptr) {
        cout << "\033[1;33mNo se encontro una pelicula con ese titulo.\033[1;36m\n";
        return;
    }

    if (anterior == nullptr) {
        // El nodo a eliminar es la cabeza de la lista
        cabeza = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }

    delete actual; // Liberar la memoria del nodo eliminado
    cout << "\033[1;32mPelicula eliminada exitosamente.\033[1;36m\n";
}

// Funciones para la cola de clientes
void encolarCliente(Cliente*& frente, Cliente*& final, Cliente nuevoCliente) {
    Cliente* nuevoNodo = new Cliente(nuevoCliente);
    nuevoNodo->siguiente = nullptr;
    if (final == nullptr) {
        frente = final = nuevoNodo;
    } else {
        final->siguiente = nuevoNodo;
        final = nuevoNodo;
    }
}

Cliente desencolarCliente(Cliente*& frente, Cliente*& final) {
    if (frente == nullptr) {
        return {"", 0, nullptr};    // Retorna un cliente vacío si la cola está vacía
    }
    Cliente cliente = *frente;
    Cliente* temp = frente;
    frente = frente->siguiente;
    if (frente == nullptr) {
        final = nullptr;
    }
    delete temp;
    return cliente;
}

// Funciones para la pila de boletos
void apilarBoleto(NodoBoleto*& cima, Pelicula pelicula, Cliente cliente) {
    NodoBoleto* nuevoNodo = new NodoBoleto({pelicula, cliente, cima});
    cima = nuevoNodo;
}

void imprimirPila(NodoBoleto* cima){
    if (cima == nullptr) {
        cout << "No se han vendido boletos." << endl;
        return;
    }
    NodoBoleto* temp = cima;
    cout << "\n--- Boletos Vendidos ---" << endl;
    while(temp != nullptr){
        cout << temp->cliente.nombre << " compro boletos para: " << temp->pelicula.titulo << endl;
        temp = temp->siguiente;
    }
}

void gestionarPeliculas(Pelicula*& catalogo) {
    int opcion;
    do {
        cout << "\n--- Gestion de Peliculas ---" << endl;
        cout << "1. Agregar Pelicula" << endl;
        cout << "2. Mostrar Catalogo" << endl;
        cout << "3. Eliminar Pelicula" << endl;
        cout << "4. Buscar pelicula" << endl;
        cout << "5. Volver al Menu Principal" << endl;
        cout << "Ingrese una opcion: ";

        while (!(cin >> opcion)) {
            cout << "Entrada invalida. Por favor, ingresa un número válido: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        while (opcion < 1 || opcion > 5)
        {
            cout << "Entrada invalida. Por favor, ingresa un número válido entre 1 y 5: ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> opcion;
        }

        switch (opcion) {
            case 1: {
                Pelicula nuevaPelicula;
                cout << "Ingrese el titulo de la pelicula: ";
                cin.ignore(); // Limpiar el buffer de entrada antes de getline
                getline(cin, nuevaPelicula.titulo);
                cout << "Ingrese el genero de la pelicula: ";
                getline(cin, nuevaPelicula.genero);
                cout << "Ingrese la duracion de la pelicula (en minutos): ";
                while (!(cin >> nuevaPelicula.duracion)) {
                    cout << "Entrada invalida. Por favor, ingresa un número válido: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                while (nuevaPelicula.duracion < 0)
                {
                    cout << "Entrada invalida. Por favor, ingresa un número válido mayor o igual a 0: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cin >> nuevaPelicula.duracion;
                }
                cout << "Ingrese la cantidad de asientos disponibles: ";
                while (!(cin >> nuevaPelicula.asientosDisponibles)) {
                    cout << "Entrada invalida. Por favor, ingresa un número válido: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                while (nuevaPelicula.asientosDisponibles < 0)
                {
                    cout << "Entrada invalida. Por favor, ingresa un número válido mayor o igual a 0: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cin >> nuevaPelicula.asientosDisponibles;
                }
                nuevaPelicula.siguiente = nullptr;
                agregarPelicula(catalogo, nuevaPelicula);
                cout << "Pelicula agregada exitosamente." << endl;
                break;
            }
            case 2:
                imprimirPeliculas(catalogo);
                break;
            case 3:
                eliminarPelicula(catalogo);
                break;
            case 4:{
                Pelicula* peliculaEncontrada = buscarPelicula(catalogo);
                if (peliculaEncontrada != nullptr) {
                    cout << "\033[1;32mPelicula encontrada:\033[1;36m\n";
                    cout << "**********************************************************\n";
                    cout << "* Titulo: " << setw(47) << left << peliculaEncontrada->titulo << "*\n";
                    cout << "* Genero: " << setw(47) << left << peliculaEncontrada->genero << "*\n";
                    cout << "* Duracion: " << setw(36) << left << peliculaEncontrada->duracion << " minutos *\n";
                    cout << "* Asientos disponibles: " << setw(33) << left << peliculaEncontrada->asientosDisponibles << "*\n";
                    cout << "**********************************************************\n";
                	}break;
            	}
            case 5:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
    }while (opcion != 5);
}

void mostrarPantallaBienvenida() {
    cout << "\033[2J\033[1;1H"; // Limpiar la terminal
    cout << "\033[1;36m"; // Cian brillante

    cout << "**********************************************************\n";
    cout << "*                                                        *\n";
    cout << "*     UNIVERSIDAD AUTONOMA BENITO JUAREZ DE OAXACA       *\n";
    cout << "*                                                        *\n";
    cout << "*              LIC. EN COMPUTACION                       *\n";
    cout << "*                                                        *\n";
    cout << "*           PROYECTO GESTION DE CINE                     *\n";
    cout << "*                                                        *\n";
    cout << "**********************************************************\n";
    cout << "\n";
    cout << "                 Cargando...                         \n";

    this_thread::sleep_for(chrono::seconds(3));
    cout << "\033[2J\033[1;1H"; // Limpiar la terminal
}

int main() {
    mostrarPantallaBienvenida();

    // Limpiar la terminal después de la pausa
    cout << "\033[2J\033[1;1H";

    Pelicula* catalogoPeliculas = nullptr;
    Cliente* colaEsperaFrente = nullptr;
    Cliente* colaEsperaFinal = nullptr;
    NodoBoleto* boletosVendidos = nullptr;

    int opcionMenu;
    do {
        cout << "**********************************************************\n";
        cout << "*                     MENU PRINCIPAL                     *\n";
        cout << "**********************************************************\n";
        cout << "* 1. Gestionar Peliculas                                 *\n";
        cout << "* 2. Comprar Boletos                                     *\n";
        cout << "* 3. Mostrar Boletos Vendidos                            *\n";
        cout << "* 4. Salir                                               *\n";
        cout << "**********************************************************\n";
        cout << "Ingrese una opcion: ";

        while (!(cin >> opcionMenu)) {
            cout << "\033[1;31mEntrada invalida. Por favor, ingresa un número válido:\033[1;36m"; // Rojo para errores
            cin.clear();
            cin.ignore(1000, '\n');
        }
        while (opcionMenu < 1 || opcionMenu > 4)
        {
            cout << "\033[1;31mEntrada invalida. Por favor, ingresa un número válido entre 1 y 4:\033[1;36m ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> opcionMenu;
        }
        switch (opcionMenu) {
            case 1:
                gestionarPeliculas(catalogoPeliculas);
                break;
            case 2: {
                imprimirPeliculas(catalogoPeliculas);
                if(catalogoPeliculas != nullptr){
                    int numClientes;
                    cout << "Ingrese el numero de clientes: ";
                    while (!(cin >> numClientes)) {
                        cout << "Entrada invalida. Por favor, ingresa un número válido: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }
                    while (numClientes < 0)
                    {
                        cout << "Entrada invalida. Por favor, ingresa un número válido mayor o igual a 0: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cin >> numClientes;
                    }
                    for(int i = 0; i < numClientes; i++){
                        string nombreCliente;
                        cout << "Ingrese el nombre del cliente " << i + 1 << ": ";
                        cin.ignore();
                        getline(cin, nombreCliente);
                        encolarCliente(colaEsperaFrente, colaEsperaFinal, {nombreCliente, 0, nullptr});
                    }
                    while (colaEsperaFrente != nullptr) {
                        Cliente cliente = desencolarCliente(colaEsperaFrente, colaEsperaFinal);

                        bool boletosComprados = false;

                        while (!boletosComprados) {
                            int opcionPelicula;
                            cout << cliente.nombre << ", elige la pelicula que deseas (ingresa el numero): ";

                            while (!(cin >> opcionPelicula)) {
                                cout << "Entrada invalida. Por favor, ingresa un número válido: ";
                                cin.clear();
                                cin.ignore(1000, '\n');
                            }
                            while (opcionPelicula < 1)
                            {
                                cout << "Entrada invalida. Por favor, ingresa un número válido mayor a 0: ";
                                cin.clear();
                                cin.ignore(1000, '\n');
                                cin >> opcionPelicula;
                            }

                            Pelicula* peliculaActual = catalogoPeliculas;
                            for (int i = 1; i < opcionPelicula; i++) {
                                if (peliculaActual == nullptr || peliculaActual->siguiente == nullptr) {
                                    cout << "Opcion de pelicula invalida." << endl;
                                    peliculaActual = nullptr;
                                    break;
                                }
                                peliculaActual = peliculaActual->siguiente;
                            }

                            if (peliculaActual != nullptr) {
                                cout << "Cuantos boletos deseas comprar para " << peliculaActual->titulo << "?: ";
                                while (!(cin >> cliente.numBoletos)) {
                                    cout << "Entrada invalida. Por favor, ingresa un número válido: ";
                                    cin.clear();
                                    cin.ignore(1000, '\n');
                                }
                                while (cliente.numBoletos < 0)
                                {
                                    cout << "Entrada invalida. Por favor, ingresa un número válido mayor o igual a 0: ";
                                    cin.clear();
                                    cin.ignore(1000, '\n');
                                    cin >> cliente.numBoletos;
                                }
                                if (peliculaActual->asientosDisponibles >= cliente.numBoletos) {
                                    peliculaActual->asientosDisponibles -= cliente.numBoletos;
                                    apilarBoleto(boletosVendidos, *peliculaActual, cliente);
                                    cout << cliente.nombre << " ha comprado " << cliente.numBoletos << " boletos para " << peliculaActual->titulo << endl;
                                    boletosComprados = true;
                                } else {
                                    cout << "Lo sentimos, no hay suficientes boletos disponibles para " << peliculaActual->titulo << ". Por favor, elige otra pelicula." << endl;
                                }
                            }
                        }
                    }
                }
                else{
                    cout << "No hay peliculas disponibles para comprar boletos. Por favor, agregue peliculas al catalogo." << endl;
                }
                break;
            }
            case 3:
                imprimirPila(boletosVendidos);
                break;
            case 4:
                cout << "\033[0m"; // Restablecer color antes de salir
                cout << "Saliendo del programa...\n";
                this_thread::sleep_for(chrono::seconds(1));
                break;
            default:
                cout << "\033[1;31mOpcion invalida.\033[1;36m\n";
        }
    } while (opcionMenu != 4);

    // Liberar memoria
    while (catalogoPeliculas != nullptr) {
        Pelicula* temp = catalogoPeliculas;
        catalogoPeliculas = catalogoPeliculas->siguiente;
        delete temp;
    }

    while (boletosVendidos != nullptr) {
        NodoBoleto* temp = boletosVendidos;
        boletosVendidos = boletosVendidos->siguiente;
        delete temp;
    }
}