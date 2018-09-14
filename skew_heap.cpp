/**
  @file skew_heap.c

  Skew Heap implementation.

  Métodos Algorítmicos en Resolución de Problemas
  Facultad de Informática
  Universidad Complutense de Madrid

  Author: David Antuña Rodríguez, 2016-2017
*/

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <iomanip>

using namespace std;

class Heap {
	public:
		/*
		 * Constructor.
		 * @param value    Entero que representa la raiz del monticulo si no se proporciona se fija a NULL (no existe la raiz).
		 * @param left     Puntero al monticulo izquierdo si no se proporciona se fija a NULL (no existe hijo izquierdo).
		 * @param right    Puntero al monticulo derecho si no se proporciona se fija a NULL (no existe hijo derecho).
		 * @return		   Devuelve el monticulo sesgado resultado de dichos parametros.
		 */
		Heap(int* value = NULL, Heap* left = NULL, Heap* right = NULL)
		{
			this->root = value;
			this->left = left;
			this->right = right;
		}

		/*
		 * Permite conocer el minimo de un monticulo sesgado.
		 * @return    Minimo absoluto del monticulo.
		 *
		 * Procedimiento:
		 *		- Devuelve la raiz del monticulo.
		 */
		int min() {
			return *this->root;
		}
		
		/*
		 * Permite insertar un valor en un monticulo sesgado.
		 * @param value    Valor que se insertara en el monticulo.
		 * No se devuelve un nuevo monticulo pues al insertar se modifica el monticulo en el que se ha realizado la operacion.
		 *
		 * Procedimiento:
		 *		- Se une el monticulo sobre el que se inserta con uno nuevo que se crea con el valor recibido como raiz, se almacena el resultado.
		 *		- Modificar los atributos del monticulo que ha invocado la operacion para que concuerden con el resultado almacenado.
		 */
		void insert(int value)
		{
			Heap* temp = merge(new Heap(new int(value)));
			swap_pointers(temp);
		}
		
		/*
		 * Elimina el minimo de un monticulo.
		 * No tiene return:
		 *			- Si se quiere conocer dicho minimo habra que llamar antes a la funcion min.
		 *			- El monticulo sobre el que se ha realizado la operacion se modifica y por tanto no hay que devolver otro monticulo.
		 *
		 * Procedimiento:
		 *		- Union del hijo izquierdo y derecho, se almacena el resultado de forma temporal.
		 *		- Modificar los atributos del monticulo que ha invocado la operacion para que concuerden con el resultado almacenado.
		 */
		void delete_min()
		{
			Heap* temp = this->left->merge(this->right);
			swap_pointers(temp);
		}
		
		/*
		 * Modifica el valor de una clave.
		 * @param new_value		Nuevo valor que se le dara a dicha clave.
		 * @param h				Puntero a la clave que se quiere modificar.
		 * No tiene return, al modificar la clave se alterara el monticulo para reflejar dicho cambio.
		 *
		 * Dado que en la representacion todo son punteros a monticulos, el puntero recibido ha de ser un puntero a un monticulo.
		 * Sien embargo dicho puntero forma parte de otro monticulo en el que se va ha modificar la clave, es decir, si queremos
		 * modificar la clave que existe como hija izquierda de la raiz pasaremos el puntero this->left, y ese puntero apunta tambien
		 * a un monticulo.
		 *
		 * Procedimiento:
		 *		- Eliminar el minimo del monticulo al que apunta el puntero recibido.
		 *		- Insertar el nuevo valor en el monticulo del que forma parte dicho puntero, el monticulo que llama a la operacion.
		 */
		void modify_key(int new_value, Heap* h) {
			h->delete_min();
			this->insert(new_value);
		}

		/*
		 * Sobrecarga del operador << para poder mostrar los monticulo mediante "cout << monticulo".
		 * @param out    Flujo de salida.
		 * @param h		 Monticulo que se mostrara.
		 * @return		 Flujo de salida con los datos del monticulo en el.
		 */
		friend ostream& operator<< (ostream& out, const Heap& h)
		{
			if (!h.root)
				out << "Monticulo vacio" << endl;
			else {
				out << "RAIZ(" << *h.root << ")";
				if (h.left && h.left->root) out << " IZQ(" << *h.root << ") [" << *h.left << "]";
				if (h.right  && h.right->root) out << " DER(" << *h.root << ") [" << *h.right << "]";
			}
			return out;
		}


		/****************************************************************************************************************
		 * Las funciones get_left() y get_right() no son necesarias para el funcionamiento del monticulo.				*
		 * Sin embargo, si se quiere usar la funcion modify_key para alterar una clave del monticulo se necesita un		*
		 * puntero a dicha clave, de modo que se pueden usar estas funciones combinadas para encontrar ese puntero.		*
		 ****************************************************************************************************************/

		/*
		 * Devuelve el puntero al hijo izquierdo de un monticulo.
		 * @return		Puntero al monticulo izquierdo.
		 */
		Heap* get_left() {
			return this->left;
		}

		/*
		* Devuelve el puntero al hijo derecho de un monticulo.
		* @return		Puntero al monticulo derecho.
		*/
		Heap* get_right() {
			return this->left;
		}
		
	private:
		/*
		* Realiza la union de dos monticulos, el que ha llamado a la operacion y uno que se le proporciona.
		* @param other    Monticulo que se quiere unir al monticulo que invoca la operacion.
		* @return		   Nuevo monticulo resultado de la union.
		*
		* Procedimiento:
		*		- Si uno de los dos monticulos que se van a unir es NULL se devuelve el otro monticulo.
		*		  Este caso es necesario para que modificar clave no falle en dos situaciones:
		*				+ Modificar clave sobre un monticulo de un solo nodo.
		*				+ Modificar clave sobre una clave que no tiene hijo izquierdo o derecho.
		*		- Compara las raices de ambos monticulos.
		*		- Toma la menor (nuevo minimo) y la asigna como raiz del monticulo union.
		*		- El hijo derecho del monticulo union es el hijo izquierdo de laque se ha marcado como menor.
		*		- El hijo izquierdo resulta de la union entre el monticulo que no contenia la menor raiz y el hijo derecho del otro monticulo.
		*		  Hay dos situaciones posibles:
		*				+ El hijo derecho del monticulo que contenia el menor no es NULL, en cuyo caso se procedera con la union.
		*				+ El hijo derecho del monticulo que contenia el menor es NULL, en cuyo caso se asignara como hijo izquierdo al monticulo
		*					que no contenia el menor.
		*/
		Heap* merge(Heap* other)
		{
			if (this == NULL)
				return other;
			else if (other == NULL)
				return this;

			if (*this->root > *other->root)
				return new Heap(other->root, (other->right != NULL ? this->merge(other->right) : new Heap(this->root, this->left, this->right)), other->left);
			else
				return new Heap(this->root, (this->right != NULL ? other->merge(this->right) : other), this->left);
		}

		/*
		 * Modifica los punteros de un monticulo para igualarlos a los del monticlo que recibe por parametro.
		 * @param other		Monticulo cuyos punteros queremos copiar.
		 * No tiene return, se modifica el monticulo que llama a la operacion.
		 *
		 * Procedimiento:
		 *		- Si el monticulo recibido es NULL se asigna todo a NULL.
		 *		- Si no es NULL se copian los punteros a la raiz, hijo izquierdo e hijo derecho.
		 */
		void swap_pointers(Heap* other) {
			if (!other) {
				this->root = NULL;
				this->left = NULL;
				this->right = NULL;
				return;
			}
			this->root = other->root;
			this->left = other->left;
			this->right = other->right;
		}

		// Valor de la raiz del monticulo.
		int* root;
		// Puntero al hijo izquierdo (que tambien es un monticulo).
		Heap* left;
		// Puntero al hijo derecho (que tambien es un monticulo).
		Heap* right;
};

int main ()
{	
	Heap* h;
	string op;

	// La primera operacion de un caso de prueba siempre tiene que ser crear, si no lo es se acaba la ejecucion.
	cin >> op;
	if (op == "CREAR") {
		int temp;
		cin >> temp;

		h = new Heap(new int(temp));

		cout << "Monticulo creado." << endl;
		cout << *h << endl << endl;
	}
	else {
		cout << "La primera instruccion del caso de prueba no es CREAR." << endl;
		system("Pause");
		return 0;
	}

	// Lee la siguiente operacion a realizar, el caso acaba cuando se lee FIN o falla la lectura
	while (cin >> op  && op != "FIN") {
		if (op == "INSERTAR") {
			// Lee el valor a insertar.
			int temp;
			cin >> temp;

			// Inserta el valor leido.
			h->insert(temp);

			cout << "Se ha insertado " << temp << " con exito." << endl;
		}
		else if (op == "BORRAR") {
			// Borra el minimo.
			h->delete_min();

			cout << "Se ha eliminado el minimo con exito." << endl;
		}
		else if (op == "MIN") {
			// Muestra el minimo.
			cout << "El minimo del monticulo es " << h->min() << "." << endl;
		}
		else if (op == "MODIFICAR") {
			Heap* temp_heap = h;

			// Lee el nuevo valor.
			int new_value;
			cin >> new_value;

			// Lee la posicion a modificar como un conjunto de movimientos en el monticulo (i o d) hasta que lee p.
			char dir;
			cin >> dir;
			while (dir != 'P') {
				if (dir == 'I') temp_heap = temp_heap->get_left(); // Si lee i se mueve a la izquierda.
				else if (dir == 'D') temp_heap = temp_heap->get_right(); // Si lee d se mueve a la derecha.
				cin >> dir;
			}

			cout << "Se va a modificar el valor " << temp_heap->min() << " por el valor " << new_value<< endl;
			// Modifica el valor.
			h->modify_key(new_value, temp_heap);
			cout << "Valor modificado." << endl;
		}
		else cout << "Operacion no reconocida." << endl;
		// Sea cual sea la operacion realizada muestra el estado del monticulo.
		cout << *h << endl << endl;
	}

	system("Pause");
	return 0;
}