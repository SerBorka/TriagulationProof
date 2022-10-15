#include <iostream>
#include "List.h"
#include <set>
#include <limits>

double nill = std::numeric_limits<double>::epsilon();

template <typename T> int sgn(T val) {
    return (nill < val) - (val < -nill);
}

void List :: insert (double a, double b) { //вставка вершины в конец двусвязного списка
    Node *tmp = new Node; //создание новой вершины
    tmp->x = a; tmp->y = b;
    if ( is_empty() ) {
        tmp->next = tmp;
        tmp->prev = tmp;
        tmp->indN = 0;
        root = tmp;

    }
    else {
        tmp->next = root; // замыкаем новую вершину на первую
        cur->next = tmp;
        tmp->prev = cur;
        tmp->next->prev = tmp;
        tmp->indN = cur->indN + 1;
    }
    cur = tmp;//меняем указатель списка на текущую вершину
    size++;
}

void List :: insert (double *a, int n) { // вставка в конец списка массива вершин
    for ( int i = 0; i < n; i += 2 ) {
        insert (a[i], a[i + 1]);
    }
}

void List :: insert (std::vector<double> a) { // вставка в конец списка массива вершин
    for ( int i = 0; i < a.size(); i += 2 ) {
        insert (a[i], a[i + 1]);
    }
    createnodeinds();
}

void List::createnodeinds() {
    Node* tmp = root;
    nodeinds.clear();
    do {
        nodeinds.push_back(tmp);
        tmp = tmp->next;
    } while (tmp!=root);
}

void List :: lookat(bool ni) const {//вывести в консоль вершины списка и прочую информацию про многоугольник
    Node* tmp = root;
    /*std::cout<<"index Node: "<<tmp->indN<<'\n';
    std::cout<<"var x, y = "<<tmp->x<<", "<<tmp->y<<'\n';
    tmp = tmp->next;*/
    do {
        std::cout<<"index Node: "<<tmp->indN<<'\n';
        std::cout<<"var x, y = "<<tmp->x<<", "<<tmp->y<<'\n';
        tmp = tmp->next;
    } while (tmp!=root);
    std::cout<<"Number of nodes: "<<size<<'\n';
    std::cout<<"Direction: "<<ldir<<'\n';
    //std::cout<<"index Node of root: "<<root->prev->indN<<'\n';
    if (ni) {
        for (auto ndi: nodeinds) {
            std::cout<<"index Node: "<<ndi->indN<<'\n';
        }
    }
}

//векторное произведение векторов, заданных точками {first, second} и {first, third}
double List :: cross_prod (Node *first, Node *second, Node *third) {
    double x1 = second->x - first->x, x2 = third->x - first->x,
            y1 = second->y - first->y, y2 = third->y - first->y;
    return (x1 * y2 - x2 * y1);
}


void List :: direction () {
    Node *tmp = cur, *min = cur;
    double min_x = tmp->x, min_y = tmp->y;
    /*поиск самой левой точки(с минимальным значением абсциссы)
    если таких несколько, выберем нижнюю из них(с минимальным значением ординаты)*/
    if ( !is_empty() ) {
        do {
            if ( tmp->x < min_x ) {
                min_x = tmp->x;
                min = tmp;
            }
            if ( tmp->x == min_x && tmp->y < min_y ) {
                min_y = tmp->y;
                min = tmp;
            }
            tmp = tmp->next;
        } while ( tmp != cur );
        /*направление обхода зависит от знака векторного произведения между векторами,
        задаваемыми "минимальной" вершиной и двумя соседними с ней*/
        ldir = ( ( cross_prod ( min, min->next, min->prev ) < nill) ? "clockwise" : "count_clockwise" );
        return;
    }
    ldir = "error";
}

Node* List::find(int ind, Node* start) {
    Node* tmp = start;
    do {
        if (tmp->indN==ind) {
            return tmp;
        }
        tmp = tmp->next;
    } while (tmp!=start);
    std::cout<<"Invalid index "<<ind<<'\n';
    return nullptr;
}

Node* List::find(int ind) const {
    return nodeinds.at(ind);
}

void List::inserttriangle(int indx, int indy, int indz) {
    Node* x = find(indx);
    Node* y = find(indy);
    Node* z = find(indz);
    auto* tr = new Triangle;
    tr->x = x;
    tr->y = y;
    tr->z = z;
    double detdir = cross_prod(x,y,z);
    if (detdir>=nill) {
        tr->tdir = "count_clockwise";
    } else if (detdir<=-nill) {
        tr->tdir = "clockwise";
    } else {
        std::cout<<"Degenerate triangle whith indexes "<<indx<<", "<<indy<<", "<<indz<<" "<<"was obtained"<<'\n';
        delete tr;
        return;
    }
    if (ldir!="unknown"&&tr->tdir!=ldir) {
        //std::cout<<"Triangle in wrong direction was obtained. Indexes of this triangle: "<<indx<<", "<<indy<<", "<<indz<<'\n';
        tr->y = z;
        tr->z = y;
        //std::cout<<"Triangle indexes were changed to: "<<indx<<", "<<indz<<", "<<indy<<'\n';
        tr->tdir = ldir;
    }
    triangles.push_back(*tr);
    delete tr;
}

void List::inserttriangle(int* a, int n) {
    for ( int i = 0; i < n; i += 3 ) {
        inserttriangle(a[i], a[i + 1],a[i + 2]);
    }

}

void List::inserttriangle(std::vector<int> &a) {
    for ( int i = 0; i < a.size(); i += 3 ) {
        inserttriangle(a[i], a[i + 1],a[i + 2]);
    }

}

bool List::boundingboxcheck (double a,double b,double c,double d) {
    if (a > b)  std::swap (a, b);
    if (c > d)  std::swap (c, d);
    return std::max(a,c) <= std::min(b,d);
}

bool List::intersect(Node *x1, Node *y1, Node *x2, Node *y2) {
    double x1x2y2 = cross_prod(x1, x2, y2);
    double y1x2y2 = cross_prod(y1, x2, y2);
    double y2x1y1 = cross_prod(y2, x1, y1);
    double x2x1y1 = cross_prod(x2, x1, y1);
    if (sgn(x1x2y2)* x1x2y2< nill && sgn(y1x2y2)*y1x2y2 < nill && sgn(y2x1y1)*y2x1y1 < nill && sgn(x2x1y1)*x2x1y1 < nill) {
        return boundingboxcheck(x1->x,y1->x,x2->x,y2->x) && boundingboxcheck(x1->y,y1->y,x2->y,y2->y);
    }
    return (sgn(x1x2y2)*sgn(y1x2y2)<=0)&&(sgn(y2x1y1)*sgn(x2x1y1)<=0);
}

bool List::isinside(Node *x) const {
    if (infin==0){
        std::cout<<"Declare the boundarybox -> infin"<<'\n';
        return false;
    }
    Node* tmp = root;
    Node* inf = new Node;
    inf->x = x->x + infin;
    inf->y = x->y;
    int count = 0;
    do {
        if (tmp->y!=tmp->next->y) { //случай горизонтальной стороны не влияет на количество пересечений
            if (intersect(x,inf,tmp,tmp->next)){ //если пересекаются
                if (x->y==std::max(tmp->y,tmp->next->y)) { //если ребро пересекается сверху счетичик увеличивается
                    ++count;
                } else if (x->y==std::min(tmp->y,tmp->next->y)){ //если ребро пересекается снизу пропускаем
                } else { // если ни сверху ни снизу увеличиваем
                    ++count;
                }
            }
        }
        tmp=tmp->next;
    } while (tmp!=root);
    delete inf;
    return count % 2 != 0;
}

bool List::trianglescheck() {
    unsigned int tsize = triangles.size();
    if (triangles.size()!=size-2) {
        std::cout<<"Not correct number of triangles"<<'\n'<<"Current number of triangles is "<<tsize<<'\n';
        return false;
    }
    if (size-2==1) {
        std::cout<<"Triangulation of triangle is this triangle "<<'\n';
        return true;
    }
    std::cout<<"Number of triangles is "<<tsize<<'\n';
    std::set<int> sidenodes;
    std::set<std::pair<int,int>> diagonalscheck;
    for (int i = 0; i < tsize; ++i ) {
        Triangle ctr = triangles[i];
        //std::cout<<"Triangle direction "<<ctr.tdir<<'\n';
        Node* x = ctr.x;
        Node* y = ctr.y;
        Node* z = ctr.z;
        std::pair<Node*,Node*>  pairs[3] = {{x,y},{y,z},{z,x}};
        for (auto & pair : pairs) {
            //std::cout<<"First ind "<<pair.first->indN<<" Second ind "<<pair.second->indN<<'\n';
            //std::cout<<"difference "<<abs(pair.second->indN-pair.first->indN)%(size-2)<<'\n';
            //if (abs(pair.second->indN-pair.first->indN)%(size-2)==1) {


            if (lendiag(pair.second->indN,pair.first->indN)==1) { // условие того, что рассматривается сторона треугольника
                sidenodes.insert(pair.first->indN);
                sidenodes.insert(pair.second->indN);// логарифмическая сложность вставки
                //std::cout<<"First ind "<<pair.first<<" Second ind "<<pair.second<<'\n';
            } else { // если это не сторона - значит диагональ
                if (pair.first->indN <= pair.second->indN) {
                    diagonals.emplace_back(pair.first,pair.second); // добавляем диагональ в хранилище диагоналей по правилу - меньший индекс больший индекс
                }
                diagonalscheck.insert({pair.second->indN, pair.first->indN});// добавляем инвертированную диагональ, логарифмическая сложность вставки
                diagonalscheck.insert({pair.first->indN, pair.second->indN});// добавляем диагональ
            }
        }
    }
    /*std::set<std::pair<int,int>> ::iterator it;

    for(it = diagonalscheck.begin(); it != diagonalscheck.end(); it++) {

        std::cout << (it->first) << " " << (it->second) << '\n';
    }
    */
    //std::cout<<diagonals.size()<<'\n';
    //std::cout<<sidenodes.size()<<'\n';

    /*
    for (int i = 0; i < size; ++i) {
        if (sidenodes.find(i) == sidenodes.end()) {
            std::cout<<"missing side "<<i<<" "<<i+1<<'\n';
        }
    }*/


    if (sidenodes.size()!=size) { // условие того, что каждая сторона многоугольника принадлежит ровно одному треугольнику
        std::cout<<"Triangulation does not includes all sides "<<'\n';
        return false;
    }
    if (diagonalscheck.size()!=2*(size-3)) { // условие того, что количество кандидатов в диагонали верное
        std::cout<<"Wrong number of diagonals "<<diagonalscheck.size()/2<<'\n';
        return false;
    }
    return true;
}

bool List::isdiagintersectwithpolynom(Node* a,Node* b) const {
    Node* tmp = root;
    do {
        if (tmp->indN==a->indN||tmp->indN==b->indN||tmp->next->indN==a->indN||tmp->next->indN==b->indN) { //смежные диагонали вершины пропускаем
        } else {
            if (intersect(a,b,tmp,tmp->next)) { // проверка на пересечение
                std::cout<<"Side whith indexes "<<tmp->indN<<", "<<tmp->next->indN<<" intersects with diagonal"<<'\n';
                return true;
            }
        }
        tmp = tmp ->next;
    } while (tmp!=root);
    return false;
}

bool List::diagsintresects(int d1, int d2, int i1, int i2) { // работает только для d1<d2 & i1<i2, иначе нужно отсортировать
    return !(((i1<=d2&&i1>=d1)&&(i2<=d2&&i2>=d1))||((i1>=d2||i1<=d1)&&(i2>=d2||i2<=d1)));
}

void List::lookatdiag() {
    for (auto dg:diagonals) {
        std::cout<<"psize: "<<size<<" Diagonal = "<<dg.first->indN<<", "<<dg.second->indN<<'\n';
    }
    std::cout<<"psize: "<<size<<" Number of diagonals = "<<diagonals.size()<<'\n';
}

int List::lendiag (int ind1, int ind2) const {
    return std::min(abs(ind2-ind1),size-abs(ind2-ind1));
}



bool List::dcheck(List& partpol) {
    //Если нет диагоналей - значит треугольник, выходим из рекурсии
    if (partpol.diagonals.empty()) {
        //std::cout<<"Triangle "<<diagonals.size()<<'\n';
        delete &partpol;
        return true;
    }
    // Найдем максимальную диагональ
    int mdind = 0;
    for (int i = 0; i < partpol.diagonals.size(); ++i ) {
        std::pair<Node*,Node*> diag = partpol.diagonals[i];
        if (partpol.lendiag(diag.first->indN,diag.second->indN)>mdind) {
            mdind = i;
        }
    }
    std::pair<Node*,Node*> mdiag = partpol.diagonals[mdind];
    int md1 = mdiag.first->indN;
    int md2 = mdiag.second->indN;

    //std::cout<<"Max diagonal whith indexes "<<md1<<", "<<md2<<'\n';

    // Проверим, не лежит ли она снаружи
    Node* dmid = new Node;
    dmid->x = (mdiag.first->x+mdiag.second->x)/2;
    dmid->y = (mdiag.first->y+mdiag.second->y)/2;
    if (!partpol.isinside(dmid)) { // проверка на принадлежность диагонали многоугольнику (не лежит ли она снаружи), линейное время
        partpol.direction();
        std::cout<<"Diagonal "<<mdiag.first->x<<", "<<mdiag.first->y<<"  "<<mdiag.second->x<<", "<<mdiag.second->y<<" is outside of polygon "<<'\n';
        return false;
    }
    delete dmid;

    //Создаем два полигона - части старого справа и слева от диагонали
    List* leftpol = new List;
    List* rightpol = new List;
    leftpol->infin = partpol.infin;
    rightpol->infin = partpol.infin;

    // Заполняем левый полигон
    Node *tmp = mdiag.first;
    do {
        leftpol->insert(tmp->x,tmp->y);
        tmp = tmp->next;
    } while (tmp!=mdiag.second->next);
    leftpol->createnodeinds();
    //leftpol->lookat(1);

    // Заполняем правый полигон
    tmp = mdiag.second;
    do {
        rightpol->insert(tmp->x,tmp->y);
        tmp = tmp->next;
    } while (tmp!=mdiag.first->next);
    rightpol->createnodeinds();

    //rightpol->lookat(1);



    // Проверим все диагонали на пересечение с максимальной

    // partpol.lookatdiag();

    for (auto diag : partpol.diagonals) {
        int i1 = diag.first->indN;
        int i2 = diag.second->indN;
        if (i1==md1&&i2==md2) {
            continue;
        }
        if (i2<=md2&&i1>=md1) {// обрабатываем диагонали с индексами между - левый
            //std::cout<<"Diagonal = "<<i1<<", "<<i2<<" in leftpol"<<'\n';
            leftpol->diagonals.emplace_back(leftpol->find(i1-md1),leftpol->find(i2-md1));
        } else if ((i2>=md2||i2<=md1)&&(i1>=md2||i1<=md1)) {// обрабатываем диагонали с индексами снаружи - правый
            //std::cout<<"Diagonal = "<<i1<<", "<<i2<<" in rightpol"<<'\n';
            int ni1 = (i1 + partpol.size - md2) % partpol.size;
            int ni2 = (i2 + partpol.size - md2) % partpol.size;
            rightpol->diagonals.emplace_back(rightpol->find(std::min(ni1,ni2)),rightpol->find(std::max(ni1,ni2)));
        } else {
            std::cout<<"Diagonals intersection "<<'\n';
            return false;
        }
    }


    //std::cout<<"Diagonals leftpol number "<<leftpol->diagonals.size()<<'\n';
    //leftpol->lookatdiag();
    //std::cout<<"Diagonals rightpol number "<<rightpol->diagonals.size()<<'\n';
    //rightpol->lookatdiag();

    //Проверяем левый и правый полигоны

    delete &partpol;
    return dcheck(*leftpol) && dcheck(*rightpol);
}

bool List::diagonalscheck() {
    if (diagonals.size()!=size-3) {
        std::cout<<"Wrong diagonals number "<<diagonals.size()<<'\n';
        return false;
    }
    std::cout<<"Number of diagonals is "<<diagonals.size()<<'\n';

    Node* dmid = new Node;
    for (int i = 0; i < (size-3); ++i ) {
        std::pair<Node*,Node*> diag = diagonals[i]; // перебор диагоналей
        dmid->x = (diag.first->x+diag.second->x)/2;
        dmid->y = (diag.first->y+diag.second->y)/2;
        if (!isinside(dmid)) { // проверка на принадлежность диагонали многоугольнику (не лежит ли она снаружи), линейное время
            std::cout<<"Diagonal whith indexes "<<diag.first->indN<<", "<<diag.second->indN<<" is outside of triangle "<<diagonals.size()<<'\n';
            return false;
        }

        if (isdiagintersectwithpolynom(diag.first,diag.second)) { //проверка на пересечение диагонали с многоугольником, линейное время
            std::cout<<"Diagonal whith indexes "<<diag.first->indN<<", "<<diag.second->indN<<" intersects the polygon"<<'\n';
            return false;
        }
        for (int j = 0; j < (size-3); ++j) { // проверка на пересечение диагоналей
            std::set<int> st{diagonals[j].first->indN,diag.first->indN,diagonals[j].second->indN,diag.second->indN};
            if (st.size()==4&&intersect(diag.first, diag.second,diagonals[j].first,diagonals[j].second)) {
                std::cout << "Diagonal whith indexes " << diag.first->indN << ", " << diag.second->indN<< " intersects with diagonal with indexes "
                << diagonals[j].first->indN << ", "<< diagonals[j].second->indN << '\n';
                return false;
                }

        }
    }
    delete dmid;

    std::cout<<"Triangulation is correct "<<'\n';
    //std::cout<<lendiag(5,0,6)<<'\n';
    return true;
}









