//
// Created by simbo on 30.09.2022.
//

#ifndef TriagulationProof_LIST_H
#define TriagulationProof_LIST_H


#include <vector>

class Node {  //узел списка(в данной задаче-вершина многоугольника)
public:
    double x, y = 0;//координаты текущей точки
    int indN = 0;  // индекс точки в многоугольнике
    Node *prev, *next = nullptr; //указатели на предыдущую и следующую вершины
    //конструктор по умолчанию
    explicit Node (double a = 0.0, double b = 0.0, int ind_=0): x(a), y(b),indN(ind_), prev(nullptr), next(nullptr) {
        //std::cout<<"Node"<<'\n';
    }
};


class Triangle {
public:
    Node* x = nullptr;
    Node* y = nullptr;
    Node* z = nullptr;
    std::string tdir = "unknown";

};



class List { //кольцевой двусвязный список(будет содержать координаты многоугольника)
public:
    int size = 0; //количество вершин
    Node* cur = nullptr; //указатель на текущую вершину (legacy)
    Node* root = nullptr;// первая вершина с нулевым индексом
    std::vector <Node*> nodeinds = {};
    //Направление, в котором заданы вершины. Возможные значения: clockwise, count_clockwise, error, unknown
    std::string ldir = "unknown";
    std::vector <Triangle> triangles = {};
    std::vector <std::pair<Node*,Node*>> diagonals = {};
    double infin = 0;

    explicit List (int s = 0, Node* tmp = nullptr): size(s), cur(tmp), root(tmp) {}
    ~List() {
        if (nodeinds.size()!=size) {
            createnodeinds();
        }
        for (auto nd:nodeinds) {
            delete nd;
        }
        //std::cout<<"Deleted List "<<size<<'\n';
    }

    bool is_empty () const {
        return size == 0;
    }
    void insert (double a, double b);//вставка одной вершины после текущей
    void insert (double *a, int n); //вставка массива координат
    void insert (std::vector<double> coords);// вставка вектора координат
    void createnodeinds();
    void lookat(bool ni = false) const;
    static double cross_prod (Node *first, Node *second, Node *third);//векторное произведение 12 крест 13. больше нуля - против часовой, меньше - по
    void direction (); //направление обхода многоугольника
    static Node* find (int ind, Node* start);//поиск вершины с заданным индексом
    Node* find(int ind) const;
    void inserttriangle(int indx, int indy, int indz);//вставка одного треугольника триангуляции
    void inserttriangle(int* a, int n);//вставка из массива треугольников
    void inserttriangle(std::vector<int> &triangles);// вставка из вектора треугольников
    static bool intersect(Node* x1,Node* y1, Node* x2, Node* y2); //проверка пересечения двух отрезков
    bool isinside(Node* x) const; // проверка, находтся ли вершина внутри многоугольника
    bool trianglescheck();
    void lookatdiag();
    bool isdiagintersectwithpolynom(Node* a,Node* b) const;
    static bool diagsintresects(int a, int b, int c, int d);
    bool diagonalscheck();
    bool dcheck( List& partpol);
private:
    static bool boundingboxcheck(double a, double b, double c, double d);//для вырожденного случая когда отрезки коллинеарны проверка проверка пересечения двух отрезков отдельно
    int lendiag(int ind1, int ind2) const;
    int lendel(int ind1, int ind2) const;

};


#endif //TriagulationProof_LIST_H
