#include<iostream>
using namespace std;

class Shape
{
protected:
    string name;
    double area;
    double perimeter;
public:
    Shape();
    Shape(string name_, double area_, double perimeter_){
        this->name = name_;
        this->area = area_;
        this->perimeter = perimeter_;
    }
    virtual float printArea() const {return 0.0;};
    virtual void setArea(double area_);
    virtual double getArea();

};


class Circle:public Shape
{
public:
    Circle(float =0);
    virtual float printArea() const {return 3.14159*radius*radius;}
    virtual void setArea(double area_){
        this->area = area_;
    }
    virtual double getArea(){

    }
protected:
    float radius;
};

//constructor
Circle::Circle(float r):radius(r)
{
}

class Triangle:public Shape
{
public:
    Triangle(float =0,float =0);
    virtual float printArea() const;
    virtual void setArea(double area_){
        this->area = area_;
    }
    //功能5
    friend bool operator == (const Triangle&t1, const Triangle&t2){
        if(t1.name!=t2.name||t1.area!=t2.area||t1.perimeter!=t2.perimeter||
                t1.height!=t2.height||t1.width!=t2.width){
            return false;
        }
        return true;
    }
protected:
    float height;
    float width;
};


Triangle::Triangle(float w,float h):width(w),height(h){
}

float Triangle::printArea()const
{
    return 0.5*width*height;
}

//功能4
void printArea(const Shape&s)
{
    cout<<s.printArea()<<endl;
}

//功能2&4
void modifyArea(Shape &s, double area){
    s.setArea(area);
}


int main()
{
    Circle circle = new Circle();
    Triangle triangle = new Triangle();
    Shape *p[2] = {&circle, &triangle};
    for(int i=0;i<2;i++){

        cout<<p[i]->printArea();
    }

}