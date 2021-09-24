#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

struct Word {
    string key;
    string meaning;
};

struct Node {
    Word value;
    Node* left;
    Node* right;
};

typedef Node* Tree;

Node* CreateNode(string key, string meaning) {
    Node* p = new Node;
    p->value.key = key;
    p->value.meaning = meaning;
    p->left = NULL;
    p->right = NULL;
    return p;
}

bool isEmpty(Tree root) {
    if (root == NULL) return true;
    return false;
}

void CreateTree(Tree& root) {
    root = NULL;
}

void DestroyTree(Tree& root) {
    if (root) {
        DestroyTree(root->left);
        DestroyTree(root->right);
        delete root;
    }
}

Node* FindNode(Tree root, string key) {
    if (root) {
        if (root->value.key == key)
            return root;
        if (key < root->value.key)
            return FindNode(root->left, key);
        return FindNode(root->right, key);
    }
    return NULL;
}

void searchWord(Tree myTree, string key) {
    if (!isEmpty(myTree)) {
        Node* p = FindNode(myTree, key);
        if (p != NULL) {
            cout << "\nMeaning: " << p->value.meaning << endl;
            return;
        }
        cout << "\nThis word doesn't exist !" << endl;
        return;
    }
    cout << "\nTree is empty !" << endl;
}

void AddNode(Tree& root, Node* node) {
    if (root) {
        if (root->value.key == node->value.key)
            return;
        if (node->value.key < root->value.key)
            AddNode(root->left, node);
        else
            AddNode(root->right, node);
    }
    else {
        root = node;
    }
}

struct node* insert(struct node* node, int data) {
    if (node == NULL)
        return(newNode(data));
    else
    {
        /* 2. Otherwise, recur down the tree */
        if (data <= node->data)
            node->left = insert(node->left, data);
        else
            node->right = insert(node->right, data);

        /* return the (unchanged) node pointer */
        return node;
    }
}

void addWord(Tree& myTree, string key, string meaning) {
    if (!FindNode(myTree, key)) {
        Node* p = CreateNode(key, meaning);
        AddNode(myTree, p);
        cout << "\nAdd successfully !" << endl;
        return;
    }
    cout << "\nThis word already exists !" << endl;
}

void editWord(Tree myTree, string key, string meaning) {
    if (!isEmpty(myTree)) {
        Node* p = FindNode(myTree, key);
        if (p != NULL) {
            p->value.meaning = meaning;
            cout << "\nEdit successfully !" << endl;
            return;
        }
        cout << "\nThis word doesn't exist !" << endl;
        return;
    }
    cout << "\nTree is empty !" << endl;
}

void FindAndReplace(Tree& p, Tree& tree) {
    if (tree->left)
        FindAndReplace(p, tree->left);
    else {
        p->value = tree->value;
        p = tree;
        tree = tree->right;
    }
}

void DeleteNode(Tree& root, string key) {
    if (root) {
        if (key > root->value.key)
            DeleteNode(root->right, key);
        else if (key < root->value.key)
            DeleteNode(root->left, key);
        else {
            Node* p = root;
            if (!root->left)
                root = root->right;
            else if (!root->right)
                root = root->left;
            else
                FindAndReplace(p, root->left);
            delete p;
            cout << "\nDelete successfully !" << endl;
        }
    }
    else {
        cout << "\nThis word does not exist !" << endl;
    }
}        

void deleteWord(Tree& myTree, string key) {
    if (!isEmpty(myTree)) {
        DeleteNode(myTree, key);
        return;
    }
    cout << "\nTree is empty !" << endl;
}

void PrintTree(Tree root, fstream& f) {
    if (root) {
        PrintTree(root->left, f);
        f << root->value.key << "  " << root->value.meaning << "\n" << endl;
        PrintTree(root->right, f);
    }
}

void saveToFile(string filename, Tree myTree) {
    clock_t start = clock();
    fstream f(filename, ios::out);

    PrintTree(myTree, f);

    clock_t end = clock();
    cout << "\nSaving time: " << end - start << " (ms)" << endl;
    f.close();
}

void menu(Tree& myTree) {
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
        string key, meaning;
        if (selection == 1) {
            cout << "Enter word: ";
            getline(cin, key);

            start = clock();
            searchWord(myTree, key);
            end = clock();

            cout << "\nSearching Time: " << end - start << " (ms)" << endl;
            system("pause");
        }
        else if (selection == 2) {
            cout << "Enter word: ";
            getline(cin, key);
            cout << "Enter meaning: ";
            getline(cin, meaning);

            start = clock();
            addWord(myTree, key, meaning);
            end = clock();

            cout << "\nAdding Time: " << end - start << " (ms)" << endl;
            system("pause");
        }
        else if (selection == 3) {
            cout << "Enter word: ";
            getline(cin, key);
            cout << "Enter meaning: ";
            getline(cin, meaning);

            start = clock();
            editWord(myTree, key, meaning);
            end = clock();

            cout << "\nEditing Time: " << end - start << " (ms)" << endl;
            system("pause");
        }
        else if (selection == 4) {
            cout << "Enter word: ";
            getline(cin, key);

            start = clock();
            deleteWord(myTree, key);
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

        if (f.eof()) break;

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

void loadFromFile(string filename, Tree& myTree) {
    clock_t start = clock();
    fstream f(filename, ios::in);
    if (f.fail()) {
        cout << "File does not exist !" << endl;
        return;
    }

    int n = countLine(filename);
    Word* s = new Word[n];

    int i = 0;
    string preWord = " ";
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
            //if (!isExist(s, n, key)) {
            s[i].key = key;
            s[i].meaning = meaning;
            i++;
            //}
        }
        else {
            // Insert "Usage ..." into meaning of the pre-word
            s[i - 1].meaning.insert(s[i - 1].meaning.size(), "\n");
            s[i - 1].meaning.insert(s[i - 1].meaning.size(), curWord);
        }
        preWord = curWord;
    }

    // Sort and delete duplicate
    insertionSort(s, n);
    deleteDuplicate(s, n);

    for (int i = 0; i < n; i++) {
        int j = rand() % n;
        swap(s[i], s[j]);
    }

    for (int i = 0; i < n; i++) {
        AddNode(myTree, CreateNode(s[i].key, s[i].meaning));
    }

    clock_t end = clock();
    cout << "Loading time: " << end - start << " (ms)\n" << endl;
    f.close();
}

int main() {
    srand(time(NULL));
    Tree myTree;
    CreateTree(myTree);
    loadFromFile("Dictionary.txt", myTree);
    menu(myTree);
    saveToFile("Dictionary.txt", myTree);
    DestroyTree(myTree);
    return 0;
}