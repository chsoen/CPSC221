/**
 * @file heap.cpp
 * Implementation of a heap class.
 */

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    return 0;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    return 2*currentIdx+1;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    return 2*currentIdx+2;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    return (currentIdx-1)/2;
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    return leftChild(currentIdx) < _elems.size();
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    size_t left = leftChild(currentIdx);
    size_t right = rightChild(currentIdx);
    if(right >= _elems.size()) {
        return left;
    }
    return higherPriority(_elems[left], _elems[right]) ? left : right;
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    if(hasAChild(currentIdx)) {
        size_t betterChild = maxPriorityChild(currentIdx);
        if(higherPriority(_elems[betterChild], _elems[currentIdx])) {
            std::swap(_elems[betterChild], _elems[currentIdx]);
            heapifyDown(betterChild);
        }
    }
}


template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    if (currentIdx == root())
        return;
    size_t parentIdx = parent(currentIdx);
    if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
        std::swap(_elems[currentIdx], _elems[parentIdx]);
        heapifyUp(parentIdx);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{
    /// @todo Depending on your implementation, this function may or may
    ///   not need modifying
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems)
{
    _elems = elems;
    for(size_t i=root(); i<_elems.size(); i++) {
        printf("%d\n", (int) i);
        heapifyDown(_elems.size()-i-1);
    }
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    T top = peek();
    _elems[root()] = _elems.back();
    _elems.pop_back();
    heapifyDown(root());
    return top;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    return _elems[root()];
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    _elems.push_back(elem);
    heapifyUp(_elems.size()-1);
}

template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    return _elems.size() == root();
}
