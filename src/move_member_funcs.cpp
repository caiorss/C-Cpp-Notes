// Author: Caio Rodrigues
// Brief:  Move member functions proof-of-concept code. 
//---------------------------------------------------------------------------------

#include <iostream>

class AClass{
public:
    // Object unique ID for easier identification
    const int m_id;
    // Simulates a resource (heap-allocated object)
    int* m_p;

    static int make_id()
    {
        static int id = 0;
        return ++id;
    }

    // Default constructor
    AClass(): AClass(0) { }

    AClass(int n): m_id{AClass::make_id()}, m_p{new int {n}}
    {
        printf(" [TRACE] Object created => id = %d ; *m_p = %d\n", m_id, *m_p);
    }

    // Copy constructor
    AClass(AClass const& that): m_id{AClass::make_id()}
    {
        m_p = new int (*that.m_p);
        printf(" [TRACE] Copy ctor invoked => Copied src=%d dest=%d \n"
               , that.m_id, m_id);
    }

    // Copy assignment operator
    AClass& operator=(AClass const& that)
    {
        printf(" [TRACE] Copy asn. operator invoked. => src_id = %d - dest_id = %d \n"
               , that.m_id, m_id);
        *m_p = *that.m_p;
        return *this;
    }

#if 1
    // Move constructor: move resource from a temporary object (RValue) to a new object.
    AClass(AClass&& that): m_id{AClass::make_id()}
    {
        printf(" [TRACE] Move ctor invoked OK. => src_id = %d - dest_id = %d \n"
               , that.m_id, m_id);
        // Transfer ownership of resource managed by temporary object that
        // to this object.
        m_p = that.m_p;
        // Make the temporary object empty
        that.m_p = nullptr;
    }

    // Move assignment operator: move resource from a temporary object (RValue)
    // to an existing object (this).
    AClass& operator=(AClass&& that)
    {
        printf(" [TRACE] Move asn operator invoked. OK. src_id = %d - dest_id = %d \n"
               , that.m_id, m_id);
        int* p_temp = m_p;
        // Transfer ownership of resources from the temporary object(that) to
        // this object.
        m_p = that.m_p;
        // Diposes the resource of the existing object (this)
        that.m_p = p_temp;
        return *this;
    }
#endif

    // Destructor
    ~AClass()
    {
        printf(" [TRACE] Object destroyed => Dtor called => id = %d \n", m_id);
        delete m_p;
        m_p = nullptr;
    }

    int get() const { return *m_p; }
    void set(int n) { *m_p = n; }
};

AClass make_object(int n)
{
    AClass cls(4);
    cls.set(cls.get() + n);
    printf(" [TRACE] FUN = %s => Created object => id = %d ; cls.get() = %d\n"
           , __FUNCTION__, cls.m_id, cls.get());
    return cls;
}

void is_lvalue_or_rvalue(AClass& arg)
{
    printf(" ===> Passed LVALUE \n");
}

void is_lvalue_or_rvalue(AClass&& arg)
{
    printf(" ===> Passed RVALUE \n");
}


int main()
{
    // Invoke default ctor
    AClass obj1;
    AClass obj2(8);
    AClass obj3(10);

    printf(" => obj1.id() = %d / obj1.get() = %d \n", obj1.m_id, obj1.get());
    printf(" => obj2.id() = %d / obj2.get() = %d \n", obj2.m_id, obj2.get());
    printf(" => obj3.id() = %d / obj3.get() = %d \n", obj3.m_id, obj3.get());

    printf("\n EXPERIMENT 1 ==>> Test whether value is Lvalue or rvalue\n");
    printf("--------------------------------------------------\n");

    is_lvalue_or_rvalue( obj1 );            // LValue
    is_lvalue_or_rvalue( AClass(5) );       // RValue => Temporary object
    is_lvalue_or_rvalue( make_object(0));   // RValue => Temporary object


    printf("\n EXPERIMENT 2 ==>> Test copy constructor and move constructor\n");
    printf("--------------------------------------------------\n");

    AClass objA(obj1);
    printf(" => objA.id() = %d - objA.value = %d\n", objA.m_id, objA.get());

    AClass objB = obj2;
    printf(" => objB.id() = %d - objB.value = %d\n", objB.m_id, objB.get());

    // Nor copy constructor neither move constructors are invoked
    // due to RVO => Return-Value Optimization.
    AClass objD = make_object(5);
    printf(" => objD.id() = %d - objD.value = %d\n", objD.m_id, objD.get());

    // Force invocation of move constructor - moving the ownership of the resources
    // of obj3 ot objE. std::move makes obj3 to be interpreted as n R-value
    AClass objE{std::move(obj3)};
    printf(" => objE.id() = %d - objE.value = %d\n", objE.m_id, objE.get());

    printf("\n EXPERIMENT 3 ==>> Test move or copy assignment operator <<========= \n");
    printf("--------------------------------------------------\n");

    // Invoke copy assignment operator
    obj1 = obj2;
    printf(" => [A] obj1.get() = %d \n", obj1.get());

    // Invoke move assignment operator
    obj1 = std::move(obj2);
    printf(" => [A2] obj1.get() = %d \n", obj1.get());

    // Invoke copy assignment operator or move assignment operator
    obj1 = AClass(6);
    printf(" => [C] obj1.get() = %d \n", obj1.get());

    // Invoke copy assignment operator or move assignment operator
    obj1 = make_object(3);
    printf(" => [B] obj1.get() = %d \n", obj1.get());

    printf("\n ====>> Exit main() ==> Shutdown() ============\n");
    return 0;
}
