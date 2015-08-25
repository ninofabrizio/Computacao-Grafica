#include <cstdio>
#include <cmath>
#include <iostream>
using namespace std;

class Rectangle {

    private:
        int width, height;
    
    public:
        void setValues(int, int);
        int area();
        int compDiag();
};

void Rectangle::setValues (int x, int y) {

    width = x;
    height = y;
}

int Rectangle::area () {

    return width * height;
}

int Rectangle::compDiag () {

    return sqrt(pow(width,2) + pow(height, 2));
}

int main () {

    Rectangle r;
    int w, h;

    cout<<"Diga largura e altura: "<<endl;
    cin>>w>>h;

    r.setValues(w, h);
    
    cout<<"Area do retangulo: "<<r.area()<<endl;
    cout<<"Comprimento de sua diagonal: "<<r.compDiag()<<endl;

    return 0;
}
