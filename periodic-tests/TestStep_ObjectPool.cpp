#include "TestSteps.h"
#include "Test.h"
#include "ObjectPool.h"

#define TEST_OBJECT_POOL_SIZE 10

class TestObject : public ObjectPool<TestObject, TEST_OBJECT_POOL_SIZE>
{
private:
    int num;

public:
    TestObject() {}

    TestObject(int num)
    {
        this->num = num;
    }

    int GetNum()
    {
        return num;
    }
};

void TestStep_ObjectPool()
{
    TestMessage("Test allocation and initialization.");

    TestObject* a = new TestObject(10);
    TestNePointer("Verify that object A was allocated successfully.", a, NULL);
    TestEqInt("Verify that object A was initialized correctly.", a->GetNum(), 10);

    TestObject* b = new TestObject(20);
    TestNePointer("Verify that object B was allocated successfully.", b, NULL);
    TestObject* c = new TestObject(50);
    TestNePointer("Verify that object C was allocated successfully.", c, NULL);

    TestNePointer("Verify that A and B aren't the same.", a, b);
    TestNePointer("Verify that B and C aren't the same.", b, c);

    TestEqInt("Verify that object A has the correct value.", a->GetNum(), 10);
    TestEqInt("Verify that object B has the correct value.", b->GetNum(), 20);
    TestEqInt("Verify that object C has the correct value.", c->GetNum(), 50);

    TestMessage("Test deallocation.");
    delete a;
    delete b;
    delete c;

    TestMessage("Test allocating every object in the pool.");
    for (int i = 0; i < TEST_OBJECT_POOL_SIZE; i++)
    {
        TestObject* object = new TestObject(i);
        TestNePointer("Verify that the object was allocated successfully.", object, NULL);
        TestEqInt("Verify that the object has the correct value.", object->GetNum(), i);
    }

    //TODO: Test allocating more objects than are available in the pool. (Need a way to verify asserts.)
}
