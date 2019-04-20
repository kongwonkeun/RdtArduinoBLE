/*
 * LinkedList.h
 *
 * Created: 2019-04-20 11:17:14
 *  Author: kong
 */ 

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stddef.h>

template<class T> struct ListNode
{
    T data;
    ListNode<T>* next;
};

template <typename T> class LinkedList
{
public:
    LinkedList();
    ~LinkedList();
    void add(T); // add to the end of list
    T get(unsigned int index);
    void clear();
    unsigned int size();
private:
    unsigned int m_size;
    ListNode<T>* m_head;
    ListNode<T>* m_tail;
};

template <typename T> LinkedList<T>::LinkedList() : 
m_size(0), m_head(NULL), m_tail(NULL)
{
}

template <typename T> LinkedList<T>::~LinkedList()
{
    clear();
}

template <typename T> void LinkedList<T>::add(T item)
{
    ListNode<T>* node = new ListNode<T>();
    node->data = item;
    node->next = NULL;
    if (m_head) {
        m_tail->next = node;
    } 
    else {
        m_head = node;
    }
    m_tail = node;
    m_size++;
}

template <typename T> T LinkedList<T>::get(unsigned int index)
{
    if (index >= m_size) {
        return T();
    }
    ListNode<T>* node = m_head;
    for (unsigned int i = 0; i < index; i++) {
        node = node->next;
    }
    return node->data;
}

template <typename T> void LinkedList<T>::clear()
{
    ListNode<T>* node = m_head;
    for (unsigned int i = 0; i < m_size; i++) {
        ListNode<T>* x = node;
        node = node->next;
        delete x;
    }
    m_head = NULL;
    m_tail = NULL;
    m_size = 0;
}

template <typename T> unsigned int LinkedList<T>::size()
{
    return m_size;
}

#endif /* LINKEDLIST_H_ */