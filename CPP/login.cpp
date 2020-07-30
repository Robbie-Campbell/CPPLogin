/*
A terminal C++ login program which allows a user to create new accounts, login and hash passwords + usernames
   This was a tutorial made by codegopher on youtube
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

// Create the login class
class LoginManager
{
    // Visible variables
 public:
    LoginManager()
    {
        accessGranted = 0;
    }

    // The main part of the login system - Takes in all login input and checks against the hashed information.
    void login()
    {
        cout << "Please enter username \nUsername:";
        cin >> userNameAttempt;

        int usrID = checkFile(userNameAttempt, "users.dat");
        if(usrID != 0)
        {
            cout << "Password:";
            cin >> passwordAttempt;

            int pwdID = checkFile(passwordAttempt, "pswds.dat");
            if(usrID == pwdID)
            {
                cout << "Logged in successfully" << endl;;
                cin.get();

            }
            else
            {
                cout << "Incorrect password" << endl;
                login();
            }
        }
        else
        {
            cout << "Username does not exist." << endl;
            login();
        }
    }

    // The function used to store user information into the data files/ also make sure that the username is unique.
    void addUser(const string username, const string password)
    {
        if(checkFile(username, "users.dat") != 0)
        {
            cout << "That username is not available." << endl;
            return;
        }

        int id = 1 + getLastID();
        saveFile(username, "users.dat", id);
        saveFile(password, "pswds.dat", id);
        cout << "Congratulations! An account has been made for: " << username << endl;
    }

    // The function to add a new user through the terminal.
    void addUserFunction()
    {
        cout << "Enter new username:" << endl;
        cin >> username;
        cout << "Enter new password:" << endl;
        cin >> password;
        addUser(username, password);
    }

    // Checks the last ID has from the data files and increments 1 to it to store new user information
    int getLastID()
    {
        fstream file;
        file.open("users.dat", ios::in);
        file.seekg(0, ios::end);

        if(file.tellg() == -1)
            return 0;

        string s;

        for(int i = -1; s.find("#") == string::npos; i--)
        {
            file.seekg(i, ios::end);
            file >> s;
        }

        file.close();
        s.erase(0, 4);

        int id;
        istringstream(s) >> id;

        return id;
    }

    // This function passes in 2 parameters to check if there is any instances of the username in the file.
    int checkFile(string attempt, const char* p_fileName)
    {
        string line;
        fstream file;

        string currentChar;
        long long eChar;

        file.open(p_fileName, ios::in);
        while(1)
        {
            file >> currentChar;
            if(currentChar.find("#ID:") != string::npos)
            {
                if(attempt == line)
                {
                    file.close();
                    currentChar.erase(0, 4);
                    int id;
                    istringstream(currentChar) >> id;
                    return id;
                }
                else
                {
                    line.erase(line.begin(), line.end());
                }
            }
            else
            {
                istringstream(currentChar) >> eChar;
                line += (char)decrypt(eChar);
                currentChar = "";
            }

            if(file.peek() == EOF)
            {
                file.close();
                return 0;
            }
        }
    }

    // The function which defines the parameters needed to store a user and save it to the data files.
    void saveFile(string p_line, const char* p_fileName, const int& id)
    {
        fstream file;
        file.open(p_fileName, ios::app);
        file.seekg(0, ios::end);

        if(file.tellg() != 0)
            file << "\n";

        file.seekg(0, ios::beg);

        for(int i = 0; i < p_line.length(); i++)
        {
            file << encrypt(p_line[i]);
            file << "\n";
        }

        file << "#ID:" << id;
        file.close();
    }

    // Encrypt the password and username
    long long encrypt(int p_letter)
    {
        return powf(p_letter, 5) * 4 - 14;
    }

    // Decrypts the password and username
    int decrypt(long long p_letter)
    {
        return powf((p_letter + 14) / 4, 1/5.f);
    }

// Declare private variables
private:
    string userNameAttempt;
    string passwordAttempt;
    bool accessGranted;
    string username;
    string password;
};

int main()
{
    // Declare input variable
    string inputType;

    // Decide whether user wants ro register or login
    LoginManager app;
    cout << "Do you need to add a new user or login? (add/ login):" << endl;
    cin >> inputType;
    if (inputType == "add")
    {
        app.addUserFunction();
    }
    else if (inputType == "login")
    {
        app.login();
        cin.get();
    }
}