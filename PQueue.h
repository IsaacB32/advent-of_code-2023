#ifndef ADVENTOFCODE2023_PQUEUE_H
#define ADVENTOFCODE2023_PQUEUE_H
#include <vector>
#include <iostream>

template <typename T>
class PQueue{
public:
    void AddMax(T item, int priority);
    void AddMin(T item, int priority);
    void AddDuplicates(T item, int priority);

    void Remove(T item);
    void RemoveIndex(int index);
    bool ContainsPriority(int priority);
    T get(int index);
    int getPriority(int index);
    T getFromPriority(int priority);
    void Print();
    inline int size(){
        return queue.size();
    };

    inline void GetProperties(int& seenBeforeSize, int& amountOfDuplication){
        seenBeforeSize = seenBefore.size();
        int amountOfDup = 0;
        for (int i = 0; i < seenBeforeSize; ++i) {
            int count = 0;
            for (int j = 0; j < queue.size(); ++j) {
                if(seenBefore[i] == queue[j].item) count++;
            }
            if(count > 1) amountOfDup += count;
        }
        amountOfDuplication = amountOfDup;
    }

private:
    struct TypePriority{
        T item;
        int priority;
    };
    std::vector<TypePriority> queue;
    std::vector<T> seenBefore;
};

template <typename T>
void PQueue<T>::AddMax(T item, int priority) {
    TypePriority p;
    p.item = item;
    p.priority = priority;

    for (int i = 0; i < queue.size(); ++i) {
        if (priority > queue[i].priority) {
            queue.insert(queue.cbegin() + i, p);
            return;
        }
    }
    queue.push_back(p);
}

template <typename T>
void PQueue<T>::AddMin(T item, int priority) {
    TypePriority p;
    p.item = item;
    p.priority = priority;

    for (int i = 0; i < queue.size(); ++i) {
        if (priority < queue[i].priority) {
            queue.insert(queue.cbegin() + i, p);
            return;
        }
    }
    queue.push_back(p);
}

template <typename T>
void PQueue<T>::AddDuplicates(T item, int priority){
    TypePriority p;
    p.item = item;
    p.priority = priority;

    if(!FileReader::Contains(seenBefore, p.item)) seenBefore.push_back(p.item);


    for (int i = 0; i < queue.size(); ++i) {
        if(priority < queue[i].priority)
        {
            queue.insert(queue.cbegin() + i, p);
            return;
        }
    }
    queue.push_back(p);
}

template <typename T>
void PQueue<T>::Remove(T item){
    if(queue.empty()) return;

    for (int i = 0; i < queue.size(); ++i) {
        if(queue[i].item == item)
        {
            queue.erase(queue.cbegin() + i);
        }
    }
}

template <typename T>
void PQueue<T>::RemoveIndex(int index){
    if(queue.empty()) return;
    queue.erase(queue.cbegin() + index);
}

template <typename T>
T PQueue<T>::get(int index) {
    if(index >= queue.size()) cout << "Error" << endl;
    return queue[index].item;
}

template <typename T>
int PQueue<T>::getPriority(int index) {
    if(index < queue.size())
        return queue[index].priority;
    return -1;
}

template <typename T>
T PQueue<T>::getFromPriority(int priority) {
    T element = queue[0].item;
    for (int i = 0; i < queue.size(); ++i) {
        if(queue[i].priority == priority) return queue[i].item;
    }
    return element;
}

template <typename T>
bool PQueue<T>::ContainsPriority(int priority) {
    for (int i = 0; i < queue.size(); ++i) {
        if(queue[i].priority == priority) return true;
    }
    return false;
}

template <typename T>
void PQueue<T>::Print() {
    string c;
    cout << "------Printing PQueue------" << endl;
    for (int i = 0; i < queue.size(); ++i) {
        cout << "Priority " << queue[i].priority << ": " << queue[i].item << endl;
    }
}

#endif //ADVENTOFCODE2023_PQUEUE_H
