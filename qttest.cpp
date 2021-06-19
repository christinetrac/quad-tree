#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

class Point {
public:
    double x; //longitude
    double y; //latitude

    Point(double x_coord, double y_coord){
        x = x_coord;
        y = y_coord;
    }

    Point(){
        x = 0;
        y = 0;
    }
};

class Node {
public:
    Point point;
    string name; //name of city
    int p; //population
    int r; //cost of living
    int s; //avg net salary

    Node *nw;
    Node *ne;
    Node *sw;
    Node *se;

    Node(Point po, string city, int population, int cost, int salary){
        point = po;
        name = city;
        p = population;
        r = cost;
        s = salary;
        nw = nullptr;
        ne = nullptr;
        sw = nullptr;
        se = nullptr;
    }
};

class QuadTree {
public:
    int size = 0;
    Node *root = nullptr; //root of the tree

    QuadTree();
    void insert(string city, double x, double y, int population, int cost, int salary);
    void recursive_insert(Node *temp, Node *parent, Node *child);

    void search(double x, double y);
    void recursive_search(double x, double y, Node *temp);

    void q_max(double x, double y, string d, string attr);
    Node *find_q_node(double x, double y);
    int recursive_q_max(string d, string attr, Node *node, int &max_value);

    void q_min(double x, double y, string d, string attr);
    int recursive_q_min(string d, string attr, Node *node, int &min_value);

    void q_total(double x, double y, string d, string attr);
    int recursive_q_total(string d, string attr, Node *node, int &total);

    void print();
    void recursive_print(Node *node);

    void clear();
    void recursive_clear(Node *node);

    void print_size();
};

QuadTree::QuadTree(){
}

void QuadTree::insert(string city, double x, double y, int population, int cost, int salary){
    Point *point = new Point(x, y);
    Node *child = new Node(*point, city, population, cost, salary);
    if(root == nullptr){
        root = child;
        size++;
        cout << "success" << endl;
    } else {
        recursive_insert(root, nullptr, child);
    }
}

void QuadTree::recursive_insert(Node *temp, Node *parent, Node *child) {
    if(temp != nullptr){
        if((child->point.x >= temp->point.x) && (child->point.y > temp->point.y)){
            recursive_insert(temp->ne, temp, child);
        } else if((child->point.x < temp->point.x) && (child->point.y >= temp->point.y)){
            recursive_insert(temp->nw, temp, child);
        } else if((child->point.x <= temp->point.x) && (child->point.y < temp->point.y)){
            recursive_insert(temp->sw, temp, child);
        } else if((child->point.x > temp->point.x) && (child->point.y <= temp->point.y)){
            recursive_insert(temp->se, temp, child);
        } else if((child->point.x == temp->point.x) && (child->point.y == temp->point.y)){
            cout << "failure" << endl;
            return; //duplicate coordinates
        }
    } else {
        if((child->point.x >= parent->point.x) && (child->point.y > parent->point.y)){
            parent->ne = child;
        } else if((child->point.x < parent->point.x) && (child->point.y >= parent->point.y)){
            parent->nw = child;
        } else if((child->point.x <= parent->point.x) && (child->point.y < parent->point.y)){
            parent->sw = child;
        } else if((child->point.x > parent->point.x) && (child->point.y <= parent->point.y)){
            parent->se = child;
        }
        size++;
        cout << "success" << endl;
    }
}

void QuadTree::recursive_search(double x, double y, Node *temp) {
    if(temp != nullptr){
        if((x >= temp->point.x) && (y > temp->point.y)){
            recursive_search(x, y, temp->ne);
        } else if((x < temp->point.x) && (y >= temp->point.y)){
            recursive_search(x, y, temp->nw);
        } else if((x <= temp->point.x) && (y < temp->point.y)){
            recursive_search(x, y, temp->sw);
        } else if((x > temp->point.x) && (y <= temp->point.y)){
            recursive_search(x, y, temp->se);
        } else if((x == temp->point.x) && (y == temp->point.y)){
            cout << "found " << temp->name << endl;
        }
    } else {
        cout << "not found" << endl;
    }
}

void QuadTree::search(double x, double y){
    if(root == nullptr){
        cout << "not found" << endl;
    } else {
        recursive_search(x, y, root);
    }
}

//find the node with x,y coordinates
//start traversing the tree beginning at that node as the "root" only in the direction of d
//find the max value for attr in the all the children node of the found node in every direction
void QuadTree::q_max(double x, double y, string d, string attr){
    int max_value = 0;
    if(root == nullptr){
        cout << "failure" << endl;
    } else {
        Node *node = find_q_node(x, y);
        if(node != nullptr){
            if(d == "NE"){
                if(node->ne == nullptr){
                    cout << "failure" << endl;
                } else {
                    max_value = recursive_q_max(d, attr, node->ne, max_value);
                    cout << "max " << max_value << endl;
                }
            } else if(d == "NW"){
                if(node->nw == nullptr){
                    cout << "failure" << endl;
                } else {
                    max_value = recursive_q_max(d, attr, node->nw, max_value);
                    cout << "max " << max_value << endl;
                }
            } else if(d == "SE"){
                if(node->se == nullptr){
                    cout << "failure" << endl;
                } else {
                    max_value = recursive_q_max(d, attr, node->se, max_value);
                    cout << "max " << max_value << endl;
                }
            } else {
                if(node->sw == nullptr){
                    cout << "failure" << endl;
                } else {
                    max_value = recursive_q_max(d, attr, node->sw, max_value);
                    cout << "max " << max_value << endl;
                }
            }
        }
    }
}

Node *QuadTree::find_q_node(double x, double y){
    bool success = false;
    if(root == nullptr){
        return nullptr;
    } else {
        Node *temp = root;
        while(temp != nullptr && !success){
            if((x >= temp->point.x) && (y > temp->point.y)){
                temp = temp->ne;
            } else if((x < temp->point.x) && (y >= temp->point.y)){
                temp = temp->nw;
            } else if((x <= temp->point.x) && (y < temp->point.y)){
                temp = temp->sw;
            } else if((x > temp->point.x) && (y <= temp->point.y)){
                temp = temp->se;
            } else if((x == temp->point.x) && (y == temp->point.y)){
                success = true; //coordinate has been found
            }
        }
        if(success){
            return temp;
        } else {
            return nullptr;
        }
    }
}

//return max attribute
int QuadTree::recursive_q_max(string d, string attr, Node *node, int &max_value) {
    if(node != nullptr){
        if(node->ne != nullptr){
            recursive_q_max(d, attr, node->ne, max_value);
        }
        if(node->nw != nullptr){
            recursive_q_max(d, attr, node->nw, max_value);
        }
        if(node->se != nullptr){
            recursive_q_max(d, attr, node->se, max_value);
        }
        if(node->sw != nullptr){
            recursive_q_max(d, attr, node->sw, max_value);
        }
        if(attr == "p"){
            max_value = std::max(max_value, node->p);
        } else if (attr == "r"){
            max_value = std::max(max_value, node->r);
        } else {
            max_value = std::max(max_value, node->s);
        }
    }
    return max_value;
}

void QuadTree::q_min(double x, double y, string d, string attr){
    int min_value = 2147483647; //biggest int value possible...
    if(root == nullptr){
        cout << "failure" << endl;
    } else {
        Node *node = find_q_node(x, y);
        if(node != nullptr){
            if(d == "NE"){
                if(node->ne == nullptr){
                    cout << "failure" << endl;
                } else {
                    min_value = recursive_q_min(d, attr, node->ne, min_value);
                    cout << "min " << min_value << endl;
                }
            } else if(d == "NW"){
                if(node->nw == nullptr){
                    cout << "failure" << endl;
                } else {
                    min_value = recursive_q_min(d, attr, node->nw, min_value);
                    cout << "min " << min_value << endl;
                }
            } else if(d == "SE"){
                if(node->se == nullptr){
                    cout << "failure" << endl;
                } else {
                    min_value = recursive_q_min(d, attr, node->se, min_value);
                    cout << "min " << min_value << endl;
                }
            } else {
                if(node->sw == nullptr){
                    cout << "failure" << endl;
                } else {
                    min_value = recursive_q_min(d, attr, node->sw, min_value);
                    cout << "min " << min_value << endl;
                }
            }
        }
    }
}

int QuadTree::recursive_q_min(string d, string attr, Node *node, int &min_value) {
    if(node != nullptr){
        if(node->ne != nullptr){
            recursive_q_min(d, attr, node->ne, min_value);
        }
        if(node->nw != nullptr){
            recursive_q_min(d, attr, node->nw, min_value);
        }
        if(node->se != nullptr){
            recursive_q_min(d, attr, node->se, min_value);
        }
        if(node->sw != nullptr){
            recursive_q_min(d, attr, node->sw, min_value);
        }
        if(attr == "p"){
            min_value = std::min(min_value, node->p);
        } else if (attr == "r"){
            min_value = std::min(min_value, node->r);
        } else {
            min_value = std::min(min_value, node->s);
        }
    }
    return min_value;
}

void QuadTree::q_total(double x, double y, string d, string attr){
    int total = 0;
    if(root == nullptr){
        cout << "failure" << endl;
    } else {
        Node *node = find_q_node(x, y);
        if(node != nullptr){
            if(d == "NE"){
                if(node->ne == nullptr){
                    cout << "failure" << endl;
                } else {
                    total = recursive_q_total(d, attr, node->ne, total);
                    cout << "total " << total << endl;
                }
            } else if(d == "NW"){
                if(node->nw == nullptr){
                    cout << "failure" << endl;
                } else {
                    total = recursive_q_total(d, attr, node->nw, total);
                    cout << "total " << total << endl;
                }
            } else if(d == "SE"){
                if(node->se == nullptr){
                    cout << "failure" << endl;
                } else {
                    total = recursive_q_total(d, attr, node->se, total);
                    cout << "total " << total << endl;
                }
            } else {
                if(node->sw == nullptr){
                    cout << "failure" << endl;
                } else {
                    total = recursive_q_total(d, attr, node->sw, total);
                    cout << "total " << total << endl;
                }
            }
        }
    }
}

int QuadTree::recursive_q_total(string d, string attr, Node *node, int &total) {
    if(node != nullptr){
        if(node->ne != nullptr){
            recursive_q_total(d, attr, node->ne, total);
        }
        if(node->nw != nullptr){
            recursive_q_total(d, attr, node->nw, total);
        }
        if(node->se != nullptr){
            recursive_q_total(d, attr, node->se, total);
        }
        if(node->sw != nullptr){
            recursive_q_total(d, attr, node->sw, total);
        }
        if(attr == "p"){
            total += node->p;
        } else if (attr == "r"){
            total += node->r;
        } else {
            total += node->s;
        }
    }
    return total;
}

void QuadTree::print() {
    recursive_print(root);
    if(root != nullptr){
        cout << endl;
    }
}

void QuadTree::recursive_print(Node *node) {
    if(node != nullptr){
        recursive_print(node->ne);
        recursive_print(node->nw);
        cout << node->name << " ";
        recursive_print(node->sw);
        recursive_print(node->se);
    }
}

void QuadTree::clear(){
    recursive_clear(root);
    cout << "success" << endl;
    root = nullptr;
    size = 0;
}

void QuadTree::recursive_clear(Node *node) {
    if(node != nullptr){
        recursive_clear(node->ne);
        recursive_clear(node->nw);
        recursive_clear(node->se);
        recursive_clear(node->sw);
        delete node;
    } else {
        return;
    }
}

void QuadTree::print_size() {
    cout << "tree size " << size << endl;
}

int main() {
    string input;
    QuadTree test;
    while(getline(cin, input)){
        if(input.substr(0, 1) == "i"){
            string str = input.substr(2);
            vector<string> result;
            stringstream ss(str);
            while(ss.good()){
                string substr;
                getline(ss, substr, ';');
                result.push_back(substr);
            }
            test.insert(result[0], stod(result[1]), stod(result[2]), stoi(result[3]), stoi(result[4]), stoi(result[5]));
        } else if (input.substr(0) == "size") {
            test.print_size();
        } else if (input.substr(0, 1) == "s") {
            string str = input.substr(2);
            vector<string> result;
            stringstream ss(str);
            while(ss.good()){
                string substr;
                getline(ss, substr, ';');
                result.push_back(substr);
            }
            test.search(stod(result[0]), stod(result[1]));
        } else if (input.substr(0, 5) == "q_max") {
            string str = input.substr(6);
            vector<string> result;
            stringstream ss(str);
            while(ss.good()){
                string substr;
                getline(ss, substr, ';');
                result.push_back(substr);
            }
            test.q_max(stod(result[0]), stod(result[1]), result[2], result[3]);
        } else if (input.substr(0, 5) == "q_min") {
            string str = input.substr(6);
            vector<string> result;
            stringstream ss(str);
            while(ss.good()){
                string substr;
                getline(ss, substr, ';');
                result.push_back(substr);
            }
            test.q_min(stod(result[0]), stod(result[1]), result[2], result[3]);
        } else if (input.substr(0, 7) == "q_total") {
            string str = input.substr(8);
            vector<string> result;
            stringstream ss(str);
            while(ss.good()){
                string substr;
                getline(ss, substr, ';');
                result.push_back(substr);
            }
            test.q_total(stod(result[0]), stod(result[1]), result[2], result[3]);
        } else if (input.substr(0) == "print") {
            test.print();
        } else if (input.substr(0) == "clear") {
            test.clear();
        }
    }
    return 0;
}
