/* A terminal C++ login program which allows a user to create new accounts, login and hash passwords + usernames
   This was a tutorial made by codegopher on youtube */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

class LoginManager
{
 public:

    LoginManager()
    {
        accessGranted = 0;
    }
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

    void addUserFunction()
    {
        cout << "Enter new username:" << endl;
        cin >> username;
        cout << "Enter new password:" << endl;
        cin >> password;
        addUser(username, password);
    }

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

    // Encrypt the passwords and usernames
    long long encrypt(int p_letter)
    {
        return powf(p_letter, 5) * 4 - 14;
    }
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