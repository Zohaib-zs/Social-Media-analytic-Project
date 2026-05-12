#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "SocialMediaSystem.h"

using namespace std;

// Utility functions for premium experience
void clearScreen() {
    system("cls");
}

void printLine(char ch, int length) {
    cout << string(length, ch) << endl;
}

void printCentered(string text, int width = 60) {
    int padding = (width - text.length()) / 2;
    if (padding > 0) {
        cout << string(padding, ' ') << text << endl;
    } else {
        cout << text << endl;
    }
}

void loadingDots(int dots = 3) {
    for (int i = 0; i < dots; i++) {
        cout << ".";
        cout.flush();
        // Simple delay
        for (int j = 0; j < 10000000; j++) {}
    }
}

void displayWelcomeHeader() {
    clearScreen();
    cout << "\n\n";
    printLine('=', 60);
    printCentered("SOCIAL MEDIA NETWORK PLATFORM");
    printCentered("Edition v2.0");
    printLine('=', 60);
    cout << endl;
}

void displaySystemOverview() {
    cout << "    +--------------- SYSTEM OVERVIEW ---------------+" << endl;
    cout << "    |                                              |" << endl;
    cout << "    |   [ARCH]  Modular Microservices Architecture |" << endl;
    cout << "    |   [DATA]  Advanced Data Structures Engine    |" << endl;
    cout << "    |   [UI]    Professional Dashboard Interface   |" << endl;
    cout << "    |   [ANALYTICS] Real-time Social Insights      |" << endl;
    cout << "    |                                              |" << endl;
    cout << "    +----------------------------------------------+" << endl;
}

void displayDataStructures() {
    cout << "\n    +------------- DATA STRUCTURES --------------+" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   [QUEUE]    Notification System         |" << endl;
    cout << "    |   [LIST]     Post Management             |" << endl;
    cout << "    |   [TREE]     Social Network Graph        |" << endl;
    cout << "    |   [STACK]    Action History & Undo       |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    +------------------------------------------+" << endl;
}

void displayFeatures() {
    cout << "\n    +---------------- FEATURES -----------------+" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   [POST] Content Creation & Management   |" << endl;
    cout << "    |   [SOCIAL] Friend Network Analytics      |" << endl;
    cout << "    |   [NOTIFY] Smart Notification System     |" << endl;
    cout << "    |   [ANALYZE] Engagement Metrics           |" << endl;
    cout << "    |   [UNDO] Advanced Action Management      |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    +------------------------------------------+" << endl;
}

void displayCredits() {
    cout << "\n    +---------------- CREDITS ------------------+" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   Project: Data Structures Lab           |" << endl;
    cout << "    |   Program: BSSE - 3rd Semester            |" << endl;
    cout << "    |   University: NUML                       |" << endl;
    cout << "    |   Course: Data Structures & Algorithms   |" << endl;
    cout << "    |   Year: 2025                            |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    +------------------------------------------+" << endl;
}

void displayLoadingSequence() {
    clearScreen();
    cout << "\n\n";
    printLine('*', 60);
    printCentered("SYSTEM INITIALIZATION");
    printLine('*', 60);
    
    cout << "\n    Initializing Social Network Platform";
    loadingDots();
    cout << " [DONE]" << endl;
    
    cout << "    Loading Data Structures Engine";
    loadingDots();
    cout << " [DONE]" << endl;
    
    cout << "    Establishing Network Connections";
    loadingDots();
    cout << " [DONE]" << endl;
    
    cout << "    Preparing User Interface";
    loadingDots();
    cout << " [DONE]" << endl;
    
    cout << "\n";
    printLine('-', 60);
    printCentered("SYSTEM READY");
    printLine('-', 60);
    
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void displayMainMenu() {
    clearScreen();
    displayWelcomeHeader();
    
    cout << "    Welcome to the Social Media Network Platform," << endl;
    cout << "    a comprehensive demonstration of data structures" << endl;
    cout << "    in real-world social media applications.\n" << endl;
    
    displaySystemOverview();
    displayDataStructures();
    displayFeatures();
    displayCredits();
    
    cout << "\n";
    printLine('-', 60);
    cout << "    NAVIGATION OPTIONS:" << endl;
    cout << "    1. [LAUNCH] Start Social Media Dashboard" << endl;
    cout << "    2. [INFO] System Information" << endl;
    cout << "    3. [FEATURES] Detailed Feature List" << endl;
    cout << "    4. [EXIT] Close Application" << endl;
    printLine('-', 60);
}

void displaySystemInfo() {
    clearScreen();
    cout << "\n";
    printLine('=', 60);
    printCentered("SYSTEM INFORMATION");
    printLine('=', 60);
    
    cout << "\n    +------------- TECHNICAL SPECS --------------+" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   ARCHITECTURE:                          |" << endl;
    cout << "    |   • Modular Component Design            |" << endl;
    cout << "    |   • Object-Oriented Architecture        |" << endl;
    cout << "    |   • Memory-Efficient Algorithms         |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   PERFORMANCE:                           |" << endl;
    cout << "    |   • O(1) Notification Processing        |" << endl;
    cout << "    |   • O(log n) Friend Search              |" << endl;
    cout << "    |   • O(n) Content Filtering              |" << endl;
    cout << "    |   • O(1) Action Undo/Redo               |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   COMPATIBILITY:                         |" << endl;
    cout << "    |   • Cross-Platform (Windows/Mac/Linux)  |" << endl;
    cout << "    |   • C++98 Standard Compliant            |" << endl;
    cout << "    |   • No External Dependencies            |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    +------------------------------------------+" << endl;
    
    cout << "\nPress Enter to return to main menu...";
    cin.ignore();
    cin.get();
}

void displayFeatureDetails() {
    clearScreen();
    cout << "\n";
    printLine('=', 60);
    printCentered("DETAILED FEATURES");
    printLine('=', 60);
    
    cout << "\n    +----------- CORE FUNCTIONALITY ------------+" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   POST MANAGEMENT:                       |" << endl;
    cout << "    |   • Create & publish posts               |" << endl;
    cout << "    |   • Categorize content                   |" << endl;
    cout << "    |   • Like/Unlike system                   |" << endl;
    cout << "    |   • Share functionality                  |" << endl;
    cout << "    |   • Comment system                       |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   SOCIAL NETWORK:                        |" << endl;
    cout << "    |   • Friend relationship management       |" << endl;
    cout << "    |   • Mutual friend detection              |" << endl;
    cout << "    |   • Friend suggestions                   |" << endl;
    cout << "    |   • Network visualization                |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   NOTIFICATION SYSTEM:                   |" << endl;
    cout << "    |   • Priority-based alerts                |" << endl;
    cout << "    |   • Categorized notifications            |" << endl;
    cout << "    |   • Real-time delivery                   |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   ANALYTICS & INSIGHTS:                  |" << endl;
    cout << "    |   • Engagement metrics                   |" << endl;
    cout << "    |   • User activity tracking               |" << endl;
    cout << "    |   • Network density analysis             |" << endl;
    cout << "    |   • Popular content identification       |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    |   ACTION MANAGEMENT:                     |" << endl;
    cout << "    |   • Multi-level undo system              |" << endl;
    cout << "    |   • Action history tracking              |" << endl;
    cout << "    |   • Smart undo operations                |" << endl;
    cout << "    |   • Performance optimization             |" << endl;
    cout << "    |                                          |" << endl;
    cout << "    +------------------------------------------+" << endl;
    
    cout << "\nPress Enter to return to main menu...";
    cin.ignore();
    cin.get();
}

void displayExitSequence() {
    clearScreen();
    cout << "\n\n";
    printLine('=', 60);
    printCentered("SESSION TERMINATION");
    printLine('=', 60);
    
    cout << "\n    Saving user data and system state";
    loadingDots();
    cout << " [DONE]" << endl;
    
    cout << "    Closing network connections";
    loadingDots();
    cout << " [DONE]" << endl;
    
    cout << "    Finalizing analytics data";
    loadingDots();
    cout << " [DONE]" << endl;
    
    cout << "    Performing cleanup operations";
    loadingDots();
    cout << " [DONE]" << endl;
    
    cout << "\n";
    printLine('-', 60);
    printCentered("THANK YOU FOR USING");
    printCentered("SOCIAL NETWORK ANALYTICS PLATFORM");
    printLine('-', 60);
    
    cout << "\n    Academic Project - Data Structures Lab" << endl;
    cout << "    BSE 3rd Semester - NUML University" << endl;
    cout << "\n    System shutdown complete." << endl;
    cout << "\n\n";
}

void runDemoMode() {
    clearScreen();
    cout << "\n";
    printLine('=', 60);
    printCentered("DEMONSTRATION MODE");
    printLine('=', 60);
    
    cout << "\n    Initializing sample data for demonstration";
    loadingDots();
    cout << " [READY]" << endl;
    
    cout << "\n    This demonstration will showcase:" << endl;
    cout << "    • Social network creation and management" << endl;
    cout << "    • Post creation and interaction system" << endl;
    cout << "    • Notification and action tracking" << endl;
    cout << "    • Advanced analytics features" << endl;
    
    cout << "\n    Press Enter to launch the interactive dashboard...";
    cin.ignore();
    cin.get();
}

int main() {
    // Display loading sequence
    displayLoadingSequence();
    
    int choice;
    bool exitProgram = false;
    
    while (!exitProgram) {
        displayMainMenu();
        cout << "\n    Enter your choice (1-4): ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                runDemoMode();
                // Launch the main social media system
                {
                    SocialMediaSystem socialMedia;
                    socialMedia.simulateGUI();
                }
                break;
                
            case 2:
                displaySystemInfo();
                break;
                
            case 3:
                displayFeatureDetails();
                break;
                
            case 4:
                exitProgram = true;
                break;
                
            default:
                cout << "\n    [ERROR] Invalid selection! Please choose 1-4." << endl;
                cout << "    Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
        }
    }
    
    // Display exit sequence
    displayExitSequence();
    
    return 0;
}
