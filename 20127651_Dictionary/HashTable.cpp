#include <iostream>
#include <ctime>
#include <time.h>
#include <string>
#include <fstream>

using namespace std;

#define tabSize 10000

struct Word {
	string key;
	string meaning;
};

struct HashNode {
	Word value;
	HashNode* pNext;
};

int hashString(string key) {
	long sum = 0, mul = 1;
	for (int i = 0; i < key.length(); i++) {
		mul = (i % 4 == 0) ? 1 : mul * 256;
		sum += key.at(i) * mul;
	}
	return (int)(abs(sum) % tabSize);
}

void initializeHash(HashNode* bucket) {
	for (int i = 0; i < tabSize; i++) {
		bucket[i].pNext = NULL;
	}
}

void memFree(HashNode* bucket) {
	for (int i = 0; i < tabSize; i++) {
		if (bucket[i].pNext != NULL) {
			HashNode* k = bucket[i].pNext;
			HashNode* g = k;
			while (k != NULL) {
				g = k;
				k = k->pNext;
				delete g;
			}
			bucket[i].pNext = NULL;
		}
	}
}

void searchWord(HashNode* bucket, string key) {
	int index = hashString(key);
	if (bucket[index].pNext != NULL) {
		HashNode* p = bucket[index].pNext;
		while (p != NULL) {
			if (p->value.key == key) {
				cout << "\nMeaning: " << p->value.meaning << endl;
				return;
			}
			p = p->pNext;
		}
	}
	cout << "\nThis word doesn't exist !" << endl;
}

bool insertWord(HashNode* bucket, string key, string meaning) {
	// Create NODE
	HashNode* p = new HashNode;
	p->value.key = key;
	p->value.meaning = meaning;
	p->pNext = NULL;

	int index = hashString(key);
	if (bucket[index].pNext == NULL) {
		bucket[index].pNext = p;
		return 1;
	}
	else {
		HashNode* k = bucket[index].pNext;
		HashNode* g = k;
		while (k != NULL) {
			if (k->value.key == key) break;
			g = k;
			k = k->pNext;
		}
		if (k == NULL) {
			g->pNext = p;
			return 1;
		}
		delete p;
		return 0;
	}
}

void addNewWord(HashNode* bucket, string key, string meaning) {
	if (!insertWord(bucket, key, meaning)) {
		cout << "\nThis word already exists !" << endl;
	}
}

void editMeaning(HashNode* bucket, string key, string meaning) {
	int index = hashString(key);
	if (bucket[index].pNext != NULL) {
		HashNode* k = bucket[index].pNext;
		while (k != NULL) {
			if (k->value.key == key) {
				k->value.meaning = meaning;
				return;
			}
			k = k->pNext;
		}
	}
	cout << "\nThis word doesn't exist !" << endl;
}

void deleteWord(HashNode*& bucket, string key) {
	int index = hashString(key);
	if (bucket[index].pNext != NULL) {
		HashNode* k = bucket[index].pNext;
		if (k->value.key == key) {
			bucket[index].pNext = k->pNext;
			delete k;
			return;
		}
		else {
			HashNode* g = k;
			while (k != NULL) {
				if (k->value.key == key) break;
				g = k;
				k = k->pNext;
			}
			if (k != NULL) {
				g->pNext = k->pNext;
				delete k;
				return;
			}
		}
	}
	cout << "\nThis word doesn't exist !" << endl;
}

void insertUsage(HashNode* bucket, HashNode* usage) {
	for (int i = 0; i < tabSize; i++) {
		if (usage[i].pNext != NULL) {
			HashNode* k = usage[i].pNext;
			while (k != NULL) {
				HashNode* g = bucket[i].pNext;
				while (g != NULL) {
					if (k->value.key == g->value.key) {
						g->value.meaning.insert(g->value.meaning.size(), "\n");
						g->value.meaning.insert(g->value.meaning.size(), k->value.meaning);
						break;
					}
					g = g->pNext;
				}
				k = k->pNext;
			}
		}
	}
}

void loadFromFile(string filename, HashNode*& bucket) {
	clock_t start = clock();
	fstream f(filename, ios::in);
	if (f.fail()) {
		cout << "File does not exist !" << endl;
		return;
	}

	bucket = new HashNode[tabSize];
	HashNode* usage = new HashNode[tabSize];
	initializeHash(bucket);
	initializeHash(usage);

	string preKey = " ", preWord = " ";
	while (!f.eof()) {
		string curKey, curWord;
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
			insertWord(bucket, key, meaning);
		}
		else {
			// Save to insert "Usage" later
			insertWord(usage, preKey, curWord);
		}
		preKey = key;
		preWord = curWord;
	}
	// Insert "Usage"
	insertUsage(bucket, usage);

	clock_t end = clock();
	cout << "Loading time: " << end - start << " (ms)\n" << endl;
	memFree(usage);
	f.close();
}

void saveToFile(string filename, HashNode* bucket) {
	clock_t start = clock();

	fstream f(filename, ios::out);
	for (int i = 0; i < tabSize - 1; i++) {
		if (bucket[i].pNext != NULL) {
			HashNode* k = bucket[i].pNext;
			while (k != NULL) {
				f << k->value.key << "  " << k->value.meaning << "\n" << endl;
				k = k->pNext;
			}
		}
	}
	if (bucket[tabSize - 1].pNext != NULL) {
		HashNode* k = bucket[tabSize - 1].pNext;
		while (k->pNext != NULL) {
			f << k->value.key << "  " << k->value.meaning << "\n" << endl;
			k = k->pNext;
		}
		f << k->value.key << "  " << k->value.meaning;
	}

	clock_t end = clock();
	cout << "\nSaving time: " << end - start << " (ms)" << endl;
	f.close();
}

void outputData(HashNode* bucket) {
	cout << "Index\t\tKey\t\tMeaning" << endl;
	for (int i = 0; i < tabSize; i++) {
		if (bucket[i].pNext != NULL) {
			HashNode* p = bucket[i].pNext;
			while (p != NULL) {
				cout << i << "\t\t" << p->value.key << "\t\t" << p->value.meaning << endl;
				p = p->pNext;
			}
		}
	}
	cout << endl;
}

void menu(HashNode*& bucket) {
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
			searchWord(bucket, key);
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
			addNewWord(bucket, key, meaning);
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
			editMeaning(bucket, key, meaning);
			end = clock();

			cout << "\nEditing Time: " << end - start << " (ms)" << endl;
			system("pause");
		}
		else if (selection == 4) {
			cout << "Enter word: ";
			getline(cin, key);

			start = clock();
			deleteWord(bucket, key);
			end = clock();

			cout << "\nDeleting Time: " << end - start << " (ms)" << endl;
			system("pause");
		}
		else {
			break;
		}
		system("cls");
	}
}

int main() {
	HashNode* bucket;

	string filename = "Oxford English Dictionary.txt";
	loadFromFile(filename, bucket);

	menu(bucket);

	saveToFile("Oxford English Dictionary.txt", bucket);
	system("pause");
	memFree(bucket);
	return 0;
}