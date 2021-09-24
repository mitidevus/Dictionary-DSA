#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Word {
    string key;
    string meaning;
};

struct Node {
    Word value;
    int height;
    Node* left;
    Node* right;
};

int height(Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* createNode(Word s) {
    Node* p = new Node;
    p->value.key = s.key;
    p->value.meaning = s.meaning;
    p->height = 1;
    p->left = NULL;
    p->right = NULL;
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

Node* rightRotate(Node* N) {
    Node* x = N->left;
    Node* T2 = x->right;

    x->right = N;
    N->left = T2;

    N->height = max(height(N->left), height(N->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* N) {
    Node* y = N->right;
    Node* T2 = y->left;

    y->left = N;
    N->right = T2;

    N->height = max(height(N->left), height(N->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insertNode(Node* N, Word s) {
    if (N == NULL)
        return createNode(s);

    if (s.key < N->value.key)
        N->left = insertNode(N->left, s);
    else if (s.key > N->value.key)
        N->right = insertNode(N->right, s);
    else
        return N;

    N->height = max(height(N->left), height(N->right)) + 1;

    int balance = getBalance(N);

    // LL & RR
    if (balance > 1 && s.key < N->value.key)
        return rightRotate(N);
    if (balance < -1 && s.key > N->value.key)
        return leftRotate(N);

    // LR & RL
    if (balance > 1 && s.key > N->value.key) {
        N->left = leftRotate(N->left);
        return rightRotate(N);
    }
    if (balance < -1 && s.key < N->value.key) {
        N->right = rightRotate(N->right);
        return leftRotate(N);
    }

    return N;
}

Node* findNode(Node* N, string key) {
    if (N) {
        if (N->value.key == key)
            return N;
        if (key < N->value.key)
            return findNode(N->left, key);
        return findNode(N->right, key);
    }
    return NULL;
}

void searchWord(Node* N, string key) {
    if (!isEmpty(N)) {
        Node* p = findNode(N, key);
        if (p != NULL) {
            cout << "\nMeaning: " << p->value.meaning << endl;
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
            p->value.meaning = s.meaning;
            cout << "\nEdit successfully !" << endl;
            return;
        }
        cout << "\nThis word doesn't exist !" << endl;
        return;
    }
    cout << "\nTree is empty !" << endl;
}

Node* minValueNode(Node* N) {
    Node* current = N;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* N, string key) {
    if (N == NULL)
        return N;

    if (key < N->value.key) 
        N->left = deleteNode(N->left, key);
    else if (key > N->value.key)
        N->right = deleteNode(N->right, key);
    else {
        if ((N->left == NULL) || (N->right == NULL)) {
            Node* p = N->left ? N->left : N->right;
            if (p == NULL) {
                p = N;
                N = NULL;
            }
            else
                N->value = p->value;
            delete p;
        }
        else {
            Node* p = minValueNode(N->right);
            N->value = p->value;
            N->right = deleteNode(N->right, p->value.key);
        }
    }

    if (N == NULL)
        return N;

    N->height = 1 + max(height(N->left), height(N->right));

    int balance = getBalance(N);

    if (balance > 1 && getBalance(N->left) >= 0)
        return rightRotate(N);

    if (balance > 1 && getBalance(N->left) < 0) {
        N->left = leftRotate(N->left);
        return rightRotate(N);
    }

    if (balance < -1 &&
        getBalance(N->right) <= 0)
        return leftRotate(N);

    if (balance < -1 && getBalance(N->right) > 0) {
        N->right = rightRotate(N->right);
        return leftRotate(N);
    }

    return N;
}

void deleteWord(Node* N, string key) {
    if (!isEmpty(N)) {
        Node* p = findNode(N, key);
        if (p != NULL) {
            N = deleteNode(N, key);
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
            s[i-1].meaning.insert(s[i-1].meaning.size(), "\n");
            s[i-1].meaning.insert(s[i-1].meaning.size(), curWord);
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
        f << N->value.key << "  " << N->value.meaning << "\n" << endl;
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
            deleteWord(N, s.key);
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
    return 0;
}
