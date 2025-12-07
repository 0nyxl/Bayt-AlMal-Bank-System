#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include <limits>

using namespace std;

// Project File TXT (Users And Client's):
const string ClientsFileName = "/home/voldemort/Project/BankUser.txt";
const string UserDataFile = "/home/voldemort/Project/UsersLogin.txt";

enum enMainMenu
{
    ShowClientList = 1,
    AddNewClient = 2,
    DeleteClient = 3,
    UpdateClient = 4,
    FindClient = 5,
    transactions = 6,
    Manageusers = 7,
    Logout = 8
};

enum enTransactions
{
    Deposit = 1,
    Withdraw = 2,
    TotalBalance = 3,
    MainMenu = 4
};

enum enManageUsers
{
    ListUsers = 1,
    AddNewUser = 2,
    DeleteUser = 3,
    UpdateUser = 4,
    FindUser = 5,
    MainMenuu = 6
};

enum enMainMenuePermissions
{
    eAll = (1 << 7) - 1,
    pListClients = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClients = 8,
    pFindClient = 16,
    pTranactions = 32,
    pManageUsers = 64
};

struct sClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0.0;
};

struct sUser
{
    string username = "";
    string password = "";
    int permission = 0;
};

sUser CurrentUser;


// ---------- CROSS-PLATFORM HELPERS ----------
void ClearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Pause()
{
#ifdef _WIN32
    system("pause > nul");
#else
    cout << "\nPress Enter to continue...";
    // clear any error state and discard leftover input, then wait for Enter
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}
// --------------------------------------------

void BackToMainMenu();
void Print_Main_Menu();
void Start();

int ChooseNumber()
{
    int Number = 0;
    cout << "Please Select From [1]-[8]: ";
    cin >> Number;
    while (cin.fail() || Number < 1 || Number > 8)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Make Sure You Select From [1] to [8]: ";
        cin >> Number;
    }
    return Number;
}

enMainMenu NumberSelected(int Number)
{
    return static_cast<enMainMenu>(Number);
}

vector<string> Split_Function(string line, string delimiter = "#//#")
{
    vector<string> vClientsData;
    size_t pos = 0;
    string word = "";
    while ((pos = line.find(delimiter)) != string::npos)
    {
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

sClient FillClient(string line, string delimiter = "#//#")
{
    vector<string> vClientsData = Split_Function(line, delimiter);
    sClient Temp;
    if (vClientsData.size() >= 5)
    {
        Temp.AccountNumber = vClientsData[0];
        Temp.PinCode = vClientsData[1];
        Temp.Name = vClientsData[2];
        Temp.Phone = vClientsData[3];
        Temp.AccountBalance = stod(vClientsData[4]);
    }
    return Temp;
}

vector<sClient> FillAllClients()
{
    vector<sClient> vClients;
    fstream file;
    string word = "";
    file.open(ClientsFileName, ios::in);
    if (file.is_open())
    {
        sClient Temp;
        while (getline(file, word))
        {
            if (word.empty())
                continue;
            Temp = FillClient(word);
            vClients.push_back(Temp);
        }
        file.close();
    }
    return vClients;
}

void ShowCardInfo(const sClient &Client)
{
    cout << "| " << left << setw(15) << Client.AccountNumber
         << "| " << left << setw(10) << Client.PinCode
         << "| " << left << setw(20) << Client.Name
         << "| " << left << setw(15) << Client.Phone
         << "| " << right << setw(8) << fixed << setprecision(2) << Client.AccountBalance
         << " |\n";
}

void ShowAllClientCardInfo()
{
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

    for (const sClient &client : vClients)
    {
        ShowCardInfo(client);
    }

    cout << "-----------------------------------------------------------------------------------------------------------\n";
}

bool CheckIfUserFound(const string &AccountNumber)
{
    vector<sClient> Temp = FillAllClients();
    for (const sClient &i : Temp)
    {
        if (i.AccountNumber == AccountNumber)
        {
            return true;
        }
    }
    return false;
}

sClient ReadNewClient()
{
    sClient User;
    cout << "Enter Account Number: ";
    cin >> User.AccountNumber;

    while (CheckIfUserFound(User.AccountNumber))
    {
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
    while (!(cin >> User.AccountBalance))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a numeric balance: ";
    }

    return User;
}

string ConvertLineToRecord(sClient NewUser, string delimiter = "#//#")
{
    string Client = "";
    Client += NewUser.AccountNumber + delimiter;
    Client += NewUser.PinCode + delimiter;
    Client += NewUser.Name + delimiter;
    Client += NewUser.Phone + delimiter;
    Client += to_string(NewUser.AccountBalance);
    return Client;
}

void AddRecordToFile(string Word)
{
    fstream File;
    File.open(ClientsFileName, ios::app);
    if (File.is_open())
    {
        File << Word << "\n";
    }
    cout << "\nClient added successfully!\n";
}

string ReadAccountNumber(string msg)
{
    string AccountNumber = "";
    cout << msg;
    cin >> AccountNumber;
    return AccountNumber;
}

void PrintInfo(const sClient &User)
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "\t\tClient Details\n";
    cout << string(70, '=') << "\n";
    cout << setw(20) << left << "Account Number:" << User.AccountNumber << "\n";
    cout << setw(20) << left << "Pin Code:" << User.PinCode << "\n";
    cout << setw(20) << left << "Full Name:" << User.Name << "\n";
    cout << setw(20) << left << "Phone Number:" << User.Phone << "\n";
    cout << setw(20) << left << "Account Balance:" << fixed << setprecision(2) << User.AccountBalance << "\n";
    cout << string(70, '=') << "\n";
}

void SaveAllClientsToFile(const vector<sClient> &clients)
{
    fstream file(ClientsFileName, ios::out);
    if (file.is_open())
    {
        for (const sClient &client : clients)
        {
            file << ConvertLineToRecord(client) << "\n";
        }
        file.close();
    }
}

void RemoveClientFromRecord(const string &AccountNumber, vector<sClient> &Temp)
{
    char YesOrNo;
    bool Found = false;
    for (size_t i = 0; i < Temp.size(); ++i)
    {
        if (Temp[i].AccountNumber == AccountNumber)
        {
            Found = true;
            PrintInfo(Temp[i]);
            cout << "\nAre you sure you want to delete this client? (Y/N): ";
            cin >> YesOrNo;
            if (tolower(YesOrNo) == 'y')
            {
                Temp.erase(Temp.begin() + i);
                SaveAllClientsToFile(Temp);
                cout << "\nClient deleted successfully.\n";
            }
            else
            {
                cout << "\nDeletion cancelled.\n";
            }
            break;
        }
    }
    if (!Found)
    {
        cout << "There is no client with Account Number [" << AccountNumber << "].\n";
    }
}

void UpdateClientData(const string &AccountNumber, vector<sClient> &Temp)
{
    bool found = false;
    for (size_t i = 0; i < Temp.size(); i++)
    {
        if (AccountNumber == Temp[i].AccountNumber)
        {
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
            while (!(cin >> Temp[i].AccountBalance))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a numeric balance: ";
            }
            SaveAllClientsToFile(Temp);
            cout << "\nClient updated successfully!\n";
            break;
        }
    }
    if (!found)
    {
        cout << "\nNo client found with Account Number [" << AccountNumber << "].\n";
    }
}

bool FindAndPrintClientByAccountNumber(const string &AccountNumber, const vector<sClient> &vClients)
{
    for (const sClient &client : vClients)
    {
        if (client.AccountNumber == AccountNumber)
        {
            PrintInfo(client);
            return true;
        }
    }
    return false;
}

void GoodBye()
{
    const int width = 70;

    cout << "\n"
         << string(width, '=') << "\n";

    string line1 = "[+] Disconnected from Bayt AlMal Bank System...";
    string line2 = "[+] Session terminated securely.";
    string line3 = "[+] Goodbye!";

    cout << setw((width + line1.length()) / 2) << line1 << "\n";
    cout << setw((width + line2.length()) / 2) << line2 << "\n";
    cout << setw((width + line3.length()) / 2) << line3 << "\n";

    cout << string(width, '=') << "\n\n";

    // Creator information section
    string creator = "Created By Ahmad Shdeed";
    cout << setw((width + creator.length()) / 2) << creator << "\n\n";

    // Contact section
    string contactHeader = "[::] Connect With Me";
    cout << setw((width + contactHeader.length()) / 2) << contactHeader << "\n";
    cout << string(width, '-') << "\n";

    string github = "GitHub     : https://github.com/0nyxl";
    string linkedin = "LinkedIn   : https://linkedin.com/in/0nyxl";
    string twitter = "X (Twitter): https://x.com/0nyxl";
    string coffee = "Coffee     : https://buymeacoffee.com/i0nyxl";
    string email = "Email      : i0nyxl@hotmail.com";

    int padding = (width - 45) / 2;

    cout << string(padding, ' ') << github << "\n";
    cout << string(padding, ' ') << linkedin << "\n";
    cout << string(padding, ' ') << twitter << "\n";
    cout << string(padding, ' ') << coffee << "\n";
    cout << string(padding, ' ') << email << "\n";

    cout << string(width, '=') << "\n";
    cout << "\nPress Enter to exit...";

   // cin.ignore(numeric_limits<streamsize>::max(), '\n');
  //  cin.get();
}

// Transaction Update:

void DepositScreen()
{
    ClearScreen();
    cout << string(50, '-') << "\n"
         << setw(35) << "Deposit Screen\n"
         << string(50, '-') << "\n";
}

void DepositUpdate(const string &AccountNumber, vector<sClient> &vClients)
{
    bool found = false;
    for (size_t i = 0; i < vClients.size(); i++)
    {
        if (AccountNumber == vClients[i].AccountNumber)
        {
            found = true;
            PrintInfo(vClients[i]);
            double DepositAmount;
            cout << "Please Enter Deposit Amount: ";
            while (!(cin >> DepositAmount) || DepositAmount <= 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a positive amount: ";
            }
            vClients[i].AccountBalance += DepositAmount;
            SaveAllClientsToFile(vClients);
            cout << "\nDeposit completed successfully.\n\n";
            break;
        }
    }
    if (!found)
    {
        cout << "\nNo client found with Account Number [" << AccountNumber << "].\n";
    }
}

void WithdrawScreen()
{
    ClearScreen();
    cout << string(50, '-') << "\n"
         << setw(35) << "Withdraw Screen\n"
         << string(50, '-') << "\n";
}

void WithdrawUpdate(const string &AccountNumber, vector<sClient> &vClients)
{
    bool found = false;
    for (size_t i = 0; i < vClients.size(); i++)
    {
        if (AccountNumber == vClients[i].AccountNumber)
        {
            found = true;
            PrintInfo(vClients[i]);
            double WithdrawAmount;
            cout << "Please Enter Withdraw Amount: ";
            while (!(cin >> WithdrawAmount) || WithdrawAmount <= 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter amount: ";
            }
            vClients[i].AccountBalance -= WithdrawAmount;
            SaveAllClientsToFile(vClients);
            cout << "\nWithdraw completed successfully.\n\n";
            break;
        }
    }
    if (!found)
    {
        cout << "\nNo client found with Account Number [" << AccountNumber << "].\n";
    }
}

double TotalBalanceFunction(vector<sClient> &vClients)
{
    double Total = 0.0;
    for (const sClient &i : vClients)
    {
        Total += i.AccountBalance;
    }
    return Total;
}

void TotllBalanceMSG(vector<sClient> &vClients)
{
    ClearScreen();
    cout << string(56, '-') << "\n";
    cout << setw(35) << "Total Balance Screen\n";
    cout << string(56, '-') << "\n";
    double totalBalance = TotalBalanceFunction(vClients);
    cout << "\nOur bank currently holds a total balance of [" << totalBalance << "] USD.\n";
    cout << string(56, '-') << "\n";
}

void PrintTransactionsMenu()
{
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

short ReadTransactionsMenu()
{
    short number = 0;
    cin >> number;
    while (cin.fail() || number < 1 || number > 4)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Make Sure You Select From [1] to [4]: ";
        cin >> number;
    }
    return number;
}

enTransactions CastNumber(short number)
{
    return static_cast<enTransactions>(number);
}

// Manage Users Update :

sUser FillOneUser(string line, string delimiter = "#//#")
{

    vector<string> vUserData = Split_Function(line, delimiter);
    sUser Temp;

    Temp.username = vUserData[0];
    Temp.password = vUserData[1];
    Temp.permission = stod(vUserData[2]);
    return Temp;
}

vector<sUser> FILLALLUSERS()
{
    vector<sUser> vUSERSDATA;
    fstream file;
    string word;

    file.open(UserDataFile, ios::in);
    if (file.is_open())
    {
        sUser temp;
        while (getline(file, word))
        {
            if (!word.empty())
            {
                temp = FillOneUser(word);
                vUSERSDATA.push_back(temp);
            }
        }
    }
    file.close();

    return vUSERSDATA;
}

void ShowCardInfoUsers(const sUser &User)
{
    cout << "| " << left << setw(10) << User.username
         << "| " << left << setw(10) << User.password
         << "| " << left << setw(10) << User.permission
         << " |\n";
}

void LISTUSERSFUNCTION()
{

    vector<sUser> vUsersData = FILLALLUSERS();
    cout << "\n\n\t\t\tUsers List (" << vUsersData.size() << ") User(s).\n";
    cout << "--------------------------------------\n";
    cout << "| " << left << setw(10) << "Username"
         << "| " << left << setw(10) << "Password"
         << "| " << left << setw(10) << "Permission"
         << " |\n";
    cout << "--------------------------------------\n";

    for (const sUser &User : vUsersData)
    {
        ShowCardInfoUsers(User);
    }

    cout << "--------------------------------------\n";
}

string ConvertUsersDataToWord(sUser User, string Delemiter = "#//#")
{

    string UserData = "";
    UserData += User.username + Delemiter;
    UserData += User.password + Delemiter;
    UserData += to_string(User.permission) + Delemiter;
    return UserData;
}

void SaveAlLUsersToFile(const vector<sUser> &Users)
{
    fstream file(UserDataFile, ios::out);
    if (file.is_open())
    {
        for (const sUser &iUser : Users)
        {
            file << ConvertUsersDataToWord(iUser) << "\n";
        }
        file.close();
    }
}

void ADDNEWUSERS()
{

    vector<sUser> veUsers = FILLALLUSERS();
    sUser NewUser;

    while (true)
    {
        cout << "Enter username: ";
        cin >> NewUser.username;

        bool taken = false;

        for (const sUser &i : veUsers)
        {
            if (i.username == NewUser.username)
            {
                cout << "Username already exists. Enter another username.\n";
                taken = true;
                break;
            }
        }

        if (!taken)
            break;
    }

    cout << "Enter Password: ";
    cin >> NewUser.password;

    cout << "\nGive full access? (1 = yes, 0 = no): ";
    int FullPremissions;
    cin >> FullPremissions;

    int Premis = 0;

    if (FullPremissions)
    {
        Premis = enMainMenuePermissions::eAll;
    }
    else
    {
        int x;
        cout << "List Clients? (1/0): ";
        cin >> x;
        if (x)
            Premis |= enMainMenuePermissions::pListClients;

        cout << "Add New Client? (1/0): ";
        cin >> x;
        if (x)
            Premis |= enMainMenuePermissions::pAddNewClient;

        cout << "Delete Client? (1/0): ";
        cin >> x;
        if (x)
            Premis |= enMainMenuePermissions::pDeleteClient;

        cout << "Update Clients? (1/0): ";
        cin >> x;
        if (x)
            Premis |= enMainMenuePermissions::pUpdateClients;

        cout << "Find Client? (1/0): ";
        cin >> x;
        if (x)
            Premis |= enMainMenuePermissions::pFindClient;

        cout << "Transactions? (1/0): ";
        cin >> x;
        if (x)
            Premis |= enMainMenuePermissions::pTranactions;

        cout << "Manage Users? (1/0): ";
        cin >> x;
        if (x)
            Premis |= enMainMenuePermissions::pManageUsers;
    }

    NewUser.permission = Premis;

    veUsers.push_back(NewUser);

    SaveAlLUsersToFile(veUsers);

    cout << "\nUser created successfully.\n";
}

void PrintUserInfo(const sUser &User)
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "\t\tUser Details\n";
    cout << string(70, '=') << "\n";
    cout << setw(20) << left << "Username :" << User.username << "\n";
    cout << setw(20) << left << "Password :" << User.password << "\n";
    cout << setw(20) << left << "Premissions :" << User.permission << "\n";

    cout << string(70, '=') << "\n";
}

bool FOUNDUSER()
{

    vector<sUser> veUsers = FILLALLUSERS();

    string username;
    cout << "Enter Username: ";
    cin >> username;

    for (const sUser &i : veUsers)
    {
        if (i.username == username)
        {
            PrintUserInfo(i);
            return true;
        }
    }
    return false;
}

bool DELETEUSER()
{

    vector<sUser> veUsers = FILLALLUSERS();
    string username;
    cout << "Enter username: ";
    cin >> username;

    for (size_t i = 0; i < veUsers.size(); i++)
    {
        if (veUsers[i].username == username)
        {

            PrintUserInfo(veUsers[i]);

            char confirm;
            cout << "\nAre you sure you want to delete this user? (Y/N): ";
            cin >> confirm;

            if (tolower(confirm) == 'y')
            {
                veUsers.erase(veUsers.begin() + i); 
                SaveAlLUsersToFile(veUsers);       
                cout << "\nUser deleted successfully.\n";
                return true;
            }
            else
            {
                cout << "\nDeletion canceled.\n";
                return false;
            }
        }
    }

    cout << "\nUser not found.\n";
    return false;
}

bool Updateuserr() {
    vector<sUser> veUsers = FILLALLUSERS();
    string username;
    cout << "Enter username: ";
    cin >> username;

    for (sUser &i : veUsers) {
        if (i.username == username) {
            PrintUserInfo(i);
            cout << "\nUser Found. Do You Want to Update User? (Y/n): ";
            char yn;
            cin >> yn;

            if (tolower(yn) == 'y') {
                cout << "Enter New Password: ";
                cin >> i.password;


                cout << "\nGive full access? (1 = yes, 0 = no): ";
                int fullPermissions;
                cin >> fullPermissions;

                int permissions = 0;
                if (fullPermissions) {
                    permissions = enMainMenuePermissions::eAll;
                } else {
                    int x;
                    cout << "List Clients? (1/0): "; cin >> x; if (x) permissions |= enMainMenuePermissions::pListClients;
                    cout << "Add New Client? (1/0): "; cin >> x; if (x) permissions |= enMainMenuePermissions::pAddNewClient;
                    cout << "Delete Client? (1/0): "; cin >> x; if (x) permissions |= enMainMenuePermissions::pDeleteClient;
                    cout << "Update Clients? (1/0): "; cin >> x; if (x) permissions |= enMainMenuePermissions::pUpdateClients;
                    cout << "Find Client? (1/0): "; cin >> x; if (x) permissions |= enMainMenuePermissions::pFindClient;
                    cout << "Transactions? (1/0): "; cin >> x; if (x) permissions |= enMainMenuePermissions::pTranactions;
                    cout << "Manage Users? (1/0): "; cin >> x; if (x) permissions |= enMainMenuePermissions::pManageUsers;
                }

                i.permission = permissions;

                
                SaveAlLUsersToFile(veUsers);
                cout << "\nUser Updated Successfully!\n";
                return true;
            }
            return false;
        }
    }

    cout << "\nUser not found!\n";
    return false; 
}




void AccessDenied(){
 ClearScreen();
cout << "\n====================================\n";
cout << "         ACCESS  DENIED\n";
cout << "====================================\n";
cout << "You don't have permission to use this feature.\n";
cout << "Please contact an administrator.\n\n";
}

bool Login() {
    vector<sUser> veUsers = FILLALLUSERS();
    string username, password;

    while (true) {
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        for (const sUser &i : veUsers) {
            if (i.username == username && i.password == password) {
                CurrentUser = i;  
                return true;      
            }
        }

        cout << "Invalid username or password. Try again.\n";
    }
    return false;
}

void ManageUsersMenu()
{
    cout << string(50, '=') << "\n";
    cout << "\tManage Users Menu\n";
    cout << string(50, '=') << "\n";
    cout << "[1] List Users\n[2] Add New User\n[3] Delete User\n[4] Update User\n[5] Find User\n[6] Back to Main Menu\n";
    cout << string(50, '=') << "\n";
    int choice;
    cout<<"Enter From [1-6]: ";
    cin >> choice;

    enManageUsers selectedOption = static_cast<enManageUsers>(choice);

    switch (selectedOption)
    {
        case ListUsers:
            ClearScreen();
            LISTUSERSFUNCTION();
            break;
        case AddNewUser:
            ClearScreen();
            ADDNEWUSERS();
            break;
        case DeleteUser:
            ClearScreen();
            DELETEUSER();
            break;
        case UpdateUser:
            ClearScreen();
            Updateuserr();
            break;
        case FindUser:
            ClearScreen();
            FOUNDUSER();
            break;
        case MainMenuu:
            ClearScreen();
            BackToMainMenu();
            break; 
        default:
            cout << "Invalid option\n";
            break;
    }

    BackToMainMenu();
}




void TransactionsMainMenu_Switch(enTransactions number)
{
    switch (number)
    {
    case Deposit:
    {
        DepositScreen();
        vector<sClient> vClients = FillAllClients();
        string DepositAcoount = ReadAccountNumber("Please Enter Account Number: ");
        ClearScreen();
        DepositUpdate(DepositAcoount, vClients);
        BackToMainMenu();
        break;
    }
    case Withdraw:
    {
        ClearScreen();
        WithdrawScreen();
        vector<sClient> vClients = FillAllClients();
        string DepositAcoount = ReadAccountNumber("Please Enter Account Number: ");
        ClearScreen();
        WithdrawUpdate(DepositAcoount, vClients);
        BackToMainMenu();
        break;
    }
    case TotalBalance:
    {
        ClearScreen();
        vector<sClient> vClients = FillAllClients();
        TotllBalanceMSG(vClients);
        BackToMainMenu();
        break;
    }
    case MainMenu:
    {
        ClearScreen();
        BackToMainMenu();
        break;
    }
    default:
        break;
    }
}


void ClientSelect(enMainMenu choice)
{
    switch (choice)
    {
    case ShowClientList: {
        if (!(CurrentUser.permission & pListClients)) { AccessDenied(); BackToMainMenu();break; }
        ClearScreen();
        ShowAllClientCardInfo();
        BackToMainMenu();
        break;
    }

    case AddNewClient: {
        if (!(CurrentUser.permission & pAddNewClient)) { AccessDenied(); BackToMainMenu(); break; }
        ClearScreen();
        sClient User = ReadNewClient();
        string Word = ConvertLineToRecord(User);
        AddRecordToFile(Word);
        BackToMainMenu();
        break;
    }

    case DeleteClient: {
        if (!(CurrentUser.permission & pDeleteClient)) { AccessDenied(); BackToMainMenu(); break; }
        ClearScreen();
        vector<sClient> vClients = FillAllClients();
        string AccountNumberForDelete = ReadAccountNumber("Enter Account Number: ");
        RemoveClientFromRecord(AccountNumberForDelete, vClients);
        BackToMainMenu();
        break;
    }

    case UpdateClient: {
        if (!(CurrentUser.permission & pUpdateClients)) { AccessDenied(); BackToMainMenu(); break; }
        ClearScreen();
        vector<sClient> vClients = FillAllClients();
        string AccountUpdate = ReadAccountNumber("Enter Account Number: ");
        UpdateClientData(AccountUpdate, vClients);
        BackToMainMenu();
        break;
    }

    case FindClient: {
        if (!(CurrentUser.permission & pFindClient)) { AccessDenied(); BackToMainMenu(); break; }
        ClearScreen();
        vector<sClient> vClients = FillAllClients();
        string FindAccount = ReadAccountNumber("Enter Account Number: ");
        if (!FindAndPrintClientByAccountNumber(FindAccount, vClients)) {
            cout << "\nNo client found with Account Number [" << FindAccount << "].\n";
        }
        BackToMainMenu();
        break;
    }

    case transactions: {
        if (!(CurrentUser.permission & pTranactions)) { AccessDenied(); BackToMainMenu(); break; }
        ClearScreen();
        PrintTransactionsMenu();
        int number = ReadTransactionsMenu();
        enTransactions s = CastNumber(number);
        TransactionsMainMenu_Switch(s);
        break;
    }

    case Manageusers: {
        if (!(CurrentUser.permission & pManageUsers)) { AccessDenied(); BackToMainMenu(); break; }
        ClearScreen();
        ManageUsersMenu();
        BackToMainMenu();
        break;
    }

    case Logout: {
        ClearScreen();
        GoodBye();
        cout<<endl;
        Start();
        break;
    }

    default: {
        cout << "Invalid selection!\n";
        BackToMainMenu();
        break;
    }
    }
}


void Print_Main_Menu()
{
    cout << string(70, '=') << "\n";
    cout << "\t\tWelcome To Bayt AlMal Bank System\n";
    cout << string(70, '=') << "\n";
    cout << "[1]: Show Client List\n"
         << "[2]: Add New Client\n"
         << "[3]: Delete Client\n"
         << "[4]: Update Client Info\n"
         << "[5]: Find Client\n"
         << "[6]: Transactions\n"
         << "[7]: Manage User\n"
         << "[8]: Logout\n";
    cout << string(70, '=') << "\n";
    short int Choice = ChooseNumber();
    enMainMenu Selected = NumberSelected(Choice);
    ClientSelect(Selected);
}

void BackToMainMenu()
{
    Pause();
    ClearScreen();
    Print_Main_Menu();
}


void Start(){
    
    if(Login()){
    ClearScreen();
     Print_Main_Menu();
    }
}

int main()
{
    
    Start();

    return 0;
}
