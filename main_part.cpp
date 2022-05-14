#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class complex {//класс комплексных чисел и математические операции к ним
public:
	long long a, b;
	friend ostream& operator<< (ostream& out, const complex& cp);
	friend istream& operator>> (istream& out, const complex& cp);
	void set(long long real_part, long long non_real_part) {
		(*this).a = real_part;
		(*this).b = non_real_part;
	}
	complex operator+(complex secon) {
		complex c;
		c.a = (*this).a + secon.a;
		c.b = (*this).b + secon.b;
		return c;
	}
	complex operator-(complex b) {
		complex c;
		c.set((*this).a - b.a, (*this).b - b.b);
		return c;
	}
	complex operator*(complex secon) {
		complex c;
		c.a = (*this).a * secon.a - (*this).b * secon.b;
		c.b = (*this).a * secon.b + secon.a * (*this).b;
		return c;
	}
	complex operator/(complex secon) {
		complex c;
		c.a = ((*this).a * secon.a + (*this).b * secon.b) / (secon.a * secon.a + secon.b * secon.b);
		c.b = (secon.a * (*this).b - (*this).a * secon.b) / (secon.a * secon.a + secon.b * secon.b);
		return c;
	}
};

vector<string>split(string a, char u = ' ') {//функция разделения строк по определенному символу
	vector<string> sent(1);
	int ctr = 0;
	for (char i : a) {
		if (i != u)sent[ctr] += i;
		else {
			++ctr;
			sent.push_back("");
		}
	}
	return sent;
}

ostream& operator<< (ostream& out, const complex& a) { //вывод к.чисел
	out << a.a << '+' << a.b << 'i';
	return out;
}
istream& operator>> (istream& in, complex& a) {//ввод к.чисел
	string u;
	getline(cin, u,'i');
	vector<string>z = split(u, '+');
	a.a = stoi(z[0]);
	a.b = stoi(z[1]);
	return in;
}
complex stocomp(string u) { //костыль чтобы работала дешифровка в 1 функцию
	u[u.size() - 1] = ' ';
	vector<string>z = split(u, '+');
	complex a;
	a.a = stoi(z[0]);
	a.b = stoi(z[1]);
	return a;
}



pair<complex, complex> vector_x_crypting(char l, char k1, char k2) { //шифрование методом перемножения векторов
	complex lett,key,key2;
	l -= 'a'; k1 -= 'a'; k2 -= 'a';
	lett.a = l; lett.b = k1 + k2;
	key.a = k1; key.b = k2;
	lett = lett * key;
	key2.a = lett.a / 26;
	key2.b = lett.b / 26;
	lett.a %= 26; lett.b %= 26;
	pair<complex, complex> c;
	c.first = lett;
	c.second = key2;
	return c;
}
char vector_x_decrypting(pair<complex, complex> kpl, char k1, char k2) {//дешифровка тем же методом
	k1 -= 'a'; k2 -= 'a';
	kpl.first.a += kpl.second.a * 26;
	kpl.first.b += kpl.second.b * 26;
	complex key; key.a = k1; key.b = k2;
	kpl.first = kpl.first / key;
	return kpl.first.a + 'a';
}

string crpt(pair<string, string> keys, string word) {//шифрование трехмерным шифром виженера
	string cpt = "";
	for (int i = 0; i < word.size(); ++i) cpt += (keys.first[i % keys.first.size()] + keys.second[i % keys.second.size()] + word[i] - 3 * 'a') % 26 + 'a';
	return cpt;
}
string dcrpt(pair<string, string> keys, string word) {//дешифрование трехмерного шифра виженера 
	string dcpt = "";
	for (int i = 0; i < word.size(); ++i) dcpt += (word[i] + 52 - keys.first[i % keys.first.size()] - keys.second[i % keys.second.size()] - 3 * 'a') % 26 + 'a' + 24;
	return dcpt;
}

pair<string, string>key_gen(vector<string> words) { //генерация ключей
	srand(static_cast<unsigned int>(clock()));
	string k1="", k2="";
	for (int i = 0; i < words.size() + rand() % 17 + 3; ++i) k1 += rand() % 26 + 'a';
	for (int i = 0; i < words.size() + rand() % 17 + 3; ++i) k2 += rand() % 26 + 'a';
	pair<string, string> t;
	t.first = k1; t.second = k2;
	return t;
}
void full_crypt(vector<string> words) {//полное шифрование 1 функцией
	pair<string, string> keys = key_gen(words);
	ofstream file;
	file.open("crpt.txt");
	file << keys.first << ' ' << keys.second << '\n' << words.size() << '\n';
	for (auto i : words)file << i.size() << ' ';
	file << '\n';
	for (auto i = 0; i < words.size(); ++i)words[i] = crpt(keys,words[i]);
	int ctr = 0;
	for (int i = 0; i < words.size(); ++i) {
		for (int j = 0; j < words[i].size(); ++j) {
			pair<complex, complex> bu = vector_x_crypting(words[i][j], keys.first[ctr % keys.first.size()], keys.second[ctr % keys.second.size()]);
			file << bu.first << ' ' << bu.second << '\n';
			++ctr;
		}
	}
	file.close();
}
vector<string>full_decrypt(string file_name) {//полное дешифрование одной функцией
	ifstream file;
	file.open(file_name);
	int hmw,ctr=0;
	pair<string, string> keys; file >> keys.first >> keys.second >> hmw;
	vector<int>words_len(hmw);
	vector<string>words(hmw, "");
	for (int i = 0; i < hmw; ++i)file >> words_len[i];
	for (int i = 0; i < hmw; ++i) {
		for (int j = 0; j < words_len[i]; ++j) {
			pair<complex, complex>bu; string a, b; file >> a >> b; bu.first = stocomp(a); bu.second = stocomp(b);
			words[i] += vector_x_decrypting(bu, keys.first[ctr % keys.first.size()], keys.second[ctr % keys.second.size()]);
			++ctr;
		}
		words[i] = dcrpt(keys,words[i]);
	}
	file.close();
	return words;
}

int main() {
	//freopen("c.txt", "w", stdout);
	string text;
	getline(cin, text);
	vector<string>words = split(text);
	
	full_crypt(words);

	system("pause");
}
//this sent is great
