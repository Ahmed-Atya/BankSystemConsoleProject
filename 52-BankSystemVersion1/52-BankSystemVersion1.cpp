#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
};

void FillStructWithStringWords(sClient& BankAccount) {
    cout << "Enter Account Number" << endl;
    getline(cin, BankAccount.AccountNumber);
    cout << "Enter Account Pin Code" << endl;
    getline(cin, BankAccount.PinCode);
    cout << "Enter Account Name" << endl;
    getline(cin, BankAccount.Name);
    cout << "Enter Account Phone" << endl;
    getline(cin, BankAccount.Phone);
    cout << "Enter Account Balance" << endl;
    cin >> BankAccount.AccountBalance;
    cin.ignore();
}

string JoinStructElementsIntoString(sClient& ClientInfo, string Separator) {
    string Record = "";
    Record += ClientInfo.AccountNumber + Separator;
    Record += ClientInfo.PinCode + Separator;
    Record += ClientInfo.Name + Separator;
    Record += ClientInfo.Phone + Separator;
    Record += to_string(ClientInfo.AccountBalance);
    return Record;
}

string ReadString() {
    string Word;
    cout << "Enter an Account Number: ";
    getline(cin, Word);
    return Word;
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

vector<string> SplitStringsIntoVector(string Word) {
    vector<string> Words;
    string CurrentWord = "";

    for (int i = 0; i < Word.length(); i++) {
        if (Word[i] == '/' || Word[i] == '#') {
            if (CurrentWord != "") {
                Words.push_back(CurrentWord);
                CurrentWord = "";
            }
        }
        else {
            CurrentWord += Word[i];
        }
    }

    if (CurrentWord != "") {
        Words.push_back(CurrentWord);
    }

    return Words;
}

sClient ConvertLineToRecord(string Line) {
    sClient Client;
    vector<string> vClientData = SplitStringsIntoVector(Line);

    if (vClientData.size() >= 5) {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];

        try {
            Client.AccountBalance = stod(vClientData[4]);
        }
        catch (...) {
            Client.AccountBalance = 0.0;  
        }
    }
    else {
        
        Client.AccountNumber = "";
        Client.PinCode = "";
        Client.Name = "";
        Client.Phone = "";
        Client.AccountBalance = 0.0;
    }

    return Client;
}

vector <sClient> LoadClientsDataFromFile(string FileName) {
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        string Line;
        while (getline(MyFile, Line))
        {
           
            if (Line.empty() || Line.find_first_not_of(" \t\r\n") == string::npos) {
                continue;
            }

            sClient Client = ConvertLineToRecord(Line);

            
            if (!Client.AccountNumber.empty()) {
                vClients.push_back(Client);
            }
        }
        MyFile.close();
    }

    return vClients;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}

string JoinVectorElementsIntoString(sClient& ClientInfo, string Separator) {
    string Record = "";
    Record += ClientInfo.AccountNumber + Separator;
    Record += ClientInfo.PinCode + Separator;
    Record += ClientInfo.Name + Separator;
    Record += ClientInfo.Phone + Separator;
    Record += to_string(ClientInfo.AccountBalance);
    return Record;
}

void RefreshDataAfterAnyProcess(string FileName, vector<sClient>& vClients) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {
        for (sClient Client : vClients) {
            string Line = JoinVectorElementsIntoString(Client, "#");
            MyFile << Line << endl;
        }
        MyFile.close();
    }
}

sClient UpdateClientByAccountNumber(vector<sClient>& vClients, string AccountNumber) {
    sClient UpdatedClient;
    for (int i = 0; i < vClients.size(); i++) {
        if (vClients[i].AccountNumber == AccountNumber) {
            UpdatedClient = vClients[i];

            cout << "Enter new Pincode for the client:\n";
            getline(cin >> ws, UpdatedClient.PinCode);
            cout << "Enter new Name for the client:\n";
            getline(cin, UpdatedClient.Name);
            cout << "Enter new Phone for the client:\n";
            getline(cin, UpdatedClient.Phone);
            cout << "Enter new Account Balance for the client:\n";
            cin >> UpdatedClient.AccountBalance;
            cin.ignore();

            vClients[i] = UpdatedClient;
            break;
        }
    }
    return UpdatedClient;
}

void SearchForClientToBeUpadatedByAccounNumber(vector<sClient>& vClients, string AccountNumber) {
    for (sClient Client : vClients) {
        if (Client.AccountNumber == AccountNumber) {
            PrintClientCard(Client);
            cout << "\n\nAre you sure you want to update this client? (Y/N): ";
            char Answer;
            cin >> Answer;
            cin.ignore();

            if (Answer == 'Y' || Answer == 'y') {
                sClient UpdatedClient = UpdateClientByAccountNumber(vClients, AccountNumber);
                RefreshDataAfterAnyProcess(ClientsFileName, vClients);
                cout << "\nClient with Account Number " << UpdatedClient.AccountNumber << " has been Updated successfully.\n";
            }
            pauseScreen();
            return;
        }
    }
    cout << "\nClient with Account Number " << AccountNumber << " not found.\n";
    pauseScreen();
}

void SearchForClientToBeDeletedByAccounNumber(vector<sClient>& vClients, string AccountNumber) {
    for (int i = 0; i < vClients.size(); i++) {
        if (vClients[i].AccountNumber == AccountNumber) {
            PrintClientCard(vClients[i]);
            cout << "\n\nAre you sure you want to delete this client? (Y/N): ";
            char Answer;
            cin >> Answer;
            cin.ignore();

            if (Answer == 'Y' || Answer == 'y') {
                vClients.erase(vClients.begin() + i);
                RefreshDataAfterAnyProcess(ClientsFileName, vClients);
                cout << "\nClient with Account Number " << AccountNumber << " has been deleted successfully.\n";
            }
            pauseScreen();
            return;
        }
    }
    cout << "\nClient with Account Number " << AccountNumber << " not found.\n";
    pauseScreen();
}

void SearchForClientByAccounNumber(vector<sClient>& vClients, string AccountNumber) {
    for (sClient Client : vClients) {
        if (Client.AccountNumber == AccountNumber) {
            PrintClientCard(Client);
            pauseScreen();
            return;
        }
    }
    cout << "\nClient with Account Number " << AccountNumber << " not found.\n";
    pauseScreen();
}

void ShowAllClients(vector<sClient>& vClients) {
    if (vClients.empty()) {
        cout << "\nNo clients found in the system.\n";
        pauseScreen();
        return;
    }

    cout << "\nAll Clients:\n";
    cout << "\n\n\t\t\t\tClient List (" << vClients.size() << ") Client(s)." << endl;

    // Print top border
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    // Print table header
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(15) << "Phone";
    cout << "| " << left << setw(15) << "Balance" << endl;

    // Print separator line
    cout << "_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sClient Client : vClients) {
        cout << "| " << left << setw(15) << Client.AccountNumber;
        cout << "| " << left << setw(10) << Client.PinCode;
        cout << "| " << left << setw(40) << Client.Name;
        cout << "| " << left << setw(15) << Client.Phone;
        cout << "| " << left << setw(15) << Client.AccountBalance << endl;


    }
    cout << "\nTotal Number of Clients: " << vClients.size() << endl;
    pauseScreen();
}
void ShowTotalBalances(vector<sClient>& vClients) {
    if (vClients.empty()) {
        cout << "\nNo clients found in the system.\n";
        pauseScreen();
        return;
    }

    cout << "\nAll Clients:\n";
    cout << "\n\n\t\t\t\tClient List (" << vClients.size() << ") Client(s)." << endl;

    // Print top border
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    // Print table header
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(15) << "Balance" << endl;

    // Print separator line
    cout << "_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sClient Client : vClients) {
        cout << "| " << left << setw(15) << Client.AccountNumber;
        cout << "| " << left << setw(40) << Client.Name;
        cout << "| " << left << setw(15) << Client.AccountBalance << endl;

    }
    cout << "\nTotal Number of Clients: " << vClients.size() << endl;
    pauseScreen();
}

void AddNewClient(vector<sClient>& vClients) {
    sClient NewClient;
    FillStructWithStringWords(NewClient);
    vClients.push_back(NewClient);
    RefreshDataAfterAnyProcess(ClientsFileName, vClients);
    cout << "\nNew client with Account Number " << NewClient.AccountNumber << " has been added successfully.\n";

    cout << "\nDo you want to add another client? (Y/N): ";
    char Answer;
    cin >> Answer;
    cin.ignore();

    if (Answer == 'Y' || Answer == 'y') {
        AddNewClient(vClients);
    }

}

void MakeAdeposit(vector <sClient>& vClients,string AccountNumber) {
    for (sClient& Client : vClients) {
    
      if(Client.AccountNumber == AccountNumber) {
            double Amount;
            cout << "Enter the amount to deposit: ";
            cin >> Amount;
            cin.ignore();
            if (Amount > 0) {
                Client.AccountBalance += Amount;
                RefreshDataAfterAnyProcess(ClientsFileName, vClients);
                cout << "\nDeposit successful. New balance: " << Client.AccountBalance << endl;
                return;
            }
            else {
                cout << "\nInvalid amount. Deposit failed.\n";
            }
           
	  }
    }
    pauseScreen();

}
void WidthdrawAnAmount(vector <sClient>& vClients, string AccountNumber) {
    for (sClient& Client : vClients) {

        if (Client.AccountNumber == AccountNumber) {
            double Amount;
            cout << "Enter the amount to deposit: ";
            cin >> Amount;
            cin.ignore();
            if (Amount > Client.AccountBalance) {
                cout << "\nInsufficient amount. Withdraw failed.\n";
              
            }
            else {
                Client.AccountBalance -= Amount;
                RefreshDataAfterAnyProcess(ClientsFileName, vClients);
                cout << "\nWithdraw successful. New balance: " << Client.AccountBalance << endl;
            }
            return;
        }
    }
   
}
void PerformBankSystemTransactionsMenuOption(vector <sClient>& vClients) {
    int Option = 0;
    cout << "\nEnter your option? ";
    cin >> Option;
    cin.ignore();
    switch (Option) {
    case 1:
		MakeAdeposit(vClients, ReadString());
        break;
    case 2:
		WidthdrawAnAmount(vClients, ReadString());
        break;
    case 3:
        ShowTotalBalances(vClients);
        break;
    case 4:
        cout << "Main Menu.\n";
        pauseScreen();
        break;
    default:
        cout << "Invalid option, try again.\n";
        pauseScreen();
    }
}
void ShowTransactionsScreen(vector <sClient>& vClients) {
    system("cls");
    cout << "\n=============================\n";
    cout << "    Bank System Transactions  \n";
    cout << "=============================\n";
    cout << "Choose what do you want to do? \n";
    cout << "1. Deposit.\n";
    cout << "2. Withdraw.\n";
    cout << "3. Total balances.\n";
    cout << "4. Main Menu.\n";
    PerformBankSystemTransactionsMenuOption(vClients);

}


void PrintBankSystemMenu() {
    cout << "\n=============================\n";
    cout << "    Bank System Main Menu    \n";
    cout << "=============================\n";
    cout << "Choose what do you want to do? \n";
    cout << "1. Add new client.\n";
    cout << "2. Update existing client.\n";
    cout << "3. Delete existing client.\n";
    cout << "4. Find client by account number.\n";
    cout << "5. Show all clients.\n";
	cout << "6. Transactions \n";
    cout << "7. Exit.\n";
}

void PerformBankSystemMainMenuOption(vector <sClient>& vClients) {
    int Option = 0;
    cout << "\nEnter your option? ";
    cin >> Option;
    cin.ignore();

    switch (Option) {
    case 1:
        cout << "Add new client.\n";
        AddNewClient(vClients);
        break;
    case 2:
        cout << "Update existing client.\n";
        SearchForClientToBeUpadatedByAccounNumber(vClients, ReadString());
        break;
    case 3:
        cout << "Delete existing client.\n";
        SearchForClientToBeDeletedByAccounNumber(vClients, ReadString());
        break;
    case 4:
        cout << "Find client by account number.\n";
        SearchForClientByAccounNumber(vClients, ReadString());
        break;
    case 5:
        cout << "Show all clients.\n";
        ShowAllClients(vClients);
        break;
    case 6:
        ShowTransactionsScreen(vClients);
        break;
    case 7:
        cout << "Exiting program. Goodbye!\n";
        exit(0);
        break;
    default:
        cout << "Invalid option, try again.\n";
        pauseScreen();
    }
}

int main() {
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    while (true) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

        PrintBankSystemMenu();
        PerformBankSystemMainMenuOption(vClients);
    }

    return 0;
}
