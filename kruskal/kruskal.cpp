#include <iostream>

using namespace std;

template <typename T>
struct BinaryHeap {
    int size;
    T* heap_array;
    BinaryHeap(T* array, int n, int (*func)(T, T), bool mode = 0) {
        heap_array = array;

        if (mode == TOP_DOWN) {
            size = 0;
            for (int i = 0; i < n; i++) {
                size++;
                heapUp(size, func);
            }
        }
        else if (mode == BOTTOM_UP) {
            size = n;
            for (int i = size / 2; i > -1; i--) {
                heapDown(i, func);
            }
        }

        sort(size, func);

    }

    void sort(int& size, int (*func)(T, T)) {
        for (int i = size - 1; i > 0; i--) {
            swap(heap_array[i], heap_array[0]);
            size--;
            heapDown(0, func);
        }
    }

    T get(int index) {

        if (index < size) {
            return heap_array[index];
        }
    }

    int parent(int i) {
        i++;
        return (i >> 1) - 1;
    }
    int left_child(int i) {
        i++;
        return (i << 1) - 1;
    }
    int right_child(int i) {
        i++;
        return (i << 1);
    }

    void heapUp(int el_index, int (*func)(T, T)) {
        if (func(heap_array[el_index], heap_array[parent(el_index)]) == 1 && el_index > 0) {
            T tmp = heap_array[parent(el_index)];
            heap_array[parent(el_index)] = heap_array[el_index];
            heap_array[el_index] = tmp;
            heapUp(parent(el_index), func);
        }
    }

    string str(string(*func)(T), int k = -1) {
        return heap_array->str(func, k);
    }

    void heapDown(int el_index, int (*func)(T, T)) {

        int l_child_index = left_child(el_index);
        int r_child_index = right_child(el_index);

        int largest;
        if (l_child_index < size && func(heap_array[l_child_index], heap_array[el_index]) == 1)
            largest = l_child_index;
        else
            largest = el_index;

        if (r_child_index < size && func(heap_array[r_child_index], heap_array[largest]) == 1)
            largest = r_child_index;

        if (el_index != largest) {
            swap(heap_array[el_index], heap_array[largest]);
            heapDown(largest, func);
        }
    }
};


template <typename T>
struct DynamicArray {

    int size{};
    int capacity{ 1 };
    T* array = new T[capacity];

    void add(T data) {
        if (size == capacity) {

            capacity *= 2;

            T* tmp_array = new T[capacity];

            for (int i = 0; i < size; i++)
                tmp_array[i] = array[i];

            delete[] array;
            array = tmp_array;

        }

        array[size] = data;
        size++;

    }

    T get(int index) {

        if (index < size) {
            return array[index];
        }
        else {
            throw out_of_range("Błędny zakres");
        }

    }

    void change(int index, T newData) {

        if (index < size) {
            array[index] = newData;
        }
        else {
            throw out_of_range("Błędny zakres");
        }

    }

    void clear() {
        delete[] array;
        size = 0;
        capacity = 1;
        array = new T[capacity];
    }

    string str(string(*func)(T so)) {

        string s = "Actual size : " + to_string(size) + "\nCapacity : " + to_string(capacity) + "\n";

        for (int i = 0; i < size; i++) {
            s += func(array[i]);
        }

        s += '\n';

        return s;

    }

    void sort(int (*func)(T*, T*)) {
        for (int i = 0; i < size - 1; i++)
            for (int j = 0; j < size - i - 1; j++) {
                if (func(&array[j], &array[j + 1]) == 1) {
                    T tmp = array[j + 1];
                    array[j + 1] = array[j];
                    array[j] = tmp;
                }
            }
    }

};

struct Node {
    double x;
    double y;
};

struct Edge {
    int index1;
    int index2;
    double weight;
};

int edge_cmp(Edge e1, Edge e2) {
    if (e1.weight == e2.weight)
        return 0;
    else if (e1.weight < e2.weight)
        return -1;
    else if (e1.weight > e2.weight)
        return 1;
}

struct Graph {
    DynamicArray<Node>* nodes = new DynamicArray<Node>();
    DynamicArray<Edge>* edges = new DynamicArray<Edge>();
    int number_of_nodes = 0;
    int number_of_edges = 0;

    void addNode(double x, double y) {
        Node tmp{ x, y };
        nodes->add(tmp);
        number_of_nodes++;
    }

    void addEdge(int index1, int index2, double weight) {
        Edge tmp{ index1, index2, weight };
        edges->add(tmp);
        number_of_edges++;
    }
};

struct Union {
    int* set = nullptr;
    int* ranks = nullptr;

    Union(int size) {
        set = new int[size] {-1};
        ranks = new int[size] {1};

    }

    ~Union() {
        delete[] set;
        delete[] ranks;
    }

    int find(int index) {
        while (set[index] <= -1) {
            index = set[index];
        }

        return index;
    }

    void unite(int index1, int index2) {
        // zamiast else mozna swap index1 i index2 a potem tylko te dwie linijki a w if else zamienic swapami jedynie
        if (set[index1] < set[index2]) {
            set[index1] += set[index2];
            set[index2] = index1;
        }
        else {
            set[index2] += set[index1];
            set[index1] = index2;
        }
    }
};

DynamicArray<Edge>* kruskal(Graph g) {
    // dodać sortowanie krawędzi
    Edge* edges = new Edge[g.number_of_edges];
    BinaryHeap<Edge>* bh = new BinaryHeap<Edge>(edges, g.number_of_edges, edge_cmp, 0);

    DynamicArray<Edge>* MST = new DynamicArray<Edge>();

    Union union1{ g.number_of_nodes };

    for (int i = 0; i < g.number_of_edges; i++) {
        Edge tmp = edges[i];

        int node1_root = union1.find(tmp.index1);
        int node2_root = union1.find(tmp.index2);

        if (node1_root != node2_root) {
            MST->add(tmp);
            union1.unite(node1_root, node2_root);
        }
    }

    delete[] edges;
    delete[] bh;

    return MST;
}

int main()
{
    // nie sprawdzałem czy cokolwiek działa
}


