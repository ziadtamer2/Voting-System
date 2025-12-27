#include <iostream>
#include <vector>
#include <string>
#include <Limits>

using namespace std;

/* ---------- Forward Declaration ---------- */
class VotingSystem;

/* ---------- Enums ---------- */
enum class ElectionStatus
{
    CREATED,
    OPENED,
    CLOSED
};

/* ---------- Election ---------- */
class Election
{
private:
    int electionId;
    string title;
    string description;
    ElectionStatus status;
    vector<int> candidateIds; // ✅ candidates inside election

public:
    Election(int id, string t, string d)
        : electionId(id), title(t), description(d),
          status(ElectionStatus::CREATED) {}

    int getElectionId() const { return electionId; }
    ElectionStatus getStatus() const { return status; }

    void open() { status = ElectionStatus::OPENED; }
    void close() { status = ElectionStatus::CLOSED; }

    bool isOpen() const { return status == ElectionStatus::OPENED; }

    void addCandidate(int candidateId)
    {
        candidateIds.push_back(candidateId);
    }

    void removeCandidate(int candidateId)
    {
        for (auto i = candidateIds.begin(); i != candidateIds.end(); ++i)
        {
            if (*i == candidateId)
            {
                candidateIds.erase(i);
                return;
            }
        }
    }

    vector<int> &getCandidates()
    {
        return candidateIds;
    }
    string getTitle() const { return title; }
    string getDescription() const { return description; }

    void setTitle(const string &newTitle)
    { // for updating election
        title = newTitle;
    }

    void setDescription(const string &newDescription)
    { // for updating election
        description = newDescription;
    }
};

/* ---------- User ---------- */
class User
{
protected:
    int userId;
    string username;
    string email;
    string password; // ✅ added
    bool isBanned;
    VotingSystem *system; // ✅ system reference

public:
    User(int id, string uname, string mail, string pass, VotingSystem *sys)
        : userId(id), username(uname), email(mail),
          password(pass), isBanned(false), system(sys) {}

    virtual ~User() {}

    virtual string getRole() const = 0;

    virtual void login() {}
    virtual void registerUser() {}
    virtual void logout() {}

    bool getBanStatus() const { return isBanned; }
    void ban() { isBanned = true; }

    void viewElections(); // implemented after VotingSystem

    int getUserId() const { return userId; }
    string getPassword() const { return password; }

    string getUsername() const { return username; }
    string getEmail() const { return email; }
};

/* ---------- Guest ---------- */ // tamer, mo3tasam
class Guest
{
private:
    VotingSystem *system;

public:
    Guest(VotingSystem *sys) : system(sys) {}

    void viewElections();

    void viewElectionDetails(int electionId);

    void viewCandidates(int electionId);
    void viewVotingRules()
    {
        cout << "\n===== Voting Rules =====\n";
        cout << "1. Each voter can vote only once per election.\n";
        cout << "2. Voting is allowed only when the election is OPEN.\n";
        cout << "3. Votes cannot be changed after submission.\n";
        cout << "4. Banned users are not allowed to vote.\n";
        cout << "5. Candidates cannot vote in elections they participate in.\n";
        cout << "========================\n";
    }
};

/* ---------- Voter ---------- */
class Voter : public User
{
public:
    Voter(int id, string uname, string mail, string pass, VotingSystem *sys)
        : User(id, uname, mail, pass, sys) {}

    string getRole() const override { return "Voter"; }

    void vote(int electionId, int candidateId) {}
    bool hasVoted(int electionId) const { return false; }
    void viewVotingStatus() {}

    void login() override
    {
        cout << "Enter username: ";
        cin >> username;

        cout << "Enter password: ";
        cin >> password;

        // authentication logic will be added later
    }
};

/* ---------- Candidate ---------- */
class Candidate : public User
{
private:
    string profileInfo;

public:
    Candidate(int id, string uname, string mail,
              string pass, string profile, VotingSystem *sys)
        : User(id, uname, mail, pass, sys),
          profileInfo(profile) {}

    string getRole() const override { return "Candidate"; }
    void login() override;
    void registerUser() override;
    void logout() override;

    void viewMyElections();
    void viewVoteCount(int electionId);
};

/* ---------- Admin ---------- */
class Admin : public User
{
public:
    Admin(int id, string uname, string mail, string pass, VotingSystem *sys)
        : User(id, uname, mail, pass, sys) {}

    string getRole() const override { return "Admin"; }

    int createElection();

    void updateElection(int electionId);

    void openElection(int electionId);

    void closeElection(int electionId);

    void addCandidate(int electionId, int candidateId);
    void removeCandidate(int electionId, int candidateId);

    void viewVoters() {}
    void banVoter(int voterId) {}
    void viewResults(int electionId) {}
};

/* ---------- Vote ---------- */
class Vote
{
private:
    int voteId;
    int electionId;
    int voterId;
    int candidateId;

public:
    Vote(int vId, int eId, int vrId, int cId)
        : voteId(vId), electionId(eId),
          voterId(vrId), candidateId(cId) {}

    int getVoterId() const { return voterId; }
    int getElectionId() const { return electionId; }
};

/* ---------- VotingSystem ---------- */
class VotingSystem
{
private:
    vector<User *> users;
    vector<Election> elections;
    vector<Vote> votes;

public:
    vector<Election> &getElections() { return elections; }
    vector<User *> &getUsers() { return users; }
    vector<Vote> &getVotes() { return votes; }

    void fillDate()
    {
        /* ----------- Elections ----------- */
        elections.push_back(
            Election(1, "Student Union Election", "Choose the student union president"));

        elections.push_back(
            Election(2, "Club Leader Election", "Choose the club leader"));

        /* ----------- Candidates ----------- */
        users.push_back(new Candidate(101, "cand1", "c1@mail.com", "123", "Profile 1", this));
        users.push_back(new Candidate(102, "cand2", "c2@mail.com", "123", "Profile 2", this));
        users.push_back(new Candidate(103, "cand3", "c3@mail.com", "123", "Profile 3", this));
        users.push_back(new Candidate(104, "cand4", "c4@mail.com", "123", "Profile 4", this));
        users.push_back(new Candidate(105, "cand5", "c5@mail.com", "123", "Profile 5", this));

        // Election 1 → 2 candidates
        elections[0].addCandidate(101);
        elections[0].addCandidate(102);

        // Election 2 → 3 candidates
        elections[1].addCandidate(103);
        elections[1].addCandidate(104);
        elections[1].addCandidate(105);

        /* ----------- Voters (10) ----------- */
        users.push_back(new Voter(1, "voter1", "v1@mail.com", "123", this));
        users.push_back(new Voter(2, "voter2", "v2@mail.com", "123", this));
        users.push_back(new Voter(3, "voter3", "v3@mail.com", "123", this));
        users.push_back(new Voter(4, "voter4", "v4@mail.com", "123", this));
        users.push_back(new Voter(5, "voter5", "v5@mail.com", "123", this));
        users.push_back(new Voter(6, "voter6", "v6@mail.com", "123", this));
        users.push_back(new Voter(7, "voter7", "v7@mail.com", "123", this));
        users.push_back(new Voter(8, "voter8", "v8@mail.com", "123", this));
        users.push_back(new Voter(9, "voter9", "v9@mail.com", "123", this));
        users.push_back(new Voter(10, "voter10", "v10@mail.com", "123", this));

        /* ----------- Admins (10) ----------- */
        users.push_back(new Admin(1001, "admin1", "admin1@mail.com", "123", this));
        users.push_back(new Admin(1002, "admin2", "admin2@mail.com", "123", this));
        users.push_back(new Admin(1003, "admin3", "admin3@mail.com", "123", this));
        users.push_back(new Admin(1004, "admin4", "admin4@mail.com", "123", this));
        users.push_back(new Admin(1005, "admin5", "admin5@mail.com", "123", this));
        users.push_back(new Admin(1006, "admin6", "admin6@mail.com", "123", this));
        users.push_back(new Admin(1007, "admin7", "admin7@mail.com", "123", this));
        users.push_back(new Admin(1008, "admin8", "admin8@mail.com", "123", this));
        users.push_back(new Admin(1009, "admin9", "admin9@mail.com", "123", this));
        users.push_back(new Admin(1010, "admin10", "admin10@mail.com", "123", this));

        /* ----------- Votes (5) ----------- */
        votes.push_back(Vote(1, 1, 1, 101)); // voter1 → election1 → candidate101
        votes.push_back(Vote(2, 1, 2, 102));
        votes.push_back(Vote(3, 2, 3, 103));
        votes.push_back(Vote(4, 2, 4, 104));
        votes.push_back(Vote(5, 2, 5, 105));
    }
    void run() {}

    void guestMenu() {}
    void voterMenu(Voter *voter) {}
    void adminMenu(Admin *admin) {}
};

//////////////////////////////

/* ---------- Test Cases ---------- */
void TestCandidate(VotingSystem &system);

/* ---------- User method implementation ---------- */
void User::viewElections()
{
    for (const Election &e : system->getElections())
    {
        cout << "Id: " << e.getElectionId() << endl;
        cout << "Status: " << e.isOpen() << endl;
    }
}

///////////////////////////////
/*Admin methods implementation */
void Admin::addCandidate(int electionId, int candidateId)
{
    Election *targetElection = nullptr;
    Candidate *targetCandidate = nullptr;

    // 1 Find election
    for (Election &e : system->getElections())
    {
        if (e.getElectionId() == electionId)
        {
            targetElection = &e;
            break;
        }
    }

    if (!targetElection)
    {
        cout << "Election with ID " << electionId << " not found.\n";
        return;
    }

    // 2 Find candidate in users
    for (User *u : system->getUsers())
    {
        if (u->getUserId() == candidateId)
        {
            targetCandidate = dynamic_cast<Candidate *>(u);
            break;
        }
    }

    if (!targetCandidate)
    {
        cout << "User is not a valid candidate or does not exist.\n";
        return;
    }

    // 3  Check if candidate already added
    for (int cid : targetElection->getCandidates())
    {
        if (cid == candidateId)
        {
            cout << "Candidate already added to this election.\n";
            return;
        }
    }

    //  Add candidate
    targetElection->addCandidate(candidateId);
    cout << "Candidate " << candidateId
         << " added to Election " << electionId << " successfully.\n";
}

void Admin::removeCandidate(int electionId, int candidateId)
{

    Election *targetElection = nullptr;
    Candidate *targetCandidate = nullptr;

    // 1 Find election
    for (Election &e : system->getElections())
    {
        if (e.getElectionId() == electionId)
        {
            targetElection = &e;
            break;
        }
    }

    if (!targetElection)
    {
        cout << "Election with ID " << electionId << " not found.\n";
        return;
    }

    // 2 Find candidate in users
    for (User *u : system->getUsers())
    {
        if (u->getUserId() == candidateId)
        {
            targetCandidate = dynamic_cast<Candidate *>(u);
            break;
        }
    }

    if (!targetCandidate)
    {
        cout << "User is not a valid candidate or does not exist.\n";
        return;
    }

    // 3 Remove candidate
    targetElection->removeCandidate(candidateId);
    cout << "Candidate " << candidateId
         << " deleted  from Election " << electionId << " successfully.\n";
}

int Admin::createElection()
{
    int id;
    cout << "Enter Election ID: ";
    cin >> id;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // to ignore leftover newline or any extra input

    for (const Election &e : system->getElections())
    {
        if (e.getElectionId() == id)
        {
            cout << "Election ID already exists.\n";
            return -1;
        }
    }
    string title, description;

    cout << "Enter Election Title: ";
    getline(cin, title);

    cout << "Enter Election Description: ";
    getline(cin, description);

    system->getElections().emplace_back(id, title, description); // this will call Election constructor
    cout << "Election has been created successfully.\n";
    return id;
} // completed

void Admin::updateElection(int electionId)
{

    for (Election &e : system->getElections())
    {
        if (e.getElectionId() == electionId)
        {
            string newTitle, newDescription;
            cout << "Current title: " << e.getTitle() << endl;
            cout << "Enter new title (or press Enter to keep current): ";

            getline(cin, newTitle);
            if (!newTitle.empty())
            {
                e.setTitle(newTitle); // updating via setter more safe than direct access via friendship
            }

            cout << "Current description: " << e.getDescription() << endl;
            cout << "Enter new description (or press Enter to keep current): ";
            getline(cin, newDescription);
            if (!newDescription.empty())
            {
                e.setDescription(newDescription); // updating via setter more safe than direct access via friendship
            }

            cout << "Election has been updated successfully." << endl;
            return;
        }
    }
    cout << "Election with ID " << electionId << " not found." << endl;
}
void Admin::openElection(int electionId)
{
    for (Election &e : system->getElections())
    {
        if (e.getElectionId() == electionId)
        {
            if (e.getStatus() == ElectionStatus::CREATED)
            {
                e.open();
                cout << "Election " << electionId << " is now open for voting." << endl;
            }
            else
            {
                cout << "Election " << electionId << " is already " << (e.getStatus() == ElectionStatus::OPENED ? "open" : "closed") << "." << endl;
            }
            return;
        }
    }
    cout << "Election with ID " << electionId << " not found." << endl;
}

void Admin::closeElection(int electionId)
{
    for (Election &e : system->getElections())
    {
        if (e.getElectionId() == electionId)
        {
            if (e.getStatus() == ElectionStatus::OPENED)
            {
                e.close();
                cout << "Election " << electionId << " has been closed successfully." << endl;
            }
            else
            {
                cout << "Election " << electionId << " is already " << (e.getStatus() == ElectionStatus::CLOSED ? "closed" : "not yet open") << "." << endl;
            }
            return;
        }
    }
    cout << "Election with ID " << electionId << " not found." << endl;
}
//////////////////////////////////////
/*Guest  methods implementation*/
void Guest::viewElections()
{

    cout << "===== Available Elections =====\n";
    for (const Election &e : system->getElections())
    {
        cout << "ID: " << e.getElectionId()
             << " | Title: " << e.getTitle()
             << " | Status: ";

        if (e.getStatus() == ElectionStatus::CREATED)
            cout << "Created";
        else if (e.getStatus() == ElectionStatus::OPENED)
            cout << "Opened";
        else
            cout << "Closed";

        cout << endl;
    }
}
void Guest::viewElectionDetails(int electionId)
{
    for (const Election &e : system->getElections())
    {
        if (e.getElectionId() == electionId)
        {
            cout << "===== Election Details =====\n";
            cout << "Title: " << e.getTitle() << endl;
            cout << "Description: " << e.getDescription() << endl;
            cout << "Status: ";

            if (e.getStatus() == ElectionStatus::CREATED)
                cout << "Created";
            else if (e.getStatus() == ElectionStatus::OPENED)
                cout << "Opened";
            else
                cout << "Closed";

            cout << endl;
            return;
        }

        cout << "Election not found.\n";
    }
}

// void Guest::viewCandidates(int electionId) // tamer , mo3tasem
//     {
//         bool electionFound = false;
//
//         for (const Election& e : system->getElections())
//         {
//             if (e.getElectionId() == electionId)
//             {
//                 electionFound = true;
//                 cout << "Candidates for Election: " << e.getTitle() << "\n";
//
//                 for (int candidateId : e.getCandidates())
//                 {
//                     for (User* u : system->getUsers())
//                     {
//                         if (u->getUserId() == candidateId &&
//                             u->getRole() == "Candidate")
//                         {
//                             cout << "- Candidate ID: " << u->getUserId()
//                                  << ", Username: " << u->getUsername()
//                                  << ", Email: " << u->getEmail() << endl;
//                         }
//                     }
//                 }
//                 return;
//             }
//         }
//
//         if (!electionFound)
//         {
//             cout << "Election with ID " << electionId << " not found.\n";
//         }
//     }
///////////////////////////////////////////
/*---  candidate methods implementation */
void Candidate::login()
{
    bool validInput = false;
    string inputUsername, inputPassword;
    do
    {
        cout << "Enter username: ";
        cin >> inputUsername;

        cout << "Enter password: ";
        cin >> inputPassword;

        for (User *user : system->getUsers())
        {
            if (user->getUsername() == inputUsername &&
                user->getPassword() == inputPassword)
            {
                username = inputUsername;
                password = inputPassword;
                cout << "Login successful!" << endl;
                validInput = true;
                break;
            }
        }
        if (!validInput)
        {
            cout << "Invalid username or password. Please try again." << endl;
        }

    } while (!validInput);

    // Navigation to candidate menu will be added later
}

void Candidate::registerUser()
{
    bool exists = false;
    string inputUsername, inputEmail, inputPassword, inputProfileInfo;

    do
    {
        exists = false;
        cout << "Enter username: ";
        cin >> inputUsername;

        if (inputUsername.empty())
        {
            cout << "Username cannot be empty." << endl;
            continue;
        }
        for (User *user : system->getUsers())
        {
            if (user->getUsername() == inputUsername)
            {
                cout << "Username already exists." << endl;
                exists = true;
                break;
            }
        }

        if (exists)
            continue;

        cout << "Enter email: ";
        cin >> inputEmail;

        if (inputEmail.empty())
        {
            cout << "Email cannot be empty." << endl;
            continue;
        }
        for (User *user : system->getUsers())
        {
            if (user->getEmail() == inputEmail)
            {
                cout << "Email already registered." << endl;
                exists = true;
                break;
            }
        }
        if (exists)
            continue;

        cout << "Enter password: ";
        cin >> inputPassword;
        if (inputPassword.empty())
        {
            cout << "Password cannot be empty." << endl;
            continue;
        }

        cout << "Enter profile info: ";
        cin.ignore(); // to ignore the newline character left in the buffer
        getline(cin, inputProfileInfo);
        if (inputProfileInfo.empty())
        {
            cout << "Profile info cannot be empty." << endl;
            continue;
        }

        exists = false;

        // Navigation to candidate menu will be added later
        break; // exit loop on successful registration

    } while (true);

    // Registration logic (e.g., saving to database) will be added later
    cout << "Registration successful!" << endl;
    username = inputUsername;
    email = inputEmail;
    password = inputPassword;
    profileInfo = inputProfileInfo;
    system->getUsers().push_back(this); // add candidate to list of users
}

void Candidate::logout()
{
    cout << "Logged out successfully." << endl;
    // Navigation to main menu will be added later
}

void Candidate::viewMyElections()
{
    cout << "Elections for Candidate " << username << ":\n";
    auto &elections = system->getElections();
    for (auto &e : elections)
    {
        vector<int> candidates = e.getCandidates();
        for (int cid : candidates)
        {
            if (cid == userId)
            {
                cout << "Election ID: " << e.getElectionId()
                     << ", Title: " << e.getTitle() << endl;
            }
        }
    }
}

void Candidate::viewVoteCount(int electionId)
{
    int count = 0;
    for (const Vote &v : system->getVotes())
    {
        if (v.getElectionId() == electionId &&
            v.getVoterId() == userId)
        {
            count++;
        }
    }
    cout << "Total votes received in Election " << electionId
         << ": " << count << endl;
}
/* ---------- main ---------- */
int main()
{
    VotingSystem system;
    system.fillDate(); // IMPORTANT
    cout << "\n===== TEST: ensure if admins created sucessfully =====\n";
    for (User *u : system.getUsers())
    {
        if (u->getRole() == "Admin")
        {
            cout << "Admin User - ID: " << u->getUserId() << ", Username: " << u->getUsername() << endl;
        }
    } /// COMPLETED
    cout << "\n===== TEST: ensure if create election logic is correct =====\n";
    Admin *adminUser = nullptr;
    for (User *u : system.getUsers())
    {
        if (u->getRole() == "Admin")
        {
            adminUser = dynamic_cast<Admin *>(u); // to convert from user to admin
            break;
        }
    }
    if (adminUser)
    {
        int id = adminUser->createElection(); // Test creating a new election
        // make sure  that the election created sucessfully

        for (const Election &e : system.getElections())
        {
            if (e.getElectionId() == id)
            {
                cout << "ID: " << e.getElectionId()
                     << " | Title: " << e.getTitle()
                     << " | Status: ";

                if (e.getStatus() == ElectionStatus::CREATED)
                    cout << "Created";
                else if (e.getStatus() == ElectionStatus::OPENED)
                    cout << "Opened";
                else
                    cout << "Closed";

                cout << endl;
            }
        }

        cout << "Enter the election id : ";
        cin >> id;
        cin.ignore();
        adminUser->updateElection(id);
        for (const Election &e : system.getElections())
        {
            if (e.getElectionId() == id)
            {
                cout << "ID: " << e.getElectionId()
                     << " | Title: " << e.getTitle()
                     << " | Status: ";

                if (e.getStatus() == ElectionStatus::CREATED)
                    cout << "Created";
                else if (e.getStatus() == ElectionStatus::OPENED)
                    cout << "Opened";
                else
                    cout << "Closed | ";
                cout << e.getDescription();

                cout << endl;
            }
        }
        cout << "Enter the election id : ";
        cin >> id;
        adminUser->openElection(id);
        for (const Election &e : system.getElections())
        {
            if (e.getElectionId() == id)
            {
                cout << "ID: " << e.getElectionId()
                     << " | Title: " << e.getTitle()
                     << " | Status: ";

                if (e.getStatus() == ElectionStatus::CREATED)
                    cout << "Created ---";
                else if (e.getStatus() == ElectionStatus::OPENED)
                    cout << "Opened -- ";
                else
                    cout << "Closed | ";
                cout << e.getDescription();

                cout << endl;
            }
        }
        cout << "Enter the election id to CLOSE IT: ";
        cin >> id;
        adminUser->closeElection(id);
        for (const Election &e : system.getElections())
        {
            if (e.getElectionId() == id)
            {
                cout << "ID: " << e.getElectionId()
                     << " | Title: " << e.getTitle()
                     << " | Status: ";

                if (e.getStatus() == ElectionStatus::CREATED)
                    cout << "Created";
                else if (e.getStatus() == ElectionStatus::OPENED)
                    cout << "Opened";
                else
                    cout << "Closed | ";
                cout << e.getDescription();

                cout << endl;
            }
        }
    }
    // cout << "\n===== TEST: Guest View Elections =====\n";

    Guest guest(&system);

    // // test view all elections
    guest.viewElections();

    // cout << "\n===== TEST: Guest View Election Details (Election ID = 1) =====\n";

    // // test view election details
    guest.viewElectionDetails(1);

    // cout << "\n===== TEST: Guest View Election Details (Invalid ID) =====\n";

    // // test invalid election
    guest.viewElectionDetails(999);

    TestCandidate(system);

    return 0;
}


void TestCandidate(VotingSystem &system) // Youssef Wagih
{
    cout<<"\n\n===== TEST CASES FOR CANDIDATE =====\n";
    cout << "\n===== TEST: Display All Users =====\n";
    // display all users
    cout << "===== All Users in System =====\n";
    for (User *u : system.getUsers())
    {
        cout << "UserID: " << u->getUserId()
             << ", Username: " << u->getUsername()
             << ", Email: " << u->getEmail()
             << ", password: " << u->getPassword()
             << ", Role: " << u->getRole() << endl;
    }
    cout << "===== TEST: Candidate Login (Existing) =====\n";

    // // Existing candidate (from fillDate)
    Candidate *existingCandidate = nullptr;

    for (User *u : system.getUsers())
    {
        if (u->getRole() == "Candidate")
        {
            existingCandidate = dynamic_cast<Candidate *>(u);
            break;
        }
    }

    if (existingCandidate)
    {
        existingCandidate->login();           // test login
        existingCandidate->viewMyElections(); // test elections
        existingCandidate->logout();          // test logout
        for (Election &e : system.getElections())
        {
            vector<int> candidates = e.getCandidates();
            for (int cid : candidates)
            {
                if (cid == existingCandidate->getUserId())
                {
                    cout << "\nViewing vote count for Election ID: " << e.getElectionId() << " : " << e.getElectionId() << endl;
                }
            }
        }
    }

    cout << "\n===== TEST: Candidate Registration =====\n";

    Candidate *newCandidate =
        new Candidate(999, "", "", "", "", &system);

    newCandidate->registerUser(); // should add itself to system users

    cout << "\n===== TEST: Login After Registration =====\n";
    newCandidate->login();

    cout << "\n===== TEST COMPLETE =====\n";
}