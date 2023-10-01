#include <iostream>
#include "MT4ManagerAPI.h" // Include the header file with the Manager API classes

// Function to connect to the MT4 Manager Server
bool ConnectToMT4Server(CManagerFactory& managerFactory, CManagerInterface*& managerInstance, const char* serverAddress, int login, const char* password) {
    // Initialize Winsock
    if (managerFactory.WinsockStartup() != RET_OK) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return false;
    }

    // Check if the Manager Factory is valid
    if (!managerFactory.IsValid()) {
        std::cerr << "Manager Factory is not valid. Please check the DLL path." << std::endl;
        managerFactory.WinsockCleanup();
        return false;
    }

    // Get the manager version
    int managerVersion = managerFactory.Version();

    // Create the Manager Interface
    managerInstance = managerFactory.Create(managerVersion);

    // Check if the Manager Interface is created successfully
    if (!managerInstance) {
        std::cerr << "Failed to create Manager Interface." << std::endl;
        managerFactory.WinsockCleanup();
        return false;
    }

    // Connect to the server
    if (managerInstance->Connect(serverAddress) != RET_OK) {
        std::cerr << "Failed to connect to the MT4 Manager Server." << std::endl;
        managerInstance->Release();
        managerFactory.WinsockCleanup();
        return false;
    }

    // Log in to the server
    if (managerInstance->Login(login, password) != RET_OK) {
        std::cerr << "Failed to log in to the server." << std::endl;
        managerInstance->Disconnect();
        managerInstance->Release();
        managerFactory.WinsockCleanup();
        return false;
    }

    return true;
}

// Function to get all users after a successful connection
void GetAllUsers(CManagerInterface* managerInstance) {
    // Check if the Manager Interface is valid
    if (!managerInstance) {
        std::cerr << "Manager Interface is not valid." << std::endl;
        return;
    }

    // Request the total number of users
    int totalUsers = 0;
    UserRecord* users = managerInstance->UsersRequest(&totalUsers);

    // Check if the request was successful
    if (totalUsers > 0 && users) {
        std::cout << "Total users: " << totalUsers << std::endl;

        // Loop through and print user information
        for (int i = 0; i < totalUsers; ++i) {
            std::cout << "User Login: " << users[i].login << ", Name: " << users[i].name << ", Balance: " << users[i].balance << std::endl;
        }

        // Free the memory allocated for users
        managerInstance->MemFree(users);
    } else {
        std::cerr << "Failed to retrieve user information." << std::endl;
    }
}

int main() {
    // Define the MT4 Manager Server connection details
    const char* serverAddress = "127.0.0.1"; // Replace with the actual server IP address
    int login = 12345; // Replace with your login ID
    const char* password = "your_password"; // Replace with your password

    // Create a Manager Factory
    CManagerFactory managerFactory;

    // Create a Manager Interface
    CManagerInterface* managerInstance = nullptr;

    // Connect to the MT4 Manager Server
    if (ConnectToMT4Server(managerFactory, managerInstance, serverAddress, login, password)) {
        // Successfully connected, now get all users
        GetAllUsers(managerInstance);

        // Disconnect and release resources
        managerInstance->Disconnect();
        managerInstance->Release();
        managerFactory.WinsockCleanup();
    }

    return 0;
}
