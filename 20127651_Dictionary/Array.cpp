#include <iostream>
#include <time.h>
#include <string>
#include <fstream>

using namespace std;

struct Word {
	string key;
	string meaning;
};

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

int binarySearch(Word* s, int l, int r, string key) {
	if (l <= r) {
		int mid = l + (r - l) / 2;
		if (s[mid].key == key) {
			return mid;
		}
		if (s[mid].key > key) {
			return binarySearch(s, l, mid - 1, key);
		}
		return binarySearch(s, mid + 1, r, key);
	}
	return -1;
}

void searchWord(Word* s, int n, string key) {
	int idx = binarySearch(s, 0, n - 1, key);
	if (idx != -1) {
		cout << "\nMeaning: " << s[idx].meaning << endl;
		return;
	}
	cout << "\nThis word doesn't exist !" << endl;
}

void addNewWord(Word*& s, int& n, string key, string meaning) {
	int pos = binarySearch(s, 0, n - 1, key);
	if (pos == -1) {
		n++;
		// Paste to new array, without wasting memory
		Word* temp = new Word[n];
		for (int i = 0; i < n - 1; i++) {
			temp[i] = s[i];
		}
		temp[n - 1].key = key;
		temp[n - 1].meaning = meaning;
		delete[] s;
		s = temp;

		// Update dictionary
		insertionSort(s, n);
		return;
	}
	cout << "\nThis word already exists !" << endl;
}

void editMeaning(Word* s, int n, string key, string meaning) {
	int pos = binarySearch(s, 0, n - 1, key);
	if (pos != -1) {
		s[pos].meaning = meaning;
		return;
	}
	cout << "\nThis word doesn't exist !" << endl;
}

void deleteWord(Word*& s, int& n, string key) {
	int pos = binarySearch(s, 0, n - 1, key);
	if (pos != -1) {
		for (int i = pos + 1; i < n; i++) {
			s[i - 1] = s[i];
		}
		n--;
		// Paste to new array, without wasting memory
		Word* temp = new Word[n];
		for (int i = 0; i < n; i++) {
			temp[i] = s[i];
		}
		delete[] s;
		s = temp;
		return;
	}
	cout << "\nThis word doesn't exist !" << endl;
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
		if ((key != "Usage") || (key == "Usage") && (meaning == "n. 1 use, treatment (damaged by rough usage). 2 customary practice, esp. In the use of a language or as creating a precedent in law.")) {
			count++;
		}
		preWord = curWord;
	}
	return count;
}

void loadFromFile(string filename, Word*& s, int& n) {
	clock_t start = clock();
	fstream f(filename, ios::in);
	if (f.fail()) {
		cout << "File does not exist !" << endl;
		return;
	}

	n = countLine(filename);
	s = new Word[n];

	int i = 0;
	string preWord = " ";
	while (!f.eof()) {
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

	clock_t end = clock();
	cout << "Loading time: " << end - start << " (ms)\n" << endl;
	f.close();
}

void saveToFile(string filename, Word* s, int n) {
	clock_t start = clock();

	fstream f(filename, ios::out);
	for (int i = 0; i < n - 1; i++) {
		f << s[i].key << "  " << s[i].meaning << "\n" << endl;
	}
	f << s[n - 1].key << "  " << s[n - 1].meaning;

	clock_t end = clock();
	cout << "\nSaving time: " << end - start << " (ms)" << endl;

	f.close();
}

void outputData(Word* s, int n) {
	for (int i = 0; i < n; i++) {
		cout << s[i].key << "  " << s[i].meaning << endl;
	}
	cout << endl;
}

void menu(Word*& s, int n) {
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
			searchWord(s, n, key);
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
			addNewWord(s, n, key, meaning);
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
			editMeaning(s, n, key, meaning);
			end = clock();

			cout << "\nEditing Time: " << end - start << " (ms)" << endl;
			system("pause");
		}
		else if (selection == 4) {
			cout << "Enter word: ";
			getline(cin, key);

			start = clock();
			deleteWord(s, n, key);
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
	Word* s;
	int n;

	string filename = "Oxford English Dictionary.txt";
	loadFromFile(filename, s, n);

	menu(s, n);

	saveToFile("Oxford English Dictionary.txt", s, n);
	system("pause");
	delete[] s;
	return 0;
}