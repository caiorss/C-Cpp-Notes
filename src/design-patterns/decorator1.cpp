// Brief:   Implementation of decorator design pattern using C++11.
// Note:    Objects allocated on the heap managed by shared pointer. 
// Author:  Caio Rodrigues 
//------------------------------------------------------------------

#include <iostream>
#include <string>
#include <memory>

// Interface IShape (IComponent) 
struct IShape
{
	virtual auto draw()        -> void = 0;
	virtual auto description() -> std::string = 0;
	virtual ~IShape() = default;
};

// Component class
class Square: public IShape
{
public:
	auto draw() -> void
	{
		std::cout << " => Draw square" << std::endl;
	}
	auto description() -> std::string 
	{
		return "square";
	}
};

// Component class 
class Triangle: public IShape
{
public:
	auto draw() -> void
	{
		std::cout << " => Draw tringle" << std::endl; 
	}
	auto description() -> std::string
	{
		return "triangle";
	}
};

// Decorator 1 => Draw shape with color
class ColorDecorator: public IShape {
public:
	ColorDecorator(std::shared_ptr<IShape> shape)
		: m_shape(shape) { }
	auto draw() -> void
	{
		// Save color:   push() 
		std::cout << " => [ColorDecorator] Draw object with color blue" << std::endl;
		m_shape->draw();
		// Restore color: pop()
	}
	auto description() -> std::string
	{
		return  m_shape->description() + " ; color = " + m_color;
	}
	// Return a reference to itself (ColorDecorator&)
	auto setColor(const std::string& color) -> decltype(*this)& {
		m_color = color;
		return *this;
	}
private:
	// The decorator owns the decorated object 
	std::shared_ptr<IShape>  m_shape;
	std::string   m_color = "blue";
};

class PositionDecorator: public IShape {
public:
	PositionDecorator(std::shared_ptr<IShape> shape)
		: m_shape(shape){ }

	auto draw() -> void
	{
		// Save transformation matrix:   pushMatrix() 
		std::cout << " =>  [PositionDecorator] Draw object at x = "
				  << m_x << " ; y = " << m_y << std::endl;
		m_shape->draw();
		// Restore transformation matrix: popMatrix()
	}
	auto description() -> std::string
	{
		return  m_shape->description() + " ; position x  = "
			+ std::to_string(m_x) +  " , y = " + std::to_string(m_y);
	}
	auto setPosition(double x, double y) -> PositionDecorator& {
		m_x = x, m_y = y;
		return *this;
	}	
private:	
	// The decorator owns the decorated object 
	std::shared_ptr<IShape> m_shape;
	int m_x = 0, m_y = 0;
};


int main(){
	
	std::puts("\n ======>> Experiment 1 <<===========");
	auto shape = std::make_shared<ColorDecorator>(std::make_shared<Square>());
	shape->setColor("yellow");
	shape->draw();
	
	std::cout << " DESCRIPTION = " << shape->description() << std::endl;	

	std::puts("\n ======>> Experiment 2 <<===========");
	auto observerd = std::shared_ptr<IShape>{nullptr};
	
	auto shapeWithColorAndPosition =
		[&observerd]{
			auto shape        = std::make_shared<Triangle>();
			observerd         = shape;
			auto shapeColored = std::make_shared<ColorDecorator>(shape);
			shapeColored->setColor("white");
			auto shapePos     = std::make_shared<PositionDecorator>(shapeColored);			
			return  shapePos;
		}();
	
	shapeWithColorAndPosition->setPosition(100, 20);
	shapeWithColorAndPosition->draw();
	std::cout << " DESCRIPTION = "
			  << shapeWithColorAndPosition->description() << std::endl;

	std::cout << " [INFO] observed shape = " << observerd->description() << std::endl;
	
	return 0;
}
