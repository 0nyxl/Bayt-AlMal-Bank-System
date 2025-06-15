#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>

using namespace std;

const string ClientsFileName = "C:\\Users\\dell\\Desktop\\BankUser.txt";     // Change The Path Here..

enum enMainMenu
{
    ShowClientList = 1,
    AddNewClient = 2,
    DeleteClient = 3,
    UpdateClient = 4,
    FindClient = 5,
    transactions = 6,
    Exit = 7
};
enum enTransactions {
    Deposit = 1,
    Withdraw = 2,
    TotalBalance = 3,
    MainMenu = 4
};

struct sClient {
    string AccountNumber="";
    string PinCode="";
    string Name="";
    string Phone="";
    double AccountBalance=0.0;
};

void BackToMainMenu();

int ChooseNumber() {
    int Number = 0;
    cout << "Please Select From [1]-[7]: ";
    cin >> Number;
    while (cin.fail() || Number < 1 || Number > 7) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Make Sure You Select From [1] to [7]: ";
        cin >> Number;  
    }
    return Number;
}
enMainMenu NumberSelected(int Number) {
    return static_cast<enMainMenu>(Number);
}
vector<string>Split_Function(string line, string delimiter = "#//#") {
    vector < string>vClientsData;
    size_t pos = 0;
    string word = "";
    while ((pos = line.find(delimiter)) != string::npos) {
        word = line.substr(0, pos);
        if (!word.empty())
        {
            vClientsData.push_back(word);
        }
        line.erase(0, pos + delimiter.length());
    }
    if (!line.empty())
    {
        vClientsData.push_back(line);
    }
    return vClientsData;
}
sClient FillClient(string line, string delimiter = "#//#") {
    vector<string>vClientsData = Split_Function(line,delimiter);
    sClient Temp;

    Temp.AccountNumber = vClientsData[0];
    Temp.PinCode = vClientsData[1];
    Temp.Name = vClientsData[2];
    Temp.Phone= vClientsData[3];
    Temp.AccountBalance = stod (vClientsData[4]);

    return Temp;
    
}
vector<sClient>FillAllClients() {
    vector<sClient>vClients;
    fstream file;
    string word = "";
    file.open(ClientsFileName, ios::in);
    if (file.is_open())
    {
        sClient Temp;

        while (getline(file, word)) {
            Temp = FillClient(word);
            vClients.push_back(Temp);
        }
        file.close();
    }
    return vClients;
}
void ShowCardInfo(const sClient& Client) {
    cout << "| " << left << setw(15) << Client.AccountNumber
        << "| " << left << setw(10) << Client.PinCode
        << "| " << left << setw(20) << Client.Name
        << "| " << left << setw(15) << Client.Phone
        << "| " << right << setw(5) << fixed << setprecision(2) << "$ " << Client.AccountBalance
        << " |\n";
}
void ShowAllClientCardInfo() {
    vector<sClient> vClients = FillAllClients();

    cout << "\n\n\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "-----------------------------------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(15) << "Account Number"
        << "| " << left << setw(10) << "Pin Code"
        << "| " << left << setw(20) << "Client Name"
        << "| " << left << setw(15) << "Phone"
        << "| " << right << setw(12) << "Balance"
        << " |\n";
    cout << "-----------------------------------------------------------------------------------------------------------\n";

    for (const sClient& client : vClients) {
        ShowCardInfo(client);
    }

    cout << "-----------------------------------------------------------------------------------------------------------\n";
}

bool CheckIfUserFound(const string& AccountNumber) {
    vector<sClient>Temp = FillAllClients();

    for (sClient& i : Temp) {
        if (i.AccountNumber== AccountNumber)
        {
            return true;
        }
    }
    return false;
}
sClient ReadNewClient() {
    sClient User;
    cout << "Enter Account Number: ";
    cin >> User.AccountNumber;

    while (CheckIfUserFound(User.AccountNumber)) {
        cout << "Account Number Already Exists. Please Try Another One: ";
        cin >> User.AccountNumber;
    }

    cout << "Enter Pin Code: ";
    cin >> User.PinCode;

    cin.ignore();           
    cout << "Enter Name: ";
    getline(cin, User.Name);

    cout << "Enter Phone: ";
    getline(cin, User.Phone);

   
    cout << "Enter Account Balance: ";
    while (!(cin >> User.AccountBalance)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a numeric balance: ";
    }


    return User;
}
string ConvertLineToRecord(sClient NewUser, string delimiter = "#//#") {
    string Client = "";

    Client += NewUser.AccountNumber + delimiter;
    Client += NewUser.PinCode + delimiter;
    Client += NewUser.Name + delimiter;
    Client += NewUser.Phone + delimiter;
    Client += to_string(NewUser.AccountBalance);

    return Client;
}
void AddRecordToFile(string Word) {

    fstream File;
    File.open(ClientsFileName, ios::app);
    if (File.is_open())
    {
        File << Word << "\n";
    }
    cout << "\nClient added successfully!\n";
}
string ReadAccountNumber(string msg) {
    string AccountNumber = "";
    cout << msg;
    cin >> AccountNumber;
    return AccountNumber;
}
void PrintInfo(const sClient& User) {

    cout << "\n" << string(70, '=') << "\n";
    cout << "\t\tClient Details\n";
    cout << string(70, '=') << "\n";

    cout << setw(20) << left << "Account Number:" << User.AccountNumber << "\n";
    cout << setw(20) << left << "Pin Code:" << User.PinCode << "\n";
    cout << setw(20) << left << "Full Name:" << User.Name << "\n";
    cout << setw(20) << left << "Phone Number:" << User.Phone << "\n";
    cout << setw(20) << left << "Account Balance:" << fixed << setprecision(2) << User.AccountBalance << "\n";

    cout << string(70, '=') << "\n";

}
void SaveAllClientsToFile(const vector<sClient>& clients) {

    fstream file(ClientsFileName, ios::out);

    if (file.is_open()) {
        for (const sClient& client : clients) {
            file << ConvertLineToRecord(client) <<"\n";
        }
        file.close();
    }
}
void RemoveClientFromRecord(const string& AccountNumber, vector<sClient>& Temp) {
    char YesOrNo;
    bool Found = false;

    for (size_t i = 0; i < Temp.size(); ++i) {
        if (Temp[i].AccountNumber == AccountNumber) {
            Found = true;

            PrintInfo(Temp[i]);

            cout << "\nAre you sure you want to delete this client? (Y/N): ";
            cin >> YesOrNo;

            if (tolower(YesOrNo) == 'y') {
                Temp.erase(Temp.begin() + i);
                SaveAllClientsToFile(Temp);
                cout << "\nClient deleted successfully.\n";
            }
            else {
                cout << "\nDeletion cancelled.\n";
            }

            break; 
        }
    }

    if (!Found) {
        cout << "There is no client with Account Number [" << AccountNumber << "].\n";
    }
}
void UpdateClientData(const string& AccountNumber, vector<sClient>& Temp) {
    bool found = false;

    for (size_t i = 0; i < Temp.size(); i++) {
        if (AccountNumber == Temp[i].AccountNumber) {
            found = true;

            cout << "\nClient Found:\n";
            PrintInfo(Temp[i]);

            cout << "\nEnter New Pin Code: ";
            cin >> Temp[i].PinCode;

            cin.ignore(); 

            cout << "Enter New Full Name: ";
            getline(cin, Temp[i].Name);

            cout << "Enter New Phone Number: ";
            getline(cin, Temp[i].Phone);

            cout << "Enter New Account Balance: ";
            while (!(cin >> Temp[i].AccountBalance)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a numeric balance: ";
            }

            // Save updates to file
            SaveAllClientsToFile(Temp);
            cout << "\nClient updated successfully!\n";

            break;
        }
    }

    if (!found) {
        cout << "\nNo client found with Account Number [" << AccountNumber << "].\n";
    }
}
bool FindAndPrintClientByAccountNumber(const string& AccountNumber, const vector<sClient>& vClients) {
    for (const sClient& client : vClients) {
        if (client.AccountNumber == AccountNumber) {
            PrintInfo(client);
            return true;
        }
    }
    return false;
}

void GoodBye() {
    const int width = 70;
    cout << "\n" << string(width, '-') << "\n";

    string line1 = "[+] Disconnected from Bayt AlMal Bank System...";
    string line2 = "[+] Session terminated securely.";
    string line3 = "[+] Goodbye";

    cout << setw((width + (int)line1.length()) / 2) << line1 << "\n";
    cout << setw((width + (int)line2.length()) / 2) << line2 << "\n";
    cout << setw((width + (int)line3.length()) / 2) << line3 << "\n";

    cout << string(width, '-') << "\n\n";
    
    string contactHeader = "[::] Connect With Me\n";
    string Name = "Created By Ahmad Shdeed";
    string github = "GitHub: https://github.com/0nyxl";
    string linkedin = "LinkedIn: https://linkedin.com/in/0nyxl";
    string twitter = "X (Twitter): https://x.com/0nyxl";
    string coffee = "Buy Me a Coffee: https://buymeacoffee.com/i0nyxl";
    string email = "Email: i0nyxl@hotmail.com";
    cout << setw((width + Name.length()) / 2) << Name << "\n";
    cout <<"\n" << setw((width + contactHeader.length()) / 2) << contactHeader << "\n";
    cout << setw((width + github.length()) / 2) << github << "\n";
    cout << setw((width + linkedin.length()) / 2) << linkedin << "\n";
    cout << setw((width + twitter.length()) / 2) << twitter << "\n";
    cout << setw((width + coffee.length()) / 2) << coffee << "\n";
    cout << setw((width + email.length()) / 2) << email << "\n";


    cout << string(width, '-') << "\n";

    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();

}
void DepositScreen() {
    system("cls");
    cout << string(50, '-')
        << "\n"
        << setw(35) << "Deposit Screen\n"
        << string(50, '-')
        << "\n";

}
void DepositUpdate(const string& AccountNumber, vector<sClient>& vClients) {
    bool found = false;

    for (size_t i = 0; i < vClients.size(); i++) {
        if (AccountNumber == vClients[i].AccountNumber) {
            found = true;
            PrintInfo(vClients[i]);
            double DepositAmount;
            cout << "Please Enter Deposit Amount: ";
            while (!(cin >> DepositAmount) || DepositAmount <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a **positive** amount: ";
            }
            vClients[i].AccountBalance += DepositAmount;


            SaveAllClientsToFile(vClients);
            cout << "\nDeposit completed successfully.\n\n";

            break;
        }
    }

    if (!found) {
        cout << "\nNo client found with Account Number [" << AccountNumber << "].\n";
    }
}

void WithdrawScreen() {
    system("cls");
    cout << string(50, '-')
        << "\n"
        << setw(35) << "Withdraw Screen\n"
        << string(50, '-')
        << "\n";

}
void WithdrawUpdate(const string& AccountNumber, vector<sClient>& vClients) {
    bool found = false;

    for (size_t i = 0; i < vClients.size(); i++) {
        if (AccountNumber == vClients[i].AccountNumber) {
            found = true;
            PrintInfo(vClients[i]);
            double DepositAmount;
            cout << "Please Enter Withdraw Amount: ";
            while (!(cin >> DepositAmount) || DepositAmount <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter amount: ";
            }
            vClients[i].AccountBalance -= DepositAmount;


            SaveAllClientsToFile(vClients);
            cout << "\nWithdraw completed successfully.\n\n";

            break;
        }
    }

    if (!found) {
        cout << "\nNo client found with Account Number [" << AccountNumber << "].\n";
    }
}
int TotalBalanceFunction(vector<sClient>& vClients) {
    double Total = 0;
    for (sClient& i : vClients) {
        Total += i.AccountBalance;
    }
    return Total;
}
void TotllBalanceMSG(vector<sClient>& vClients) {
    system("cls");

    cout << string(56, '-') << "\n";
    cout << setw(35) << "Total Balance Screen\n";
    cout << string(56, '-') << "\n";

    double totalBalance = TotalBalanceFunction(vClients);

    cout << "\nOur bank currently holds a total balance of [" << totalBalance << "] USD.\n";
    cout << string(56, '-') << "\n";

}
void PrintTransactionsMenu() {
    cout << string(50, '=') << "\n";
    cout << "\t     Transactions Menu\n";
    cout << string(50, '=') << "\n";

    cout << "[1]: Deposit\n";
    cout << "[2]: Withdraw\n";
    cout << "[3]: Total Balance\n";
    cout << "[4]: Main Menu\n";

    cout << string(50, '=') << "\n";
    cout << "Please select an option [1-4]: ";

}
short ReadTransactionsMenu() {

    short number = 0;
    cin >> number;

    while (cin.fail() || number < 1 || number > 4) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Make Sure You Select From [1] to [4]: ";
        cin >> number;
    }
    return number;


}
enTransactions CastNumber(short number) {

    return static_cast<enTransactions>(number);

}
void TransactionsMainMenu_Switch(enTransactions number) {

    switch (number)
    {
    case Deposit: {
        DepositScreen();
        vector<sClient>vClients = FillAllClients();
        string DepositAcoount = ReadAccountNumber("Please Enter Account Number: ");
        system("cls");
        DepositUpdate(DepositAcoount, vClients);
        BackToMainMenu();
        break;
    }
    case Withdraw:
    {
        system("cls");
        WithdrawScreen();
        vector<sClient>vClients = FillAllClients();
        string DepositAcoount = ReadAccountNumber("Please Enter Account Number: ");
        system("cls");
        WithdrawUpdate(DepositAcoount, vClients);
        BackToMainMenu();
        break;
    }
    case TotalBalance:
    {
        system("cls");
        vector<sClient>vClients = FillAllClients();
        TotllBalanceMSG(vClients);
        BackToMainMenu();
        break;
    }
    case MainMenu:
    {
        system("cls");
        BackToMainMenu();
    }
    break;
    default:
        break;
    }


}

void ClientSelect(enMainMenu choice) {

    switch (choice)
    {
    case ShowClientList: {
        system("cls");
        ShowAllClientCardInfo();
        BackToMainMenu();
        break;
    }
    case AddNewClient: {
        system("cls");
        sClient User = ReadNewClient();
        string Word = ConvertLineToRecord(User);
        AddRecordToFile(Word);
        BackToMainMenu();
        break;
    }
    case DeleteClient: {
        system("cls");
        vector<sClient>vClients = FillAllClients();
        string AccountNumberForDelete = ReadAccountNumber("Enter Account Number: ");
        RemoveClientFromRecord(AccountNumberForDelete,vClients); // Here We Can Maybe Use <Iterators Or Pointers> .
        BackToMainMenu();
        break;
    }
    case UpdateClient: {
        system("cls");
        vector<sClient>vClients = FillAllClients();
        string AccountUpdate = ReadAccountNumber("Enter Account Number: ");
        UpdateClientData(AccountUpdate, vClients);
        BackToMainMenu();
        break;
    }
    case FindClient: {
        system("cls");
        vector<sClient>vClients = FillAllClients();
        string FindAccount = ReadAccountNumber("Enter Account Number: ");
        if (!FindAndPrintClientByAccountNumber(FindAccount, vClients)) {
            cout << "\nNo client found with Account Number [" << FindAccount << "].\n";
        }
        BackToMainMenu();
        break;
    }
    case transactions: {
        system("cls");
        PrintTransactionsMenu();
        int number = ReadTransactionsMenu();
        enTransactions s = CastNumber(number);
        TransactionsMainMenu_Switch(s);
        break;
    }
    case Exit: {
        system("cls");
        GoodBye();
        break;
    }
    
    default:
        break;
    }



}
void Print_Main_Menu() {
    cout << string(70, '=') << "\n";
    cout << "\t\tWelcome To Bayt AlMal Bank System\n";
    cout << string(70, '=') << "\n";

    cout << "[1]: Show Client List\n"
        << "[2]: Add New Client\n"
        << "[3]: Delete Client\n"
        << "[4]: Update Client Info\n"
        << "[5]: Find Client\n"
        << "[6]: Transactions\n"
        << "[7]: Exit\n";
    cout << string(70, '=') << "\n";
    short int Choice = ChooseNumber();
    enMainMenu Selected = NumberSelected(Choice);
    ClientSelect(Selected);
}
void BackToMainMenu() {
    cout << "Press any key to return to main menu..";
    system("pause > nul");
    system("cls");
    Print_Main_Menu();
}



int main() {

    Print_Main_Menu();
    
}
