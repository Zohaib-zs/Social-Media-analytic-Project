#ifndef SOCIALMEDIASYSTEM_H
#define SOCIALMEDIASYSTEM_H

#include "NotificationQueue.h"
#include "PostList.h"
#include "FriendTree.h"
#include "ActionStack.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

class SocialMediaSystem {
private:
    PostList posts;
    NotificationQueue notifications;
    FriendTree network;
    ActionStack actionHistory;
    vector<int> users;
    int currentUser;

    string intToString(int value) {
        stringstream ss;
        ss << value;
        return ss.str();
    }

    string getCurrentTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string timeStr = string(dt);
        return timeStr.substr(0, timeStr.length()-1);
    }

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void pressEnterToContinue() {
        cout << "\n";
        cout << "    +- Press ENTER to return to dashboard...";
        cin.ignore();
        cin.get();
    }

    void printLine(char ch, int length) {
        cout << string(length, ch) << endl;
    }

    void printBoxedText(string text, bool centered = true) {
        int padding = (60 - text.length()) / 2;
        if (centered && padding > 0) {
            cout << "    ¦ " << setw(padding + text.length()) << text << setw(padding) << " ¦" << endl;
        } else {
            cout << "    ¦ " << setw(58) << left << text << " ¦" << endl;
        }
    }

    void printHeader() {
        clearScreen();
        cout << "    +----------------------------------------------------------+" << endl;
        cout << "    ¦                                                          ¦" << endl;
        printBoxedText("SOCIAL NETWORK ANALYTICS PLATFORM");
        printBoxedText("Enterprise Dashboard v2.1");
        cout << "    ¦                                                          ¦" << endl;
        cout << "    +----------------------------------------------------------+" << endl;
    }

    void printSectionHeader(string title) {
        cout << "\n";
        cout << "    +----------------------------------------------------------+" << endl;
        cout << "    ¦ " << setw(58) << left << ("? " + title) << "¦" << endl;
        cout << "    +----------------------------------------------------------+" << endl;
    }

    void printStatusMessage(string type, string message) {
        string prefix;
        if (type == "SUCCESS") prefix = "? ";
        else if (type == "ERROR") prefix = "? ";
        else if (type == "INFO") prefix = "?? ";
        else if (type == "WARNING") prefix = "?? ";
        
        cout << "    [" << setw(8) << left << type << "] " << message << endl;
    }

    void displayUserPanel() {
        cout << "\n";
        cout << "    +------------------- USER PANEL --------------------------+" << endl;
        cout << "    ¦                                                        ¦" << endl;
        cout << "    ¦   ?? User ID: #" << setw(44) << left << intToString(currentUser) << "¦" << endl;
        cout << "    ¦   ?? Session: " << setw(44) << left << getCurrentTime() << "¦" << endl;
        cout << "    ¦   ?? Status: " << setw(44) << left << "Active Session" << "¦" << endl;
        cout << "    ¦                                                        ¦" << endl;
        cout << "    +---------------------------------------------------------+" << endl;
    }

    void displayQuickStats() {
        cout << "\n";
        cout << "    +------------------ QUICK STATISTICS ---------------------+" << endl;
        cout << "    ¦                                                        ¦" << endl;
        cout << "    ¦   ?? Posts:        " << setw(8) << posts.getPostCount() << "          ?? Alerts:     " << setw(8) << notifications.getSize() << "   ¦" << endl;
        cout << "    ¦   ?? Actions:      " << setw(8) << actionHistory.getSize() << "          ?? Network:   " << setw(8) << "5" << "   ¦" << endl;
        cout << "    ¦   ?? Storage:      " << setw(8) << "Active" << "          ?? Last Sync: " << setw(8) << "Now" << "   ¦" << endl;
        cout << "    ¦                                                        ¦" << endl;
        cout << "    +---------------------------------------------------------+" << endl;
    }

    void displayMainMenu() {
        printHeader();
        displayUserPanel();
        displayQuickStats();
        
        cout << "\n";
        cout << "    +------------------- NAVIGATION MENU ---------------------+" << endl;
        cout << "    ¦                                                        ¦" << endl;
        cout << "    ¦   1. ??  Compose New Post                              ¦" << endl;
        cout << "    ¦   2. ??  View News Feed                                ¦" << endl;
        cout << "    ¦   3. ??  Engage - Like Post                            ¦" << endl;
        cout << "    ¦   4. ??  Engage - Unlike Post                          ¦" << endl;
        cout << "    ¦   5. ??  Notification Center                           ¦" << endl;
        cout << "    ¦   6. ??  Network Analysis                              ¦" << endl;
        cout << "    ¦   7. ??  Social Graph View                             ¦" << endl;
        cout << "    ¦   8. ??   Action History                               ¦" << endl;
        cout << "    ¦   9. ??  System Logs                                   ¦" << endl;
        cout << "    ¦                                                        ¦" << endl;
        cout << "    ¦   0. ??  Exit Dashboard                                ¦" << endl;
        cout << "    ¦                                                        ¦" << endl;
        cout << "    +---------------------------------------------------------+" << endl;
        cout << "\n    +- Enter your choice [0-9]: ";
    }

    void displayPostInBox(int postId, int userId, string content, int likes, string timestamp) {
        cout << "    +------------------- POST #" << setw(2) << postId << " --------------------+" << endl;
        cout << "    ¦ User: #" << setw(47) << left << intToString(userId) << "¦" << endl;
        cout << "    ¦ " << setw(56) << left << content.substr(0,56) << "¦" << endl;
        if (content.length() > 56) {
            cout << "    ¦ " << setw(56) << left << content.substr(56) << "¦" << endl;
        }
        cout << "    ¦ Likes: " << setw(46) << left << intToString(likes) << "¦" << endl;
        cout << "    ¦ " << setw(56) << left << timestamp << "¦" << endl;
        cout << "    +---------------------------------------------------------+" << endl;
    }

public:
    SocialMediaSystem() : currentUser(1) {
        initializeSampleData();
    }
    
    void initializeSampleData() {
        // Initialize network
        for (int i = 1; i <= 5; i++) {
            network.addUser(i);
            users.push_back(i);
        }
        
        // Create social connections
        network.addFriendship(1, 2);
        network.addFriendship(1, 3);
        network.addFriendship(2, 3);
        network.addFriendship(2, 4);
        network.addFriendship(3, 5);
        
        // Sample posts with professional content
        posts.addPost(1, "Welcome to the Social Network Analytics Platform. This system demonstrates advanced data structure implementations.");
        posts.addPost(2, "Network analysis completed. All data structures (Queue, Stack, Tree, Linked List) operating at optimal efficiency.");
        posts.addPost(3, "Real-time analytics dashboard activated. Monitoring social interactions and network patterns.");
        
        // System notifications
        notifications.enqueue("System: Platform initialized successfully");
        notifications.enqueue("Alert: New network connections detected");
        notifications.enqueue("Update: Performance metrics within normal parameters");
    }
    
    void createPost() {
        printSectionHeader("COMPOSE NEW POST");
        string content;
        cout << "    Enter your message: ";
        cin.ignore();
        getline(cin, content);
        
        if (content.empty()) {
            printStatusMessage("ERROR", "Post content cannot be empty");
            return;
        }
        
        posts.addPost(currentUser, content);
        notifications.enqueue("New content published by User #" + intToString(currentUser));
        printStatusMessage("SUCCESS", "Post published to network feed");
    }
    
    void likePost() {
        printSectionHeader("ENGAGE - LIKE POST");
        int postId;
        cout << "    Enter Post ID to engage: ";
        cin >> postId;
        
        cout << "    Processing engagement request..." << endl;
        posts.likePost(postId);
        actionHistory.push("like", postId);
        notifications.enqueue("Post #" + intToString(postId) + " received positive engagement");
        printStatusMessage("SUCCESS", "Engagement recorded successfully");
    }
    
    void unlikePost() {
        printSectionHeader("ENGAGE - UNLIKE POST");
        int postId;
        cout << "    Enter Post ID to modify engagement: ";
        cin >> postId;
        
        cout << "    Processing engagement update..." << endl;
        posts.unlikePost(postId);
        actionHistory.push("unlike", postId);
        printStatusMessage("SUCCESS", "Engagement updated successfully");
    }
    
    void undoLastAction() {
        printSectionHeader("ACTION MANAGEMENT");
        if (actionHistory.isEmpty()) {
            printStatusMessage("INFO", "No recent actions available for undo");
            return;
        }
        
        string lastAction = actionHistory.getTopAction();
        cout << "    Last action: " << lastAction << endl;
        cout << "    Reverting changes..." << endl;
        actionHistory.pop();
        printStatusMessage("SUCCESS", "Action reverted successfully");
    }
    
    void findMutualFriends() {
        printSectionHeader("NETWORK ANALYSIS");
        int user1, user2;
        cout << "    Enter First User ID: ";
        cin >> user1;
        cout << "    Enter Second User ID: ";
        cin >> user2;
        
        cout << "\n    Analyzing social graph connections..." << endl;
        cout << "    Users: #" << user1 << " ? #" << user2 << endl;
        cout << "    --------------------------------------------------------" << endl;
        network.findMutualFriends(user1, user2);
    }
    
    void displayFeed() {
        printSectionHeader("NETWORK ACTIVITY FEED");
        cout << "    Recent posts from your network:\n" << endl;
        posts.displayPosts();
    }
    
    void checkNotifications() {
        printSectionHeader("NOTIFICATION CENTER");
        cout << "    Recent system alerts and updates:\n" << endl;
        notifications.display();
    }
    
    void displayNetwork() {
        printSectionHeader("SOCIAL GRAPH VISUALIZATION");
        cout << "    Current network structure and connections:\n" << endl;
        network.displayNetwork();
    }
    
    void displayActionHistory() {
        printSectionHeader("SYSTEM ACTION LOG");
        cout << "    Recent user interactions and system events:\n" << endl;
        actionHistory.display();
    }
    
    void simulateGUI() {
        int choice;
        
        do {
            displayMainMenu();
            cin >> choice;
            
            switch(choice) {
                case 1:
                    createPost();
                    pressEnterToContinue();
                    break;
                case 2:
                    displayFeed();
                    pressEnterToContinue();
                    break;
                case 3:
                    likePost();
                    pressEnterToContinue();
                    break;
                case 4:
                    unlikePost();
                    pressEnterToContinue();
                    break;
                case 5:
                    checkNotifications();
                    pressEnterToContinue();
                    break;
                case 6:
                    findMutualFriends();
                    pressEnterToContinue();
                    break;
                case 7:
                    displayNetwork();
                    pressEnterToContinue();
                    break;
                case 8:
                    undoLastAction();
                    pressEnterToContinue();
                    break;
                case 9:
                    displayActionHistory();
                    pressEnterToContinue();
                    break;
                case 0:
                    clearScreen();
                    cout << "    +----------------------------------------------------------+" << endl;
                    cout << "    ¦                                                          ¦" << endl;
                    printBoxedText("SESSION TERMINATED");
                    printBoxedText("Thank you for using");
                    printBoxedText("Social Network Analytics Platform");
                    cout << "    ¦                                                          ¦" << endl;
                    cout << "    ¦   Data Structures Project - BSSE Semester            ¦" << endl;
                    cout << "    ¦   " << getCurrentTime() << "                  ¦" << endl;
                    cout << "    ¦                                                          ¦" << endl;
                    cout << "    +----------------------------------------------------------+" << endl;
                    break;
                default:
                    printStatusMessage("ERROR", "Invalid selection. Please choose option 0-9");
                    pressEnterToContinue();
            }
        } while (choice != 0);
    }
};

#endif
