#include <iostream>
#include <cstring>

using namespace std;

//* -------- ------- ------ ----- Node ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

template <class T>
class Node
{
private:
  T data;
  T *dataPtr;
  Node<T> *next;
  Node<T> *prev;

public:
  class Exception : public std::exception
  {
  private:
    std::string msg;

  public:
    explicit Exception(const char *message) : msg(message) {}

    explicit Exception(const std::string &message) : msg(message) {}

    virtual ~Exception() throw() {}

    virtual const char *what() const throw()
    {
      return msg.c_str();
    }
  };

  Node();
  Node(const T &);

  ~Node();

  T *&getDataPtr();
  T &getData() const;
  Node<T> *getNext() const;
  Node<T> *getPrev() const;

  void setDataPtr(T *);
  void setData(const T &);
  void setNext(Node *);
  void setPrev(Node *);
};

template <class T>
Node<T>::Node() : next(nullptr), prev(nullptr), dataPtr(nullptr) {}

template <class T>
Node<T>::Node(const T &e) : dataPtr(new T(e)), prev(nullptr), next(nullptr)
{
  if (dataPtr == nullptr)
  {
    throw Exception("Memoria insuficiente, creando nodo");
  }
}

template <class T>
T *&Node<T>::getDataPtr()
{
  return dataPtr;
}

template <class T>
Node<T>::~Node()
{
  delete dataPtr;
  prev = nullptr;
  next = nullptr;
}

template <class T>
T &Node<T>::getData() const
{
  if (dataPtr == nullptr)
  {
    throw Exception("Dato inexistente, getData");
  }
  return *dataPtr;
}

template <class T>
Node<T> *Node<T>::getNext() const
{
  return next;
}

template <class T>
Node<T> *Node<T>::getPrev() const
{
  return prev;
}

template <class T>
void Node<T>::setDataPtr(T *p)
{
  dataPtr = p;
}

template <class T>
void Node<T>::setData(const T &e)
{
  if (dataPtr == nullptr)
  {
    if ((dataPtr = new T(e)) == nullptr)
    {
      throw Exception("Memoria no disponible, setData");
    }
  }
  else
  {
    *dataPtr = e;
  }
}

template <class T>
void Node<T>::setNext(Node *p)
{
  next = p;
}

template <class T>
void Node<T>::setPrev(Node *p)
{
  prev = p;
}

//* -------- ------- ------ ----- List ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

// -------- ------- ------ ----- Definition ----- ------ ------- --------

template <class T>
class List
{
private:
  Node<T> *anchor;

  bool isValidPosition(Node<T> *);
  void copyAll(const List<T> &);
  void swapData(Node<T> *, Node<T> *);
  Node<T> *getHalf(Node<T> *, Node<T> *);
  static int compare(const T &a, const T &b);

public:
  typedef Node<T> *Position;

  class Exception : public std::exception
  {
  private:
    std::string msg;

  public:
    explicit Exception(const char *message) : msg(message) {}

    explicit Exception(const std::string &message) : msg(message) {}

    virtual ~Exception() throw() {}

    virtual const char *what() const throw()
    {
      return msg.c_str();
    }
  };

  List();
  List(const List<T> &);

  ~List();

  bool isEmpty() const;

  void insert(const T &, Node<T> *); //data, positon
  void remove(Node<T> *);

  Node<T> *getFirst();
  Node<T> *getLast();
  Node<T> *getPreviousPos(Node<T> *);
  Node<T> *getNextPos(Node<T> *);

  Node<T> *linearSearch(const T &, int (*comp)(const T &, const T &) = List::compare);
  Node<T> *binarySearch(const T &, int (*comp)(const T &, const T &) = List::compare);
  Node<T> *binarySearch(const T &, Node<T> *first, Node<T> *last, int (*comp)(const T &, const T &) = List::compare);

  T recover(Node<T> *);

  void bubbleSort();
  void bubbleSort(Node<T> *);

  std::string toString() const;

  void deleteAll();

  List<T> &operator=(const List<T> &);
};

// -------- ------- ------ ----- Implementation ----- ------ ------- --------
using namespace std;

template <class T>
bool List<T>::isValidPosition(Node<T> *position)
{
  Node<T> *aux{anchor};

  while (aux != nullptr)
  {
    if (aux == position)
    {
      return true;
    }

    aux = aux->getNext();
  }

  return false;
}

template <class T>
void List<T>::copyAll(const List<T> &newList)
{
  Node<T> *aux{newList.anchor};
  Node<T> *last{nullptr};
  Node<T> *newNode;

  while (aux != nullptr)
  {
    newNode = new Node<T>(aux->getData());

    if (newNode == nullptr)
    {
      throw Exception("Memoria no disponible, copyAll");
    }

    if (last == nullptr)
    {
      anchor = newNode;
    }
    else
    {
      newNode->setPrev(last);
      last->setNext(newNode);
    }
    last = newNode;
    aux = aux->getNext();
  }
}

template <class T>
void List<T>::swapData(Node<T> *a, Node<T> *b)
{
  T aux{a->getData()};
  a->setData(b->getData());
  b->setData(aux);
}

template <class T>
Node<T> *List<T>::getHalf(Node<T> *first, Node<T> *last)
{
  Node<T> *half{first};
  Node<T> *aux{first};

  while (aux != last)
  {
    aux = aux->getNext();
    if (aux != last)
    {
      half = half->getNext();
      aux = aux->getNext();
    }
  }

  if (aux != first)
  {
    aux = aux->getNext();
  }

  return half;
}

template <class T>
int List<T>::compare(const T &a, const T &b)
{
  if (a > b)
    return 1;
  else if (a == b)
    return 0;
  else
    return -1;
}

template <class T>
List<T>::List() : anchor(nullptr)
{
}

template <class T>
List<T>::List(const List<T> &newList) : anchor(nullptr)
{
  copyAll(newList);
}

template <class T>
List<T>::~List()
{
  deleteAll();
}

template <class T>
bool List<T>::isEmpty() const
{
  return anchor == nullptr;
}

template <class T>
void List<T>::insert(const T &value, Node<T> *position)
{
  if (anchor != nullptr and !isValidPosition(position))
  {
    throw Exception("Posicion invalida, insert");
  }

  Node<T> *aux;
  aux = new Node<T>(value);

  if (aux == nullptr)
  {
    throw Exception("Memoria no disponible, insert");
  }

  if (position == nullptr)
  { //insert at the beginning
    aux->setPrev(nullptr);
    if (anchor != nullptr)
    {
      anchor->setPrev(aux);
    }
    anchor = aux;
  }
  else
  { //insert in any other position
    aux->setPrev(position);
    aux->setNext(position->getNext());
    if (position->getNext() != nullptr)
    {
      position->getNext()->setPrev(aux);
    }
    position->setNext(aux);
  }
}

template <class T>
void List<T>::remove(Node<T> *position)
{
  if (!isValidPosition(position))
  {
    throw Exception("Posicion invalida, remove");
  }

  if (position->getPrev() != nullptr)
  {
    position->getPrev()->setNext(position->getNext());
  }

  if (position->getNext() != nullptr)
  {
    position->getNext()->setPrev(position->getPrev());
  }

  if (position == anchor)
  { //delete first
    anchor = anchor->getNext();
  }

  delete position;
}

template <class T>
Node<T> *List<T>::getFirst()
{
  return anchor;
}

template <class T>
Node<T> *List<T>::getLast()
{

  if (isEmpty())
    return nullptr;

  Node<T> *aux{anchor};

  while (aux->getNext() != nullptr)
  {
    aux = aux->getNext();
  }

  return aux;
}

template <class T>
Node<T> *List<T>::getPreviousPos(Node<T> *position)
{
  if (!isValidPosition(position))
    return nullptr;

  return position->getPrev();
}

template <class T>
Node<T> *List<T>::getNextPos(Node<T> *position)
{
  if (!isValidPosition(position))
    return nullptr;

  return position->getNext();
}

template <class T>
Node<T> *List<T>::linearSearch(const T &value, int (*comp)(const T &, const T &))
{
  Node<T> *aux{anchor};

  while (aux != nullptr)
  {
    if (comp(aux->getData(), value) == 0)
    {
      return aux;
    }
    aux = aux->getNext();
  }
  return nullptr;
}

template <class T>
Node<T> *List<T>::binarySearch(const T &value, int (*comp)(const T &, const T &))
{
  if (isEmpty())
    return nullptr;
  return binarySearch(value, anchor, getLast(), comp);
}

template <class T>
Node<T> *List<T>::binarySearch(const T &value, Node<T> *first, Node<T> *last, int (*comp)(const T &, const T &))
{

  Node<T> *half{getHalf(first, last)};

  if (comp(half->getData(), value) == 0)
    return half; //the element was found
  if (first == last)
    return nullptr; // Stop recursion
  if (comp(value, half->getData()) < 0)
  {
    last = half->getPrev();
  }
  else
  {
    first = half->getNext();
  }

  return binarySearch(value, first, last, comp);
}

template <class T>
T List<T>::recover(Node<T> *position)
{
  if (!isValidPosition(position))
  {
    throw Exception("Posicion invalida, recover");
  }

  return position->getData();
}

template <class T>
void List<T>::bubbleSort()
{
  if (isEmpty())
    return;
  bubbleSort(getLast());
}

template <class T>
void List<T>::bubbleSort(Node<T> *last)
{
  Node<T> *aux{anchor};
  bool flag{false};

  while (aux != last)
  {
    if (aux->getData() > aux->getNext()->getData())
    {
      swapData(aux, aux->getNext());
      flag = true;
    }
    aux = aux->getNext();
  }

  last = last->getPrev();

  if (aux == last or flag == false)
    return;

  bubbleSort(last);
}

template <class T>
string List<T>::toString() const
{
  if (isEmpty())
    return "";

  Node<T> *aux{anchor};

  string stringList{aux->getData().toString()}; //-------- ------- ------ -----  ----- ------ ------- --------

  while (aux->getNext() != nullptr)
  {
    aux = aux->getNext();
    stringList += '\n' + aux->getData().toString();
  }

  return stringList + '\n';
}

template <class T>
void List<T>::deleteAll()
{
  Node<T> *aux;

  while (anchor != nullptr)
  {
    aux = anchor;
    anchor = anchor->getNext();
    delete aux;
  }
}

template <class T>
List<T> &List<T>::operator=(const List<T> &newList)
{
  deleteAll();
  copyAll(newList);
  return *this;
}

//* -------- ------- ------ ----- Functions ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

//* -------- ------- ------ ----- Reacciones ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

class Reaction
{
private:
  int id;
  string name;
  string estequiometria;
  int lowerLimit;
  int higherLimit;

public:
  Reaction();
  Reaction(const Reaction &);

  int getId() const;
  string getName() const;
  string getEstequiometria() const;
  int getLowerLimit() const;
  int getHigherLimit() const;

  void setId(const int &);
  void setName(const string &);
  void setEstequiometria(const string &);
  void setLowerLimit(const int &);
  void setHigherLimit(const int &);

  string toString();

  bool operator == (const Reaction &) const;
  bool operator > (const Reaction &) const;
};

Reaction::Reaction() {}

Reaction::Reaction(const Reaction &r) : id(r.id), name(r.name), estequiometria(r.estequiometria), lowerLimit(r.lowerLimit), higherLimit(r.higherLimit) {}

int Reaction::getId() const
{
  return id;
}

string Reaction::getName() const
{
  return name;
}

string Reaction::getEstequiometria() const
{
  return estequiometria;
}

int Reaction::getLowerLimit() const
{
  return lowerLimit;
}

int Reaction::getHigherLimit() const
{
  return higherLimit;
}

void Reaction::setId(const int &e)
{
  id = e;
}

void Reaction::setName(const string &e)
{
  name = e;
}

void Reaction::setEstequiometria(const string &e)
{
  estequiometria = e;
}

void Reaction::setLowerLimit(const int &e)
{
  lowerLimit = e;
}

void Reaction::setHigherLimit(const int &e){
  higherLimit = e;
}

string Reaction::toString()
{
  string result{""};
  result += "Id: " + id;
  result += "Nombre: " + name;
  result += "Estequiometria: " + estequiometria;
  result += "Limite Inferior: " + to_string(lowerLimit);
  result += "Limite Superior: " + to_string(higherLimit);

  return result;
};

bool Reaction::operator == (const Reaction &e) const
{
  return name == e.name;
}

bool Reaction::operator > (const Reaction &e) const
{
  return name > e.name;
}

//* -------- ------- ------ ----- Metabolitos ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

class Metabolite
{
private:
  int id;
  string name;
  string chemicalForm;
  string compartment;

public:
  Metabolite();
  Metabolite(const Metabolite &);

  int getId() const;
  string getName() const;
  string getChemicalForm() const;
  string getCompartment() const;

  void setId(const int &);
  void setName(const string &);
  void setChemicalForm(const string &);
  void setCompartment(const string &);

  string toString();

  bool operator == (const Metabolite &) const;
  bool operator > (const Metabolite &) const;
};

Metabolite::Metabolite() {}

Metabolite::Metabolite(const Metabolite &m) : id(m.id), name(m.name), chemicalForm(m.chemicalForm), compartment(m.compartment) {}

int Metabolite::getId() const
{
  return id;
}

string Metabolite::getName() const
{
  return name;
}

string Metabolite::getChemicalForm() const
{
  return chemicalForm;
}

string Metabolite::getCompartment() const
{
  return compartment;
}

void Metabolite::setId(const int &e)
{
  id = e;
}

void Metabolite::setName(const string &e)
{
  name = e;
}

void Metabolite::setChemicalForm(const string &e)
{
  chemicalForm = e;
}

void Metabolite::setCompartment(const string &e)
{
  compartment = e;
}

string Metabolite::toString()
{
  string result{""};
  result += "Id: " + to_string(id);
  result += "Nombre: " + name;
  result += "Formula quimica: " + chemicalForm;
  result += "Compartimiento: " + compartment;

  return result;
}

bool Metabolite::operator == (const Metabolite &e) const
{
  return name == e.name;
}

bool Metabolite::operator > (const Metabolite &e) const
{
  return name > e.name;
}

//* -------- ------- ------ ----- Genes ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

class Gen
{
private:
  int id;
  string name;
  string functional;
  string reaction;

public:
  Gen();
  Gen(const Gen &);

  int getId() const;
  string getName() const;
  string getFunctional() const;
  string getReaction() const;

  void setId(const int &);
  void setName(const string &);
  void setFunctional(const string &);
  void setReaction(const string &);

  string toString();

  bool operator == (const Gen &) const;
  bool operator > (const Gen &) const;
};

Gen::Gen() {}

Gen::Gen(const Gen &g) : id(g.id), name(g.name), functional(g.functional), reaction(g.reaction) {}

int Gen::getId() const
{
  return id;
}

string Gen::getName() const
{
  return name;
}

string Gen::getFunctional() const
{
  return functional;
}

string Gen::getReaction() const
{
  return reaction;
}

void Gen::setId(const int &e)
{
  id = e;
}

void Gen::setName(const string &e)
{
  name = e;
}

void Gen::setFunctional(const string &e)
{
  functional = e;
}

void Gen::setReaction(const string &e)
{
  reaction = e;
}

string Gen::toString()
{
  string result{""};
  result += "Id: " + to_string(id);
  result += "Nombre: " + name;
  result += "Funcional: " + functional;
  result += "Reaccion: " + reaction;

  return result;
}

bool Gen::operator == (const Gen &e) const
{
  return name == e.name;
}

bool Gen::operator > (const Gen &e) const
{
  return name > e.name;
}

//* -------- ------- ------ ----- Modelo Metabolico ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

class Model
{
private:
  string name;
  Node<Model> *memoryDirection;
  int numberOfMetabolites;
  int numberOfReactions;
  string objetiveExpression;
  string compartments;

  List<Reaction> reactionList;
  List<Metabolite> metaboliteList;
  List<Gen> genList;

public:
  Model();
  Model(const Model &);
  ~Model();

  string getName() const;
  Node<Model> *getMemoryDirection() const;
  int getNumberOfMetabolites() const;
  int getNumberOfReactions() const;
  string getObjetiveExpression() const;
  string getCompartments() const;

  void setName(const string &);
  void setMemoryDirection(Node<Model> *);
  void setNumberOfMetabolites(const int &);
  void setNumberOfReactions(const int &);
  void setObjetiveExpression(const string &);
  void setCompartments(const string &);

  string toString();

  void chooseList() const;

  bool operator == (const Model &) const;
  bool operator > (const Model &) const;
};

Model::Model() : memoryDirection(nullptr) {}

Model::Model(const Model &m) : name(m.name), memoryDirection(m.memoryDirection), numberOfMetabolites(m.numberOfMetabolites), numberOfReactions(m.numberOfReactions), objetiveExpression(m.objetiveExpression), compartments(m.compartments) {}

Model::~Model()
{
  delete memoryDirection;
  memoryDirection = nullptr;
}

string Model::getName() const
{
  return name;
}

Node<Model> *Model::getMemoryDirection() const
{
  return memoryDirection;
}

int Model::getNumberOfMetabolites() const
{
  return numberOfMetabolites;
}

int Model::getNumberOfReactions() const
{
  return numberOfReactions;
}

string Model::getObjetiveExpression() const
{
  return objetiveExpression;
}

string Model::getCompartments() const
{
  return compartments;
}

void Model::setName(const string &e) // e -> element
{
  name = e;
}

void Model::setMemoryDirection(Node<Model> *e)
{
  memoryDirection = e;
}

void Model::setNumberOfMetabolites(const int &e)
{
  numberOfMetabolites = e;
}

void Model::setNumberOfReactions(const int &e)
{
  numberOfReactions = e;
}

void Model::setObjetiveExpression(const string &e)
{
  objetiveExpression = e;
}

void Model::setCompartments(const string &e)
{
  compartments = e;
}

string Model::toString()
{
  string result{""};

  result += "\nNombre: " + name;
  // result += "\nDireccion de memoria: " + to_string(memoryDirection);
  result += "\nNumero de Metabolitos: " + to_string(numberOfMetabolites);
  result += "\nNumero de Reacciones: " + to_string(numberOfReactions);
  result += "\nExpresion Objetivo: " + objetiveExpression;
  result += "\nCompartimentos: " + compartments;

  return result;
}

void Model::chooseList() const
{
  int option{0};
  cout << "\n1.Reacciones\n";
  cout << "\n2.Metabolitos\n";
  cout << "\n3.Genes\n";
  cin >> option;

  if (option == 1) {
    // reactionList
  } else if (option == 2) {
    // metaboliteList
  } else {
    //genList
  }
}

bool Model::operator == (const Model &e) const
{
  return name == e.name;
}

bool Model::operator > (const Model &e) const
{
  return name > e.name;
}

//* -------- ------- ------ ----- Interface ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

// -------- ------- ------ ----- Definition ----- ------ ------- --------
class Interface
{
private:
  int option;
  Model modelAux;

  Node<Model> *search(List<Model> &);
  void userInterface(List<Model> &);
  int optionList();
  void addModel(List<Model> &);

public:
  Interface(List<Model> &);
};

// -------- ------- ------ ----- Implementation ----- ------ ------- --------

Interface::Interface(List<Model> &modelList)
{
  userInterface(modelList);
}

int Interface::optionList()
{
  int option{0};
  cout << "\n1.Agregar\n";
  cout << "2.Acceder\n";
  cout << "3.Buscar\n";
  cout << "4.Editar\n";
  cout << "5.Eliminar\n";
  cout << "6.Ordenar\n";
  cout << "8.Salir\n";
  cin >> option;
  return option;
}

void Interface::addModel(List<Model> &modelList)
{
  string stringAux{""};

  cin.ignore();
  cout << "\nNombre: ";
  getline(cin, stringAux);
  modelAux.setName(stringAux);

  cout << "\nAgregar Metabolito: ";
  cout << "\nAgregar Reaccion: ";

  cout << "\nExpresion Objetivo: ";
  getline(cin, stringAux);
  modelAux.setObjetiveExpression(stringAux);

  cout << "\nCompartimientos: ";
  getline(cin, stringAux);
  modelAux.setCompartments(stringAux);

  modelList.insert(modelAux, modelList.getLast()); //insercion despues del punto de interes

  // cout << modelList.getLast();

  // modelList.getLast()->getDataPtr()->setMemoryDirection(modelList.getFirst());
}

Node<Model> * Interface::search(List<Model> &modelList) {
  string stringAux{""};
  Node<Model> *auxNodeModel{nullptr};
  cout << "Nombre del Modelo: ";
  cin.ignore();
  getline(cin, stringAux);
  modelAux.setName(stringAux);
  auxNodeModel = modelList.linearSearch(modelAux);
  return auxNodeModel;
}


void Interface::userInterface(List<Model> &modelList)
{
  string stringAux{""};
  Node<Model> *auxNodeModel;
  cout << "Modelos metabolicos: \n";
  do
  {
    option = optionList();
    switch (option)
    {
    case 1:
      cout << "\n1.Agregar Modelo Metabolico: \n";
      addModel(modelList);
      break;
    case 2:
      cout << "\n2.Acceder\n";
      cout << "\nModelos Metabolicos Existentes: \n";
      cout << modelList.toString() << endl;
      cout << "Nombre del Modelo: ";
      cin.ignore();
      getline(cin, stringAux);
      modelAux.setName(stringAux);
      auxNodeModel = modelList.linearSearch(modelAux);
      if (auxNodeModel != nullptr) {
        auxNodeModel->getDataPtr()->chooseList();
      }
      break;
    case 3:
      cout << "\n3.Buscar: \n";
      auxNodeModel = search(modelList);
      if (auxNodeModel != nullptr) {
        cout << "\nDato encontrado: \n" << auxNodeModel->getDataPtr()->toString() << endl;
      } else {
        cout << "\nDato no encontrado... \n";
      }
      break;
    case 4:
      cout << "\n4.Editar: \n";
      cout << "Nombre del Modelo a editar: ";
      cin.ignore();
      getline(cin, stringAux);
      modelAux.setName(stringAux);
      auxNodeModel = modelList.linearSearch(modelAux);
      cout << "\n4.Editar: \n";
      cout << "1.Nombre\n";
      cout << "2.Expresion Objetivo: \n";
      cout << "3.Compartimentos \n";
      cin >> option;

      cout << "\nNuevo: ";
      cin.ignore();
      getline(cin, stringAux);

      if (option == 1) {
        auxNodeModel->getDataPtr()->setName(stringAux);
      } else if ( option == 2) {
        auxNodeModel->getDataPtr()->setObjetiveExpression(stringAux);
      } else {
        auxNodeModel->getDataPtr()->setCompartments(stringAux);
      }

      break;
    case 5:
      cout << "\n4.Eliminar: \n";
      cout << "Nombre del modelo a eliminar: ";
      cin.ignore();
      getline(cin, stringAux);
      modelAux.setName(stringAux);
      auxNodeModel = modelList.linearSearch(modelAux);
      try {
        modelList.remove(auxNodeModel);
        cout << "\nElemento eliminado\n";
      } catch (List<Model>::Exception ex) {
        ex.what();
      }
      break;
    case 6:
      modelList.bubbleSort();
      cout << "\nElementos ordenados\n";
      break;
    default:
      break;
    }
  } while (option != 8);
}

//* -------- ------- ------ ----- Main ----- ------ ------- ---------------- ------- ------ -----  ----- ------ ------- --------

int main(int argc, char const *argv[])
{
  List<Model> modelList;
  Interface myInterface(modelList);
  return 0;
}
