#include "LinkedList.h"
#include "Element.h"

void PrintElementList(ElementLinkedList* elementList)
{
    LOG("Print = [ ");
    for (int i = 0; i < elementList->Count(); i++)
    {
        if (i > 0) { LOG(", "); }
        Element* element = elementList->Get(i);
        LOG("Element::0x%X (%s)", element, element->GetSymbol());
    }
    LOG(" ]\n");
}
