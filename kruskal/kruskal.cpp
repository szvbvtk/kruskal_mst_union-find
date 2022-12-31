#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum mode { BOTTOM_UP, TOP_DOWN };

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

    void printGraph() {
        cout << "Nodes: \n";
        for (int i = 0; i < number_of_nodes; i++)
            cout << nodes->get(i).x << ' ' << nodes->get(i).y << '\n';

        cout << "Edges: \n";
        for (int i = 0; i < number_of_edges; i++)
            cout << edges->get(i).index1 << ' ' << edges->get(i).index2 << ' ' << edges->get(i).weight << '\n';
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
        while (set[index] > 0) {
            index = set[index];
        }

        return index;
    }

    int find_compress(int index) {

        int index_compress = index;

        while (set[index] > 0) {
            index = set[index];
        }

        while (set[index_compress] != set[index]) {
            int tmp_index = set[index_compress];
            set[index_compress] = index;
            index_compress = tmp_index;
        }

        return index;
    }

    void unite(int index1, int index2) {
        //if (set[index1] > set[index2]) {
        //    set[index1] += set[index2];
        //    set[index2] = index1;
        //}
        //else {
        //    set[index2] += set[index1];
        //    set[index1] = index2;
        //}

        // tak lub tak może być

        if (index1 < index2)
            swap(index1, index2);

        set[index1] += set[index2];
        set[index2] = index1;
    }
};

enum {FIND, FIND_AND_COMPRESS};

DynamicArray<Edge> kruskal(Graph g, bool find_mode) {
    // dodać sortowanie krawędzi
    Edge* edges = g.edges->array;
    BinaryHeap<Edge>(edges, g.number_of_edges, edge_cmp, 0);

    DynamicArray<Edge>* MST = new DynamicArray<Edge>();

    Union union1{ g.number_of_nodes };

    for (int i = 0; i < g.number_of_edges; i++) {
        Edge tmp = edges[i];
        int node1_root;
        int node2_root;

        if (find_mode == FIND) {
            node1_root = union1.find(tmp.index1);
            node2_root = union1.find(tmp.index2);
        }
        else if (find_mode == FIND_AND_COMPRESS) {
            node1_root = union1.find_compress(tmp.index1);
            node2_root = union1.find_compress(tmp.index2);
        }


        if (node1_root != node2_root) {
            MST->add(tmp);
            union1.unite(node1_root, node2_root);
            //cout << node1_root << ' ' << node2_root << '\n';
            cout << tmp.index1 << ' ' << tmp.index2 << '\n';
        }
    }

    //delete[] edges;
    //delete bh;

    return *MST;
}

Graph graphFromFile(string fileName) {
    ifstream plik("../excercises/g1.txt");
    int number_of_nodes;
    int number_of_edges;
    double tmp_x;
    double tmp_y;
    int tmp_index1;
    int tmp_index2;
    double tmp_weight;

    Graph g;

    plik >> number_of_nodes;

    for (int i = 0; i < number_of_nodes; i++) {
        plik >> tmp_x >> tmp_y;

        g.addNode(tmp_x, tmp_y);
    }

    plik >> number_of_edges;

    for (int i = 0; i < number_of_edges; i++) {
        plik >> tmp_index1 >> tmp_index2 >> tmp_weight;

        g.addEdge(tmp_index1, tmp_index2, tmp_weight);
    }


    plik.close();

    return g;
}

int main()
{
    // nie sprawdzałem czy cokolwiek działa
    // materiały gdybym potrzebował pomocy
    //  https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-algorithm-greedy-algo-2/
    // książka
    // algolearn
    // Kurs Udemy

    Graph graph = graphFromFile("../excercises/g1.txt");
    //graph.printGraph();

    DynamicArray<Edge> wynik = kruskal(graph, FIND_AND_COMPRESS);

    //for (int i = 0; i < wynik.size; i++) {
    //    cout << wynik.array[i].index1 << ' ' << wynik.array[i].index2 << '\n';
    //}

}


