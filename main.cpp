#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "HashTable.h"

using namespace std;

void ADD(HashTable* nameTable, HashTable* shortTable){
    string nameS, shortS, wknS;

    cout<<"Was ist der Name der Aktie: ";
    cin>> nameS;
    cout<<endl;
    cout<<"Was ist der Kuerzel: ";
    cin>> shortS;
    cout<<endl;
    cout<<"Was ist die WKN: ";
    cin>> wknS;
    cout<<endl;

    if(nameS.size() < 6 || shortS.size() > 5){
        cout<<"Input invalid. Try again";
        return;
    }
    Stock *stock1;
    stock1 = new Stock(nameS, shortS, wknS, " ");
    nameTable->add(stock1, nameS);
    shortTable->add(stock1, shortS);
}

void SEARCH(HashTable* nameTable, HashTable* shortTable){
    int inputSearch;
    string key;
    tuple<string, string, string, string> result;
    int correct = 1;

    while(correct){
        cout<<"Nach welcher Aktie suchen Sie:";
        cin>>key;
        cout<<endl;
        if(key.size() < 6 && key.size()>0)
            inputSearch = 2;
        else
            inputSearch = 1;
        switch (inputSearch) {
            case 1:
                result = nameTable->search(key, 0);
                correct = 0;
                break;
            case 2:
                result = shortTable->search(key, 1);
                correct = 0;
                break;
        }
    }

    vector<string> lastRowValues;
    ifstream csvFile(get<3>(result));

    if (!csvFile.is_open()) {
        cout << "Error beim oeffnen des Files"<<endl;

    }

    string line;
    while (getline(csvFile, line)) {
        vector<string> values;
        string value;
        istringstream ss(line);
        while (getline(ss, value, ',')) {
            values.push_back(value);
        }
        lastRowValues = values;
    }

    csvFile.close();

    if (result == make_tuple("", "", "", "") || get<2>(result) =="delete") {
        cout << "Aktie konnte nicht gefunden werden" << endl;
    } else {
        string name = get<0>(result);
        string short_name = get<1>(result);
        string wkn = get<2>(result);

        // Print the returned values
        cout << "Name: " << name << endl;
        cout << "Kuerzel: " << short_name << endl;
        cout << "WKN: " << wkn << endl;

        cout << "Die aktuellsten Werte (Date,Open,High,Low,Close,Volume,Adj Close):";
        cout << fixed << setprecision(2);
        for (const auto& value : lastRowValues) {
            cout << value << "; ";
        }

        cout<<endl;
    }
}

void IMPORT(HashTable* nameTable, HashTable* shortTable){
    string csv, nameS, short_name;
    int check;
    tuple<string, string, string, string> stock;


    cout<<"Was ist der Name des Files, den Sie importierne moechten: ";
    cin>>csv;
    cout<<endl;
    cout<<"Geben Sie den Namen der Aktie ein, die mit dem File verknuepft werden sollt(keine Kuerzel): ";
    cin>>nameS;
    cout<<endl;
    stock = nameTable->search(nameS, 0);
    if(get<2>(stock) == "delete")
        check = 1;
    else
        check = nameTable->import(csv, nameS, 0);
    switch (check) {
        case 0:
            short_name = get<1>(stock);
            shortTable->import(csv, short_name, 1);
            cout<<"Die csv wurde erfolgreich verknuepft"<<endl;
            break;
        case 1:
            cout<<"Die Aktie existiert nicht. Bitte fuellen Sie als erstes die Aktie ein und importieren erst dann die csv"<<endl;
            break;
        case 2:
            cout<<"Die csv existiert nicht. Bitte kopieren Sie es im richtigen Ordner und versuchen den Import dann erneut."<<endl;
            break;
    }
}

void DELETE(HashTable* nameTable, HashTable* shortTable){
    string stockName;

    cout<<"Geben Sie den Namen der Aktie ein, die Sie loeschen moechten (kein Kuerzel): ";
    cin>>stockName;
    tuple<string, string, string, string> stock = nameTable->search(stockName, 0);
    string short_name = get<1>(stock);
    int result = nameTable->delStock(stockName, 0);
    int tmp = shortTable->delStock(short_name, 1);
    //Dadurch dass die beiden Tabellen synchronisiert sind, muss nur result überprüft werden
    switch (result) {
        case 0:
            cout<<endl<<"Aktie wurde erfolgreich geloescht."<<endl;
            break;
        case 1:
            cout<<"Aktie existiert nicht.";
            break;
    }
}

void LOAD(HashTable* nameTable, HashTable* shortTable){
    int stat;
    for(int i = 0; i<1;i++) {
        cout << "Hashtabelle wird gesucht..."<<endl;

        stat = nameTable->loadFile("hash_table_stockName.txt", 0);
        if(stat == 1) {
            cout<< "hash_table_stockName.txt existiert nicht. Erstellen Sie es nochmal und versuchen sie es dann erneut"<< endl;
            break;
        }
        stat = shortTable->loadFile("hash_table_stockShort.txt", 1);
        if(stat == 1){
            cout<< "hash_table_stockShort.txt existiert nicht. Erstellen Sie es nochmal und versuchen sie es dann erneut"<< endl;
            break;
        }
        cout << endl;
    }
}

void PLOT(HashTable* nameTable, HashTable* shortTable){
    string stockName;
    tuple<string, string, string, string> stock;

    cout<<"Fuegen Sie die Aktie ein, welche sie ausgegeben haben wollen:";
    cin>>stockName;
    if(stockName.size() < 6)
        stock = shortTable->search(stockName, 1);
    else
        stock = nameTable->search(stockName, 0);

    string fileCSV = get<3>(stock);

    // Check if the file was opened successfully
    ifstream file(fileCSV);
    if (get<2>(stock) =="delete") {
        cout << "Aktie konnte nicht gefunden werden" << endl;
        return;
    }
    if (!file.is_open()) {
        cout << "Failed to open file." << std::endl;
    }

    vector<double> closeValues = nameTable->closeList(fileCSV, 5);
    auto maxValue = *max_element(closeValues.begin(), closeValues.end());
    auto minValue = *min_element(closeValues.begin(), closeValues.end());
    auto graphHeight = (maxValue - minValue) / 10;

    cout << "Value" << endl;
    cout << fixed << setprecision(2);

    int startIndex = max(0, (int)closeValues.size() - 30); // start index to print
    for (double y = maxValue; y >= minValue; y -= graphHeight) {
        cout << setw(6) << right << y << " |";
        for (int i = startIndex; i < closeValues.size(); i++) {
            double x = closeValues[i];
            if (x >= y) {
                cout << " *";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }

// Print the horizontal axis
    const char *xAchse = "_";

    cout << "        " ;
    int sizeX;
    if(closeValues.size() > 30)
        sizeX = 30;
    else
        sizeX = closeValues.size();
    for(int i=1;i<=sizeX;i++) {
        cout << " "<<xAchse;
    }
    cout<<endl;
}

int main() {
    HashTable nameTable;
    HashTable shortTable;
    nameTable.counter = 0;
    shortTable.counter = 0;
    int input, status = 0;

    while(!status){
        cout<<endl<<"1:Add, 2: Delete; 3: Import, 4: Search, 5: Plot, 6: Save File, 7: Load File, 8: Quit Programm.";
        cout<<endl<<"Tippen Sie die Zahl von einer der Auswahlmoeglichkeiten von oben ein: ";
        cin>>input;
        cout<<endl;


        switch (input) {
            case 1:
                ADD(&nameTable, &shortTable);
                break;
            case 2:
                DELETE(&nameTable, &shortTable);
                break;
            case 3:
                IMPORT(&nameTable, &shortTable);
                break;
            case 4:
                SEARCH(&nameTable, &shortTable);
                break;
            case 5:
                PLOT(&nameTable, &shortTable);
                break;
            case 6:
                nameTable.saveFile(0);
                shortTable.saveFile(1);
                cout<<"Hashtable wurde erfolgreich gespeichert.";
                break;
            case 7:
                LOAD(&nameTable, &shortTable);
                break;
            case 8:
                cout<<"Das Programm wird beendet.";
                status = 1;
                break;
            default:
                cout<<"Nicht gueltige Eingabe, versuchen Sie es erneut.";
                // Clear input stream
                cin.clear();
                // Ignore remaining characters in the input buffer up to the next newline character
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
        }
    }
    return 0;
}
