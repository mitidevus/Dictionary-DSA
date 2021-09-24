#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Word {
    string key;
    string meaning;
};

struct Node {
    Word data;
    int color;
    Node* left, * right, * parent;
};

Node* createNode(Word s) {
    Node* p = new Node;
    p->data = s;
    p->left = NULL;
    p->right = NULL;
    p->parent = NULL;
    p->color = 1;
    return p;
}

bool isEmpty(Node* N) {
    if (N == NULL) return true;
    return false;
}

void freeTree(Node*& N) {
    if (N) {
        freeTree(N->left);
        freeTree(N->right);
        delete N;
    }
}

void rotateLeft(Node*& root, Node*& pt) {
    Node* pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

void rotateRight(Node*& root, Node*& pt) {
    Node* pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

Node* BSTInsert(Node* N, Node* pt) {
    if (N == NULL)
        return pt;

    if (pt->data.key < N->data.key) {
        N->left = BSTInsert(N->left, pt);
        N->left->parent = N;
    }
    else if (pt->data.key > N->data.key) {
        N->right = BSTInsert(N->right, pt);
        N->right->parent = N;
    }

    return N;
}

void fixInsert(Node*& root, Node*& pt) {
    Node* parent_pt = NULL;
    Node* grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != 0) && (pt->parent->color == 1)) {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        if (parent_pt == grand_parent_pt->left) {
            Node* uncle_pt = grand_parent_pt->right;

            if (uncle_pt != NULL && uncle_pt->color == 1) {
                grand_parent_pt->color = 1;
                parent_pt->color = 0;
                uncle_pt->color = 0;
                pt = grand_parent_pt;
            }
            else {
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
        else {
            Node* uncle_pt = grand_parent_pt->left;

            if ((uncle_pt != NULL) && (uncle_pt->color == 1)) {
                grand_parent_pt->color = 1;
                parent_pt->color = 0;
                uncle_pt->color = 0;
                pt = grand_parent_pt;
            }
            else {
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
    root->color = 0;
}

Node* insertNode(Node* N, Word s) {
    Node* pt = createNode(s);
    N = BSTInsert(N, pt);
    fixInsert(N, pt);
    return N;
}

Node* findNode(Node* N, string key) {
    if (N) {
        if (N->data.key == key)
            return N;
        if (key < N->data.key)
            return findNode(N->left, key);
        return findNode(N->right, key);
    }
    return NULL;
}

void searchWord(Node* N, string key) {
    if (!isEmpty(N)) {
        Node* p = findNode(N, key);
        if (p != NULL) {
            cout << "\nMeaning: " << p->data.meaning << endl;
            return;
        }
        cout << "\nThis word doesn't exist !" << endl;
        return;
    }
    cout << "\nTree is empty !" << endl;
}

void addWord(Node* N, Word s) {
    if (!findNode(N, s.key)) {
        N = insertNode(N, s);
        cout << "\nAdd successfully !" << endl;
        return;
    }
    cout << "\nThis word already exists !" << endl;
}

void editWord(Node* N, Word s) {
    if (!isEmpty(N)) {
        Node* p = findNode(N, s.key);
        if (p != NULL) {
            p->data.meaning = s.meaning;
            cout << "\nEdit successfully !" << endl;
            return;
        }
        cout << "\nThis word doesn't exist !" << endl;
        return;
    }
    cout << "\nTree is empty !" << endl;
}

Node* maxValueNode(Node*& node) {
    Node* ptr = node;

    while (ptr->right != nullptr)
        ptr = ptr->right;

    return ptr;
}

Node* minValueNode(Node*& node) {
    Node* ptr = node;

    while (ptr->left != nullptr)
        ptr = ptr->left;

    return ptr;
}

Node* getNodeToDelete(Node* N, Word s) {
    if (N == NULL)
        return N;

    if (s.key < N->data.key)
        return getNodeToDelete(N->left, s);

    if (s.key > N->data.key)
        return getNodeToDelete(N->right, s);

    if (N->left == NULL || N->right == NULL)
        return N;

    Node* temp = maxValueNode(N->left);
    N->data = temp->data;
    return getNodeToDelete(N->left, temp->data);
}

void fixDelete(Node*& root, Node*& pt) {
    if (pt == NULL)
        return;

    if (pt == root) {
        root = NULL;
        return;
    }

    if (pt->color == 1 || pt->left->color == 1 || pt->right->color == 1) {
        Node* child = pt->left != NULL ? pt->left : pt->right;
         
        if (pt == pt->parent->left) {
            pt->parent->left = child;
            if (child != NULL) {
                child->parent = pt->parent;
                child->color = 0;
            }
            delete pt;
        }
        else {
            pt->parent->right = child;
            if (child != NULL) {
                child->parent = pt->parent;
                child->color = 0;
            }
            delete pt;
        }
    }
    else {
        Node* sibling = NULL;
        Node* parent = NULL;
        Node* ptr = pt;
        ptr->color = 2;
        while (ptr != root && ptr->color == 2) {
            parent = ptr->parent;
            if (ptr == parent->left) {
                sibling = parent->right;
                if (sibling->color == 1) {
                    sibling->color = 0;
                    parent->color = 1;
                    rotateLeft(root, parent);
                }
                else {
                    if (sibling->left->color == 0 && sibling->right->color == 0) {
                        sibling->color = 1;
                        if (parent->color == 1)
                            parent->color = 0;
                        else
                            parent->color = 2;
                        ptr = parent;
                    }
                    else {
                        if (sibling->right->color == 0) {
                            sibling->left->color = 0;
                            sibling->color = 1;
                            rotateRight(root, sibling);
                            sibling = parent->right;
                        }
                        sibling->color = parent->color;
                        parent->color = 0;
                        sibling->right->color = 0;
                        rotateLeft(root, parent);
                        break;
                    }
                }
            }
            else {
                sibling = parent->left;
                if (sibling->color == 1) {
                    sibling->color = 0;
                    parent->color = 1;
                    rotateRight(root, parent);
                }
                else {
                    if (sibling->left->color == 0 && sibling->right->color == 0) {
                        sibling->color = 1;
                        if (parent->color == 1)
                            parent->color = 0;
                        else
                            parent->color = 2;
                        ptr = parent;
                    }
                    else {
                        if (sibling->left->color == 0) {
                            sibling->right->color = 0;
                            sibling->color = 1;
                            rotateLeft(root, sibling);
                            sibling = parent->left;
                        }
                        sibling->color = parent->color;
                        parent->color = 0;
                        sibling->left->color = 0;
                        rotateRight(root, parent);
                        break;
                    }
                }
            }
        }
        if (pt == pt->parent->left)
            pt->parent->left = NULL;
        else
            pt->parent->right = NULL;
        delete pt;
        root->color = 0;
    }
}

Node* deleteNode(Node*& N, Word s) {
    Node* pt = getNodeToDelete(N, s);
    fixDelete(N, pt);
    return N;
}

void deleteWord(Node* N, Word s) {
    if (!isEmpty(N)) {
        Node* p = findNode(N, s.key);
        if (p != NULL) {
            N = deleteNode(N, s);
            cout << "\nDelete successfully !" << endl;
            return;
        }
        cout << "\nThis word doesn't exist !" << endl;
        return;
    }
    cout << "\nTree is empty !" << endl;
}

int countLine(string filename) {
    fstream f(filename, ios::in);
    if (f.fail()) {
        cout << "File does not exist !" << endl;
        return 0;
    }

    int count = 0;
    string preWord = " ";
    while (!f.eof()) {
        string curWord;
        do {
            getline(f, curWord);
            if (f.eof()) break;
        } while (curWord.size() < 3 || curWord == preWord);

        if (f.eof() && curWord == "") break;

        // Separate key and meaning
        int start = 0, end = curWord.find(' ');
        while (curWord[(double)end + 1] != ' ' && end < curWord.size()) {
            start = end + 1;
            end = curWord.find(' ', start);
        }
        string key = curWord.substr(0, end);
        string meaning = curWord.substr((double)end + 2, curWord.size() - ((double)end + 2));

        // Check if key is "Usage" or not
        if ((key != "Usage") || (key == "Usage") && (meaning == "n. 1 use, treatment (damaged by rough usage). 2 customary practice, esp. In the use of a language or as creating a precedent in law.")) {
            count++;
        }
        preWord = curWord;
    }
    return count;
}

void insertionSort(Word* s, int n) {
    for (int i = 1; i < n; i++) {
        Word x = s[i];
        int j = i - 1;
        while (j >= 0 && s[j].key > x.key) {
            s[j + 1] = s[j];
            j--;
        }
        s[j + 1] = x;
    }
}

void deleteDuplicate(Word*& s, int& n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (s[i].key == s[j].key) {
                for (int pos = j + 1; pos < n; pos++) {
                    s[pos - 1] = s[pos];
                }
                n--;
                j--;
            }
            else {
                break;
            }
        }
    }
    // Paste to new array, without wasting memory
    Word* temp = new Word[n];
    for (int i = 0; i < n; i++) {
        temp[i] = s[i];
    }
    delete[] s;
    s = temp;
}

void loadFromFile(Node*& N, string filename) {
    clock_t start = clock();

    fstream f(filename, ios::in);
    if (f.fail()) {
        cout << "File does not exist !" << endl;
        return;
    }

    int n = countLine(filename);
    Word* s = new Word[n];

    int i = 0;
    string preKey = " ", preWord = " ";
    for (int j = 0; j < n; j++) {
        string curWord;
        do {
            getline(f, curWord);
        } while (curWord.size() < 3 || curWord == preWord);

        // Separate key and meaning
        int start = 0, end = curWord.find(' ');
        while (curWord[(double)end + 1] != ' ' && end < curWord.size()) {
            start = end + 1;
            end = curWord.find(' ', start);
        }
        string key = curWord.substr(0, end);
        string meaning = curWord.substr((double)end + 2, curWord.size() - ((double)end + 2));

        // Check if key is "Usage" or not
        if ((key != "Usage") || (key == "Usage" && meaning == "n. 1 use, treatment (damaged by rough usage). 2 customary practice, esp. In the use of a language or as creating a precedent in law.")) {
            s[i].key = key;
            s[i].meaning = meaning;
            i++;
        }
        else {
            // Insert "Usage ..." into meaning of the pre-word
            s[i - 1].meaning.insert(s[i - 1].meaning.size(), "\n");
            s[i - 1].meaning.insert(s[i - 1].meaning.size(), curWord);
            j--;
        }
        preKey = key;
        preWord = curWord;
    }

    // Sort and delete duplicate
    insertionSort(s, n);
    deleteDuplicate(s, n);

    for (int i = 0; i < n; i++) {
        N = insertNode(N, s[i]);
    }

    clock_t end = clock();
    cout << "Loading time: " << end - start << " (ms)\n" << endl;
    delete[] s;
    f.close();
}

void printTree(Node* N, fstream& f) {
    if (N) {
        printTree(N->left, f);
        f << N->data.key << "  " << N->data.meaning << "\n" << endl;
        printTree(N->right, f);
    }
}

void saveToFile(Node* N, string filename) {
    clock_t start = clock();
    fstream f(filename, ios::out);

    printTree(N, f);

    clock_t end = clock();
    cout << "\nSaving time: " << end - start << " (ms)" << endl;
    f.close();
}

void menu(Node* N) {
    while (true) {
        cout << "* * * * * * * * * * * * * * * * * * *" << endl;
        cout << "* 1. Search meaning of a word.      *" << endl;
        cout << "* 2. Add new word to dictionary.    *" << endl;
        cout << "* 3. Edit meaning of a word.        *" << endl;
        cout << "* 4. Delete a word.                 *" << endl;
        cout << "* 0. Exit.                          *" << endl;
        cout << "* * * * * * * * * * * * * * * * * * *" << endl;

        int selection;
        do {
            cout << "\nYour choice: ";
            cin >> selection;
            cin.ignore();
            if (selection != 0 && selection != 1 && selection != 2 && selection != 3 && selection != 4) {
                cout << "Try Again !" << endl;
            }
        } while (selection != 0 && selection != 1 && selection != 2 && selection != 3 && selection != 4);

        clock_t start, end;
        Word s;
        if (selection == 1) {
            cout << "Enter word: ";
            getline(cin, s.key);

            start = clock();
            searchWord(N, s.key);
            end = clock();

            cout << "\nSearching Time: " << end - start << " (ms)" << endl;
            system("pause");
        }
        else if (selection == 2) {
            cout << "Enter word: ";
            getline(cin, s.key);
            cout << "Enter meaning: ";
            getline(cin, s.meaning);

            start = clock();
            addWord(N, s);
            end = clock();

            cout << "\nAdding Time: " << end - start << " (ms)" << endl;
            system("pause");
        }
        else if (selection == 3) {
            cout << "Enter word: ";
            getline(cin, s.key);
            cout << "Enter meaning: ";
            getline(cin, s.meaning);

            start = clock();
            editWord(N, s);
            end = clock();

            cout << "\nEditing Time: " << end - start << " (ms)" << endl;
            system("pause");
        }
        else if (selection == 4) {
            cout << "Enter word: ";
            getline(cin, s.key);

            start = clock();
            deleteWord(N, s);
            end = clock();

            cout << "\nDeleting Time: " << end - start << " (ms)" << endl;
            system("pause");
        }
        else {
            break;
            system("pause");
        }
        system("cls");
    }
}

int main() {
    Node* N = NULL;
    loadFromFile(N, "Oxford English Dictionary.txt");
    menu(N);
    saveToFile(N, "Oxford English Dictionary.txt");
    freeTree(N);
    system("pause");
    return 0;
}