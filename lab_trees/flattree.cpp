using namespace std;

template <typename T>
void FlatTree<T>::printLeftToRight() const {
    // Your code here

    // Do not remove this line - used for correct print output
    cout << endl;
}

template <typename T>
T& FlatTree<T>::getElement(const string& path) {
    // Your code here

    return data.front(); // STUB, delete this line
}

template <typename T>
void FlatTree<T>::fromBinaryTree(const BinaryTree<T>& tree) {
    // Your code here

}

template <typename T>
BinaryTree<T> FlatTree<T>::toBinaryTree() const {
    // Your code here
    return BinaryTree<T>();
}

