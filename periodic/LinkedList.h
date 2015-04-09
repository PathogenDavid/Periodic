#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include "ObjectPool.h"

template <class T, int GlobalCapacityT>
class LinkedList
{
private:
    class LinkedListNode : public ObjectPool<LinkedListNode, GlobalCapacityT>
    {
    public:
        LinkedListNode* next;
        T value;

        LinkedListNode()
        { }

        LinkedListNode(T value)
        {
            this->value = value;
        }
    };

    LinkedListNode* root = NULL;
    int count = 0;
public:
    void Add(T item)
    {
        LinkedListNode** node = &root;
        
        while (*node != NULL)
        { node = &(*node)->next; }
        
        *node = new LinkedListNode(item);
        count++;
    }

    void RemoveAt(int index)
    {
        Assert(index >= 0 && index < count); // Assert index is within bounds

        LinkedListNode* node = root;
        LinkedListNode** newNextLocation = &root;
        while (node != NULL && index >= 0)
        {
            if (index == 0)
            {
                *newNextLocation = node->next;
                delete node;
                count--;
                return;
            }

            newNextLocation = &node->next;
            node = node->next;
            index--;
        }

        AssertAlways(); // If we reach here, something terrible has happened.
    }

    bool Remove(T item)
    {
        int index = IndexOf(item);
        if (index < 0)
        { return false; }

        RemoveAt(index);
        return true;
    }

    int IndexOf(T item)
    {
        LinkedListNode* node = root;
        int index = 0;
        while (node != NULL)
        {
            if (node->value == item)
            { return index; }

            index++;
            node = node->next;
        }

        return -1;
    }

    T Get(int index)
    {
        Assert(index >= 0 && index < count); // Assert index is within bounds

        LinkedListNode* node = root;
        while (node != NULL && index > 0)
        {
            index--;
            node = node->next;
        }

        Assert(node != NULL); // If node is NULL here, something terrible has happened.
        return node->value;
    }

    T operator[](int index)
    {
        return Get(index);
    }

    unsigned int Count()
    {
        return count;
    }

    void Clear()
    {
        while (count > 0)
        { RemoveAt(0); }
    }

    ~LinkedList()
    {
        Clear();
    }
};

class Element;
typedef LinkedList<Element*, 20> ElementLinkedList;

void PrintElementList(ElementLinkedList* elementList);

#endif
