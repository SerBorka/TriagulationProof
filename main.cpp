#include <iostream>
#include <vector>
#include "List.h"
#include <algorithm>
/*
// В массиве coords записаны последовательно координаты многоугольника x,y по парам, то есть x и y на позициях разной четности.
// Необходимо, чтобы они составляли многоугольник и были заданны в одном направлении, по часовой стрелке или против - не важно
std::vector<double> coords = { 0, 0,
1, 1,
2, 4,
3, 9,
4, 16,
5, 25,
6, 36,
7, 49,
8, 64,
9, 81};

// В массиве triangles записаны последовательно индексы треугольников по тройкам - первая тройка индексов, первый треугольник и тд.
// Их ориентация не важна, программа ее сама поменяет когда захочет (а еще скажет об этом). Важно: Нумерация индексов вершин начинается с нуля.
std::vector<int> triangles = {6, 7, 8,
6, 8, 9,
6, 9, 0,
6, 0, 1,
6, 1, 2,
6, 2, 3,
6, 3, 4,
6, 4, 5};

// На выход выдается bool - ответ на вопрос правильная триангуляция или нет, а также некоторая сводная информация по работе программы (возможно, скажет, что ей не понравилось в триангуляции)
*/
bool handmade = true; // true - ввод с консоли, false - используются те, что сверху прямо здесь





bool trproof (std::vector<double> &coords, std::vector<int> &triangles) {
    auto maxcooord = std::max_element(coords.begin(), coords.end());
    auto mincooord = std::min_element(coords.begin(), coords.end());
    double inf = 2*(*maxcooord-*mincooord);

    List *polygon = new List;
    polygon->insert(coords); //инициализация многоугольника
    polygon->infin=inf; // инициализация верхней оценки максимальной диагонали многоугольника
    //delete &coords;
    polygon->direction();
    //polygon->lookat(); // Выводит вершины многоугольника
    polygon->inserttriangle(triangles);// инициализация треугольников
    //delete &triangles;


    if (polygon->trianglescheck()) {
        bool proof = polygon->dcheck(*polygon);
        if (!proof) {
            std::cout<<"Triangulation is not correct "<<'\n';
            return proof;
        }
        std::cout<<"Triangulation is correct "<<'\n';
        return proof;
    } else {
        std::cout<<"Triangulation is not correct "<<'\n';
        return false;
    }

}

int main() {
    std::vector<double> coords;
    std::vector<int> triangles;
    if (handmade) {
        //coords.clear();
        //triangles.clear();
        int size;
        std::cout<<"Enter the number of polygon vertexes: "<<'\n';
        std::cin>>size;
        std::cout<<size<<'\n';
        coords.reserve(2*size);
        for (int i = 0; i < size; ++i) {
            double _x;
            double _y;
            //std::cout<<"Enter the x coordinate of vertex "<<i<<" :"<<'\n';
            std::cin>>_x;
            //std::cout<<_x<<'\n';
            coords.emplace_back(_x);
            //std::cout<<"Enter the y coordinate of vertex "<<i<<" :"<<'\n';
            std::cin>>_y;
            //std::cout<<_y<<'\n';
            coords.emplace_back(_y);
            std::cin.clear();
        }
        //std::cout<<coords.size()<<'\n';

        /*for (double coord : coords) {
            std::cout<<coord<<'\n';
        }*/
        int sizetr;
        std::cout<<"Enter the number of triangles in triangulation: "<<'\n';
        std::cin>>sizetr;
        std::cout<<sizetr<<'\n';
        triangles.reserve(3*sizetr);
        for (int i = 0; i < sizetr; ++i) {
            int a;
            int b;
            int c;
            //std::cout<<"Enter the first index of triangle number "<<i+1<<" :"<<'\n';
            std::cin>>a;
            triangles.emplace_back(a);
            //std::cout<<"Enter the second index of triangle number "<<i+1<<" :"<<'\n';
            std::cin>>b;
            triangles.emplace_back(b);
            //std::cout<<"Enter the third index of triangle number "<<i+1<<" :"<<'\n';
            std::cin>>c;
            triangles.emplace_back(c);
            std::cin.clear();
        }
        //std::cout<<triangles.size()<<'\n';
        /*for (int tr : triangles) {
            std::cout<<tr<<'\n';
        }*/
    }
    //bool ret = trproof( coords,triangles);
    //::cout<<"Result "<<ret<<'\n'<<"Enter anything to exit :"<<'\n';
    //int x;
    //std::cin>>x;
    return trproof( coords,triangles);
}