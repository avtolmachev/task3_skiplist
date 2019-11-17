////////////////////////////////////////////////////////////////////////////////
// Module Name:  skip_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include skip_list.h here, 'cause it leads to circular refs. !!!

#include <cstdlib>

//==============================================================================
// class NodeSkipList
//==============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value, Key, numLevels>::clear(void)
{
    for (int i = 0; i < numLevels; ++i)
        Base::nextJump[i] = 0;

    Base::levelHighest = -1;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
    clear();
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey)
{
    clear();

    Base::Base::key = tkey;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey, const Value& val)
{
    clear();

    Base::Base::key = tkey;
    Base::Base::value = val;
}


//==============================================================================
// class SkipList
//==============================================================================

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
    _probability = probability;

    // Lets use m_pPreHead as a final sentinel element
    for (int i = 0; i < numLevels; ++i)
        Base::_preHead->nextJump[i] = Base::_preHead;

    Base::_preHead->levelHighest = numLevels - 1;
}

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::~SkipList()
{
    Node* temp;
    Node* currentNode = Base::_preHead->next;

    if (currentNode != Base::_preHead)
    while (currentNode != Base::_preHead)
        do{
            temp = currentNode->next;
            delete currentNode;
            currentNode = temp;
        }while(currentNode != Base::_preHead);

    delete currentNode;
}
//----------------------------------------------------------------------------—

template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(Node * nodeBefore)
{
    if (!nodeBefore || nodeBefore->next == Base::getPreHead() || !nodeBefore->next)
        throw std::invalid_argument("");
    else
    {
        Node *remove = nodeBefore->next;
        for (int i = remove->levelHighest; i > -1; i--)
        {
            nodeBefore->nextJump[i] = remove->nextJump[i];
            remove->nextJump[i] = 0;
        }
        nodeBefore->next = remove->next;
        remove->levelHighest = -1;
        remove->next = 0;
    }
}

static double myRandomDoubleNum()
{
    return (rand() / RAND_MAX);
}

template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::insert(const Value & val, const Key & key)
{
    Node* slider = Base::getPreHead();
    Node* lastNodes[slider->levelHighest];

    for (int i = numLevels - 1; i > -1; i--)
    {
        if (slider->nextJump[i] != Base::getPreHead() && slider->nextJump[i]->key <= key)
             do
             {
                 slider = slider->nextJump[i];
             }while(slider->nextJump[i] != Base::getPreHead() && slider->nextJump[i]->key <= key);

        lastNodes[i] = slider;
    }

    Node* nodeToInsert = new Node(key, val);
    int levelsNum = -1;
    while (myRandomDoubleNum() < _probability && levelsNum < Base::getPreHead()->levelHighest)
        ++levelsNum;

    nodeToInsert->levelHighest = levelsNum;

    if (slider->next != Base::getPreHead() && slider->next->key <= key)
        do
        {
            slider = slider->next;
        }while(slider->next != Base::getPreHead() && slider->next->key <= key);

        nodeToInsert->next = slider->next;
    slider->next = nodeToInsert;
    for (int i = 0; i <= levelsNum; i++)
    {
        nodeToInsert->nextJump[i] = lastNodes[i]->nextJump[i];
        lastNodes[i]->nextJump[i] = nodeToInsert;
    }

}
//----------------------------------------------------------------------------—

template<class Value, class Key, int numLevels>
typename SkipList<Value, Key, numLevels>::Node* SkipList<Value, Key, numLevels>::findFirst(const Key & key) const
{
    Node* current = Base::getPreHead();
    for (int i = numLevels - 1; i > -1; --i)
        if (current->nextJump[i] != Base::getPreHead() && current->nextJump[i]->key < key)
            do{
                current = current->nextJump[i];
            }while(current->nextJump[i] != Base::getPreHead() && current->nextJump[i]->key < key);

    if (current->next != Base::getPreHead() && current->next->key < key)
        do
        {
            current = current->next;
        }while(current->next != Base::getPreHead() && current->next->key < key);
    current = current->next;
    if (current != Base::getPreHead() && current->key == key)
       return current;
    return nullptr;
}

//----------------------------------------------------------------------------—

template<class Value, class Key, int numLevels>
typename SkipList<Value, Key, numLevels>::Node* SkipList<Value, Key, numLevels>::findLastLessThan(const Key & key) const
{
    Node* slider = Base::getPreHead();
    for (int i = numLevels - 1; i > -1; --i)
    if (slider->nextJump[i] != Base::getPreHead() && slider->nextJump[i]->key < key)
        do
            slider = slider->nextJump[i];
        while(slider->nextJump[i] != Base::getPreHead() && slider->nextJump[i]->key < key);
    if (slider)
        return slider;
    return Base::getPreHead();
}


