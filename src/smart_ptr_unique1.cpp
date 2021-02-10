// File:   smart_ptr_unique1.cpp
// Author: Caio Rodrigues
// Brief:  Basic experiment for demonstrating functionalities of std::unique_ptr
//--------------------------------------------------------------

#include <iostream>
#include <string>

#include <type_traits>

// Provide std::unique_ptr and std::shared_ptr
#include <memory>

// Car Engine Interface
class IEngine{
public:
    virtual std::string type() const = 0;
    virtual void        run()  = 0;
    // virtual void        shutdown() = 0;
    virtual ~IEngine() = default;
};

class DieselEngine: public IEngine
{
public:
    DieselEngine()
    {
        std::puts(" [INFO] Diesel Engine created.");
    }

    ~DieselEngine()
    {
        std::puts(" [INFO] Diesel Engine deleted.");
    }

    std::string type() const override
    {
        return "diesel engine";
    }

    void run() override
    {
        std::puts(" Running a super powerful diesel Engine!!");
    }
};

class ElectricEngine: public IEngine
{
public:
    ElectricEngine()
    {
        std::puts(" [INFO] Electrict Engine created.");
    }

    ~ElectricEngine()
    {
        std::puts(" [INFO] Electric Engine deleted.");
    }

    std::string type() const override
    {
        return "electric engine";
    }

    void run() override
    {
        std::puts(" =>> Running a silent and clean electric engine!!");
    }
};


class SomeCar{
private:
    std::unique_ptr<IEngine> m_engine;
public:

    SomeCar(): m_engine(nullptr) { }

    // Constructor: Dependency injection
    // => the dependency is supplied and instantiated
    // by an external code rather than being instantiated by this class.
    SomeCar(IEngine* obj): m_engine(obj){ }

    // Unique_ptr cannot be copied, they only can be moved.
    // The std::move, transfers the ownership of the supplied std::unique_ptr
    // to this class.
    SomeCar(std::unique_ptr<IEngine> engine)
        : m_engine(std::move(engine))
    {
    }

    void run_engine(){
        std::puts(" [INFO] Run car engine.");
        // Unique_ptr can be used just as an ordinary pointer
        m_engine->run();
    }

    // Set a new engine.
    void set_engine(IEngine* engine)
    {
        // Disposes the old engine and sets a new one
        m_engine.reset(engine);
    }

    void set_engine(std::unique_ptr<IEngine> engine)
    {
        // Calls move assingment operator disposing the old engine
        m_engine  = std::move(engine);
    }

    std::string engine_type() {
        return m_engine->type();
    }

    void remove_engine() {
        // Delete wrapped heap-allocated object
        m_engine.release();
    }

    // Check if the car has an engine
    bool has_engine(){
        return m_engine != nullptr;
    }

    ~SomeCar(){
        std::puts(" [INFO] object of class SomeCar disposed. OK");
    }

};


std::unique_ptr<IEngine>
factoryFunction(char code)
{
    if(code == 'e') { return std::make_unique<ElectricEngine>(); }
    if(code == 'd') { return std::make_unique<DieselEngine>(); }
    return nullptr;
}


int main()
{
    std::cout << "\n ***** EXPERIENT 0 === Type Traits *********************\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << std::boolalpha;
    {

        std::cout << " is_abstract<IEngine>() = "
                  << std::is_abstract<IEngine>() << "\n";

        std::cout << " is_abstract<ElectricEngine>() = "
                  << std::is_abstract<ElectricEngine>() << "\n";

        std::cout << " is_fundamental<ElectricEngine>() = "
                  << std::is_fundamental<ElectricEngine>() << "\n";

        std::cout << " is_polymorphic<ElectricEngine>() = "
                  << std::is_polymorphic<ElectricEngine>() << "\n";

        std::cout << " is_trivially_copiable<ElectricEngine() = "
                  << std::is_trivially_copyable<ElectricEngine>() << "\n";

        std::cout << " is_trivially_move_constructible<ElectricEngine() = "
                  << std::is_trivially_move_constructible<ElectricEngine>() << "\n";

    }


    std::cout << "\n ***** EXPERIMENT 1 = Instatiate a car with new operator ****\n ";
    std::cout << "---------------------------------------------------------------\n";
    {
        // Note: note recommended using 'new', instead use std::make_unique
        SomeCar car1(new DieselEngine);
        std::cout << " car1.has_engine() = " << car1.has_engine() << "\n";
        std::cout << " car1.engine_type() = " << car1.engine_type() << "\n";
        car1.run_engine();

        // Car and engine deleted at the end of this scope or at this bracket.
    }

    std::cout << "\n **** EXPERIMENT 2 = Instatiate a car with an exising unique_ptr ****\n ";
    std::cout << "---------------------------------------------------------------\n";
    {
        // Not recommended using new!
        std::unique_ptr<IEngine> engineA(new ElectricEngine);

        // Note: engineA is not copiable, can only be moved
        SomeCar carA(std::move(engineA));
        std::cout << " carA.has_engine() = " << carA.has_engine() << "\n";
        std::cout << " carA.engine_type() = " << carA.engine_type() << "\n";
        carA.run_engine();

        // Remove engine of carA and sell it at the market
        carA.remove_engine();
        std::cout << " After removal of carA's engine." << "\n";
        std::cout << " car1.has_engine() = " << carA.has_engine() << "\n";

        std::cout << "\n After removing engine of CarA" << "\n";
        carA.set_engine(new DieselEngine);
        std::cout << " carA.engine_type() = " << carA.engine_type() << "\n";
        carA.run_engine();
    }

    std::cout << "\n **** EXPERIMENT 3 = Using std::make_unique ****\n ";
    std::cout << "---------------------------------------------------------------\n";
    {
        SomeCar carA(std::make_unique<DieselEngine>());
        std::cout << " carA.has_engine() = " << carA.has_engine() << "\n";
        std::cout << " carA.engine_type() = " << carA.engine_type() << "\n";
        carA.run_engine();


        std::cout << "\n [TRACE] After setting a new engine for CarA" << "\n";
        auto engine = std::make_unique<ElectricEngine>();
        std::cout << " ?? engine == nullptr " << (engine == nullptr) << std::endl;

        carA.set_engine(std::move(engine));

        std::cout << " carA.engine_type() = " << carA.engine_type() << "\n";
        carA.run_engine();

        std::cout << "\n [TRACE] After setting a new engine again for CarA" << "\n";
        carA.set_engine( std::make_unique<DieselEngine>() );
        std::cout << " carA.engine_type() = " << carA.engine_type() << "\n";
        carA.run_engine();

    }

    std::cout << "\n **** EXPERIMENT 4 = Factory Function ****\n ";
    std::cout << "---------------------------------------------------------------\n";
    {
        SomeCar car;
        auto engine1 = factoryFunction('x');

        // If the pointer is not nullptr (null) evalutes to true
        if(engine1){
            std::cout << " Engine created from factory function. Ok. \n";
        } else {
            car.set_engine(std::move(engine1));
        }
        std::cout << " ??? car.has_engine() = " << car.has_engine() << std::endl;

        std::cout << "\n Set engine to DIESEL engine.\n";
        car.set_engine(factoryFunction('d'));
        std::cout << " ??? car.has_engine() = " << car.has_engine() << std::endl;
        std::cout << " Engine type = " << car.engine_type() << "\n";
        car.run_engine();

        std::cout << "\n Set engine to ELECTRIC engine.\n";
        car.set_engine(factoryFunction('e'));
        std::cout << " ??? car.has_engine() = " << car.has_engine() << std::endl;
        std::cout << " Engine type = " << car.engine_type() << "\n";
        car.run_engine();
    }


    return 0;
}
