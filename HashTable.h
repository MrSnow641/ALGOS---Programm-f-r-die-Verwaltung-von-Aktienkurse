#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <cmath>
#include <tuple>

using namespace std;

class Stock {
public:
    string name;
    string short_name;
    string wkn;
    string file;
    //vector<vector<string>> stockData;

    Stock(string n, string s, string k, string f) : name(n), short_name(s), wkn(k), file(f) {};
};

class HashTable {
private:
    array<tuple<string, string, string, string>, 2003> table;
    int hash(string key) {
        int hashVal = 0;
        int n = key.length();
        for (int i = 0; i < n; i++) {
            hashVal += key[i] * pow(7, (n - 1 - i));
        }
        int index = hashVal % 2003;

        return index;
    }
public:
    int counter;
    int sorting(string key, Stock* stock){
        int index = hash(key);
        int i = 0;
        string curr;
        if(key.size() < 6) //entshieden ob Kürzel doer Name eingegeben wurde
            curr = get<1>(table[index]);
        else
            curr = get<0>(table[index]);
        if(get<2>(table[index]) == "delete") {
            get<2>(table[index]) = stock->wkn;
            return 5467;
        }
        if(key == curr)
            return 4089;
            //Die Sondierung
        if(counter > 1000){
            return 2420;
        }
        while (get<2>(table[index]) != "") {
            index = (index + i * i) % 2003;
            i++;
        }
        return index;
    }

    void add(Stock* stock, string key) {
        int index = sorting(key, stock);

        /*if(key == "delete")
            return;*/
        //string nameIndexStok = get<0>(table[hash(key)]);
        if(index == 2420){
            cout<<"Die Tabelle ist voll. Sie koennen nichts mehr hinzufuegen."<<endl;
        }else if(index == 5467){
            cout<<"Aktie wurde hinzugefuegt.";
            counter++;
        }
        else if(index == 4089){
            cout<<"Die Aktie existiert schon."<<endl;
        }
        else
            table[index] = make_tuple(stock->name, stock->short_name, stock->wkn, stock->file);
            counter++;
    }

    tuple<string, string, string, string> search(string key, int method) {
        int index = hash(key);

        for (int i = 0; i < 2000; i++) {
            if (key == get<0>(table[index])) {
                return table[index];
            }
            if (key == get<1>(table[index])) {
                return table[index];
            }
            index = (index + i * i) % 2000;
        }
        return make_tuple("", "", "", ""); // Return empty tuple if Stock object not found
    }

    int import(string csv, string name, int method) {
        //check if Stock exists in table
        tuple<string, string, string, string>nameS = search(name, method);
        if(nameS == make_tuple("", "", "", "")){
            return 1;
        }
        //check if file exists
        fstream file (csv, ios::in);
        if(!file.is_open())
            return 2;
        //add file, if both stock and file exist
        int index = hash(name);
        auto myTuple = table[index];
        get<3>(myTuple) = csv;
        table[index] = myTuple;
        return 0;
    }

    void saveFile(int method) {
        string fileName;
        if(method == 0)
            fileName = "hash_table_stockName.txt";
        else if(method == 1)
            fileName = "hash_table_stockShort.txt";

        ofstream file(fileName);

        for (int i = 0; i < 2000; i++) {
            if(get<0>(table[i]) != "")
                file << get<0>(table[i]) << "," << get<1>(table[i]) << "," << get<2>(table[i])<< "," << get<3>(table[i]) << endl;
        }

        file.close();
    }
    int delStock(string name, int method) {
        int index = hash(name);

        switch (method) {
            case 0:
                if(get<0>(table[index])  == "")
                    return 1;
            case 1:
                if(get<1>(table[index])  == "")
                    return 1;
        }
        get<2>(table[index]) = "delete";
        counter--;
        return 0;
    }

    int loadFile(const string filename, int method) {
        string key;

        ifstream file(filename);
        if (!file.is_open()) {
            return 1;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, short_name, wkn, fileCSV;
            getline(ss, name, ',');
            getline(ss, short_name, ',');
            getline(ss, wkn, ',');
            getline(ss, fileCSV, ',');
            auto* stock = new Stock(name, short_name, wkn, fileCSV);
            //switch case überprüft ob wir uns in der shortTable oder nameTable bfinden,
            //um danach richtig zu hashen
            switch (method) {
                case 0:
                    key = name;
                    break;
                case 1:
                    key = short_name;
                    break;
            }
            add(stock, key);
        }

        file.close();
        return 0;
    }
    vector<double> closeList(string fileCSV, int columnNum){
        std::ifstream file(fileCSV);

        // Read the CSV file line by line
        std::string line;
        std::getline(file, line);
        std::vector<double> values;
        while (std::getline(file, line)) {
            // Split the line into columns
            std::stringstream ss(line);
            std::string column;
            int column_number = 0;
            while (std::getline(ss, column, ',')) {
                // Select the desired column (e.g., column 2)
                if (++column_number == columnNum) {
                    // Convert the column value to a double and add it to the vector
                    double value = std::stod(column);
                    values.push_back(value);
                }
            }
        }
        return values;
    }
};
