#include <iostream>
#include "MT4ManagerAPI.h" // Include the MT4 Manager API header file

// Function to connect to MT4 Manager Server and get all users
void ConnectAndRetrieveUsers() {
    // Create a manager factory instance
    CManagerFactory managerFactory;

    // Initialize Winsock
    if (managerFactory.WinsockStartup() != RET_OK) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return;
    }

    // Check if the manager factory is valid
    if (!managerFactory.IsValid()) {
        std::cerr << "Failed to load MT4 Manager API DLL." << std::endl;
        managerFactory.WinsockCleanup();
        return;
    }

    // Get the manager version
    int version = managerFactory.Version();

    // Create a manager interface instance
    CManagerInterface* manager = managerFactory.Create(version);

    // Check if the manager interface instance was created successfully
    if (!manager) {
        std::cerr << "Failed to create manager interface." << std::endl;
        managerFactory.WinsockCleanup();
        return;
    }

    // Connect to the MT4 Manager Server (replace "server_address" with your server's address)
    if (manager->Connect("server_address") != RET_OK) {
        std::cerr << "Failed to connect to the MT4 Manager Server." << std::endl;
        manager->Release();
        managerFactory.WinsockCleanup();
        return;
    }

    // Login to the server (replace "login" and "password" with your credentials)
    if (manager->Login(login, "password") != RET_OK) {
        std::cerr << "Failed to log in to the server." << std::endl;
        manager->Disconnect();
        manager->Release();
        managerFactory.WinsockCleanup();
        return;
    }

    // Retrieve all users
    int totalUsers = 0;
    UserRecord* users = manager->UsersRequest(&totalUsers);

    if (users) {
        std::cout << "Total Users: " << totalUsers << std::endl;
        for (int i = 0; i < totalUsers; ++i) {
            // Process user data (you can print or manipulate the user data here)
            std::cout << "User #" << i + 1 << ": Login " << users[i].login << ", Name: " << users[i].name << std::endl;
        }

        // Free the memory allocated for user data
        manager->MemFree(users);
    } else {
        std::cerr << "Failed to retrieve user data." << std::endl;
    }

    // Disconnect from the server
    manager->Disconnect();

    // Release the manager interface
    manager->Release();

    // Cleanup Winsock
    managerFactory.WinsockCleanup();
}

int main() {
    // Connect to the MT4 Manager Server and retrieve all users
    ConnectAndRetrieveUsers();

    return 0;
}
