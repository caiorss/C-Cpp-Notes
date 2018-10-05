
#include <iostream>
#include <cmath>

using namespace std;

class vector2D
{
private:
  // Private members
  double x;
  double y;
  
public:
  
  // Class constructors 
  vector2D();
  vector2D(double vX, double vY);

  // Getters
  double getX() const;
  double getY() const;

  void print() const;

  void move(double dx, double dy);

  void setPos(double vX, double vY);

  //-- Operators (Binary Functions/ Class members) -- //

  vector2D operator+ (vector2D B);
  vector2D operator- (vector2D B);
  
  vector2D operator* (double factor);
  vector2D operator/ (double factor);
  
};

vector2D::vector2D() {
  x = 0.0;
  y = 0.0; 
}

vector2D::vector2D(double vX, double vY){
  x = vX;
  y = vY;
}

double vector2D::getX() const{
  return x;
}

double vector2D::getY() const{
  return y;
}


void vector2D::move(double dx, double dy){
  x = x + dx;
  y = y + dy;
}


void vector2D::setPos(double vX, double vY)
{
  x = vX;
  y = vY;
}

void vector2D::print() const
{
  cout << "Vector2D (X = " << x << ", Y = " << y << ")" << endl;
}

vector2D vector2D::operator + (vector2D B){
  vector2D res;
  res.x = x + B.x;
  res.y = y + B.y;
  return res;
}


vector2D vector2D::operator - (vector2D B){
  vector2D res;
  res.x = x - B.x;
  res.y = y - B.y;
  return res;
}


vector2D vector2D::operator * (double factor){
  vector2D res;
  res.x = x * factor; 
  res.y = y * factor;
  return res;
}


vector2D vector2D::operator / (double factor){
  vector2D res;
  res.x = x / factor; 
  res.y = y / factor;
  return res;
}



int main(){

  vector2D vA = vector2D();
  vector2D vB = vector2D(10.23, -8.63);

  vA.print();
  vA.move(2.0, 3.0);
  vA.print();
  
  vB.print();
  vB.move(2.0, 3.0);
  vB.print();

  cout << "\n(1) Vc = vA + vB = " << endl;
  vector2D vC = vA + vB;
  vC.print();

  cout << "\n(2) Vc = vA + vB = " << endl;  
  (vA + vB).print();

  cout << "\n(3) vB * 3.0 = " << endl;
  (vB * 3.0).print();
  
  cout << "\n(4) (vA + vB) / 2.0 = " << endl;
  vector2D vD = (vB + vB) / 2.0 ;
  vD.print();


  // ----------- Pointer Tests ------------------ // 

  vector2D * vp ;

  vp = & vB ;  // Assign pointer to address of vector vB.

  cout  << "\n\nPointer address = " << vp << endl ;


  cout << "\nvp->print()   = " << endl;
  vp->print();

  cout << "\n(*vp).print() = " << endl;  
  (*vp).print();

  cout << "\nvector2D m = *vp; m.print(); = " << endl;  
  vector2D m = *vp;  
  m.print();
  
  return 0;
}
