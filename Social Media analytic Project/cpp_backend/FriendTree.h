#ifndef FRIENDTREE_H
#define FRIENDTREE_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
using namespace std;

// Comparison function for sorting (replaces lambda)
bool comparePairsByScore(const pair<int, int>& a, const pair<int, int>& b);

class FriendTree {
private:
    struct UserProfile {
        int userId;
        string username;
        int age;
        string location;
        string interests;
        int friendCount;
        int connectionStrength; // 1-10 scale
    };

    struct TreeNode {
        int userId;
        UserProfile profile;
        vector<int> friends;
        vector<int> pendingRequests;
        TreeNode* left;
        TreeNode* right;
        
        TreeNode(int id) : userId(id), left(NULL), right(NULL) {
            profile.userId = id;
            //  Replace to_string with stringstream
            stringstream ss1;
            ss1 << id;
            profile.username = "User_" + ss1.str();
            profile.age = 18 + (id % 30); // Random age 18-48
            //  Replace to_string with stringstream
            stringstream ss2;
            ss2 << ((id % 5) + 1);
            profile.location = "City_" + ss2.str();
            //  Replace to_string with stringstream
            stringstream ss3;
            ss3 << ((id % 3) + 1);
            profile.interests = "Interest_" + ss3.str();
            profile.friendCount = 0;
            profile.connectionStrength = 1 + (id % 10);
        }
        
        string getFriendStatus(int targetUserId) {
            //  Replace range-based for with traditional for
            for (size_t i = 0; i < friends.size(); i++) {
                if (friends[i] == targetUserId) {
                    return "[FRIENDS] Friends";
                }
            }
            for (size_t i = 0; i < pendingRequests.size(); i++) {
                if (pendingRequests[i] == targetUserId) {
                    return "[PENDING] Pending";
                }
            }
            return "[NOT CONNECTED] Not Connected";
        }
    };
    
    TreeNode* root;
    int totalUsers;
    int totalConnections;

public:
    FriendTree() : root(NULL), totalUsers(0), totalConnections(0) {}
    
    ~FriendTree() {
        clearTree(root);
    }

    // Enhanced user management
    void addUser(int userId) {
        root = insert(root, userId);
        totalUsers++;
        cout << "    [SUCCESS] User #" << userId << " added to network" << endl;
    }

    void addUserWithProfile(int userId, string username, int age, string location, string interests) {
        root = insert(root, userId);
        TreeNode* user = search(root, userId);
        if (user) {
            user->profile.username = username;
            user->profile.age = age;
            user->profile.location = location;
            user->profile.interests = interests;
        }
        totalUsers++;
        cout << "    [SUCCESS] User '" << username << "' added with complete profile" << endl;
    }

    // Advanced friendship system
    void sendFriendRequest(int fromUser, int toUser) {
        TreeNode* sender = search(root, fromUser);
        TreeNode* receiver = search(root, toUser);
        
        if (!sender || !receiver) {
            cout << "    [ERROR] One or both users not found!" << endl;
            return;
        }
        
        if (fromUser == toUser) {
            cout << "    [ERROR] Cannot send friend request to yourself!" << endl;
            return;
        }
        
        // Check if already friends or request pending
        string status = sender->getFriendStatus(toUser);
        if (status == "[FRIENDS] Friends") {
            cout << "    [INFO] Already friends with User #" << toUser << endl;
            return;
        }
        if (status == "[PENDING] Pending") {
            cout << "    [INFO] Friend request already pending" << endl;
            return;
        }
        
        receiver->pendingRequests.push_back(fromUser);
        cout << "    [REQUEST] Friend request sent from User #" << fromUser << " to User #" << toUser << endl;
    }

    void acceptFriendRequest(int userId, int requesterId) {
        TreeNode* user = search(root, userId);
        
        if (!user) {
            cout << "    [ERROR] User not found!" << endl;
            return;
        }
        
        // Find and remove from pending requests
        //  Replace auto with explicit iterator type
        vector<int>::iterator it = find(user->pendingRequests.begin(), user->pendingRequests.end(), requesterId);
        if (it == user->pendingRequests.end()) {
            cout << "    [ERROR] No pending friend request from User #" << requesterId << endl;
            return;
        }
        
        user->pendingRequests.erase(it);
        addFriendship(userId, requesterId);
        cout << "    [SUCCESS] Friend request from User #" << requesterId << " accepted!" << endl;
    }

    void addFriendship(int user1, int user2) {
        TreeNode* node1 = search(root, user1);
        TreeNode* node2 = search(root, user2);
        
        if (node1 && node2) {
            // Check if not already friends
            //  Replace auto with explicit iterator type
            vector<int>::iterator it = find(node1->friends.begin(), node1->friends.end(), user2);
            if (it == node1->friends.end()) {
                node1->friends.push_back(user2);
                node2->friends.push_back(user1);
                node1->profile.friendCount++;
                node2->profile.friendCount++;
                totalConnections++;
                cout << "    [FRIENDSHIP] Friendship established between User #" << user1 << " and User #" << user2 << endl;
            } else {
                cout << "    [INFO] Users are already friends" << endl;
            }
        } else {
            cout << "    [ERROR] One or both users not found!" << endl;
        }
    }

    void removeFriendship(int user1, int user2) {
        TreeNode* node1 = search(root, user1);
        TreeNode* node2 = search(root, user2);
        
        if (node1 && node2) {
            // Remove user2 from user1's friends
            //  Replace auto with explicit iterator type
            vector<int>::iterator it1 = find(node1->friends.begin(), node1->friends.end(), user2);
            if (it1 != node1->friends.end()) {
                node1->friends.erase(it1);
                node1->profile.friendCount--;
            }
            
            // Remove user1 from user2's friends
            //  Replace auto with explicit iterator type
            vector<int>::iterator it2 = find(node2->friends.begin(), node2->friends.end(), user1);
            if (it2 != node2->friends.end()) {
                node2->friends.erase(it2);
                node2->profile.friendCount--;
            }
            
            totalConnections--;
            cout << "    [REMOVE] Friendship removed between User #" << user1 << " and User #" << user2 << endl;
        } else {
            cout << "    [ERROR] One or both users not found!" << endl;
        }
    }

    // Enhanced mutual friends with advanced algorithms
    void findMutualFriends(int user1, int user2) {
        TreeNode* node1 = search(root, user1);
        TreeNode* node2 = search(root, user2);
        
        if (!node1 || !node2) {
            cout << "    [ERROR] One or both users not found!" << endl;
            return;
        }
        
        vector<int> mutualFriends;
        cout << "    [ANALYZE] Analyzing social connections..." << endl;
        cout << "    |- User #" << user1 << " has " << node1->friends.size() << " friends" << endl;
        cout << "    |- User #" << user2 << " has " << node2->friends.size() << " friends" << endl;
        cout << "    |- Scanning for mutual connections...\n" << endl;
        
        // More efficient O(n) algorithm using hash map
        map<int, bool> friendMap;
        // Replace range-based for with traditional for
        for (size_t i = 0; i < node1->friends.size(); i++) {
            friendMap[node1->friends[i]] = true;
        }
        
        //  Replace range-based for with traditional for
        for (size_t i = 0; i < node2->friends.size(); i++) {
            if (friendMap[node2->friends[i]]) {
                mutualFriends.push_back(node2->friends[i]);
            }
        }
        
        if (mutualFriends.empty()) {
            cout << "    [INFO] No mutual friends found between User #" << user1 << " and User #" << user2 << endl;
        } else {
            cout << "    [SUCCESS] Found " << mutualFriends.size() << " mutual friend(s):" << endl;
            cout << "    +--------------------------------------------------+" << endl;
            for (size_t i = 0; i < mutualFriends.size(); i++) {
                TreeNode* mutualUser = search(root, mutualFriends[i]);
                cout << "    | " << setw(2) << (i+1) << ". User #" << setw(3) << mutualFriends[i];
                if (mutualUser) {
                    cout << " - " << setw(15) << left << mutualUser->profile.username;
                    cout << " (" << mutualUser->profile.friendCount << " friends)";
                }
                cout << setw(5) << " |" << endl;
            }
            cout << "    +--------------------------------------------------+" << endl;
        }
    }

    // Friend suggestions based on mutual friends and interests
    void suggestFriends(int userId) {
        TreeNode* user = search(root, userId);
        if (!user) {
            cout << "    [ERROR] User not found!" << endl;
            return;
        }
        
        cout << "    [SUGGEST] Generating friend suggestions for User #" << userId << "..." << endl;
        
        map<int, int> suggestionScores; // userID -> score
        
        // Score based on mutual friends
        //  Replace range-based for with traditional for
        for (size_t i = 0; i < user->friends.size(); i++) {
            int friendId = user->friends[i];
            TreeNode* friendNode = search(root, friendId);
            if (friendNode) {
                //  Replace range-based for with traditional for
                for (size_t j = 0; j < friendNode->friends.size(); j++) {
                    int friendOfFriend = friendNode->friends[j];
                    if (friendOfFriend != userId) {
                        // Check if not already friends
                        bool alreadyFriend = false;
                        for (size_t k = 0; k < user->friends.size(); k++) {
                            if (user->friends[k] == friendOfFriend) {
                                alreadyFriend = true;
                                break;
                            }
                        }
                        if (!alreadyFriend) {
                            suggestionScores[friendOfFriend] += 3; // Mutual friend bonus
                        }
                    }
                }
            }
        }
        
        // Score based on shared interests
        //  Replace range-based for with traditional iterator
        for (map<int, int>::iterator it = suggestionScores.begin(); it != suggestionScores.end(); ++it) {
            TreeNode* suggestedUser = search(root, it->first);
            if (suggestedUser && suggestedUser->profile.interests == user->profile.interests) {
                it->second += 2; // Shared interest bonus
            }
        }
        
        // Convert to vector and sort by score
        vector<pair<int, int> > sortedSuggestions;
        for (map<int, int>::iterator it = suggestionScores.begin(); it != suggestionScores.end(); ++it) {
            sortedSuggestions.push_back(make_pair(it->first, it->second));
        }
        
        // Replace lambda with function pointer
        sort(sortedSuggestions.begin(), sortedSuggestions.end(), comparePairsByScore);
        
        if (sortedSuggestions.empty()) {
            cout << "    [INFO] No friend suggestions at this time" << endl;
        } else {
            cout << "    +-------------- FRIEND SUGGESTIONS --------------+" << endl;
            int displayCount = 5;
            if (sortedSuggestions.size() < 5) {
                displayCount = sortedSuggestions.size();
            }
            for (int i = 0; i < displayCount; i++) {
                TreeNode* suggestedUser = search(root, sortedSuggestions[i].first);
                cout << "    | " << (i+1) << ". User #" << setw(3) << sortedSuggestions[i].first;
                if (suggestedUser) {
                    cout << " - " << setw(15) << left << suggestedUser->profile.username;
                    cout << " Score: " << setw(2) << sortedSuggestions[i].second;
                }
                cout << setw(5) << " |" << endl;
            }
            cout << "    +--------------------------------------------------+" << endl;
        }
    }

    // Network analysis features
    void findMostConnectedUser() {
        if (!root) {
            cout << "    [ERROR] Network is empty!" << endl;
            return;
        }
        
        TreeNode* mostConnected = NULL;
        findMostConnectedHelper(root, mostConnected);
        
        if (mostConnected) {
            cout << "    [TOP USER] Most Connected User:" << endl;
            displayUserProfile(mostConnected->userId); // Fixed: Pass user ID instead of node
        }
    }

    void calculateNetworkDensity() {
        if (totalUsers < 2) {
            cout << "    [INFO] Network too small for density calculation" << endl;
            return;
        }
        
        int maxPossibleConnections = totalUsers * (totalUsers - 1) / 2;
        double density = (double)totalConnections / maxPossibleConnections;
        
        cout << "    [ANALYTICS] Network Density Analysis:" << endl;
        cout << "    +--------------------------------------------------+" << endl;
        cout << "    | Total Users:          " << setw(27) << right << totalUsers << " |" << endl;
        cout << "    | Total Connections:    " << setw(27) << right << totalConnections << " |" << endl;
        cout << "    | Max Possible:         " << setw(27) << right << maxPossibleConnections << " |" << endl;
        cout << "    | Density:              " << setw(27) << right << fixed << setprecision(3) << density << " |" << endl;
        cout << "    +--------------------------------------------------+" << endl;
    }

    // Advanced traversal with formatting
    void displayNetwork() {
        if (!root) {
            cout << "    [INFO] Social network is empty" << endl;
            return;
        }
        
        cout << "    +---------------- SOCIAL NETWORK -----------------+" << endl;
        cout << "    | Total: " << setw(3) << totalUsers << " users | " << setw(4) << totalConnections << " connections |" << endl;
        cout << "    +--------------------------------------------------+" << endl;
        
        cout << "\n    [USERS] USER CONNECTIONS:" << endl;
        inOrderTraversal(root);
    }

    void displayUserProfile(int userId) {
        TreeNode* user = search(root, userId);
        if (!user) {
            cout << "    [ERROR] User not found!" << endl;
            return;
        }
        
        cout << "    +---------------- USER PROFILE ------------------+" << endl;
        cout << "    | User ID:        " << setw(30) << right << user->userId << " |" << endl;
        cout << "    | Username:       " << setw(30) << right << user->profile.username << " |" << endl;
        cout << "    | Age:            " << setw(30) << right << user->profile.age << " |" << endl;
        cout << "    | Location:       " << setw(30) << right << user->profile.location << " |" << endl;
        cout << "    | Interests:      " << setw(30) << right << user->profile.interests << " |" << endl;
        cout << "    | Friend Count:   " << setw(30) << right << user->profile.friendCount << " |" << endl;
        cout << "    | Connection Str: " << setw(30) << right << user->profile.connectionStrength << " |" << endl;
        cout << "    +----------------- FRIENDS -----------------------|" << endl;
        
        if (user->friends.empty()) {
            cout << "    |             No friends yet                   |" << endl;
        } else {
            for (size_t i = 0; i < user->friends.size(); i++) {
                cout << "    | " << setw(2) << (i+1) << ". User #" << setw(3) << user->friends[i] 
                     << setw(35) << " |" << endl;
            }
        }
        
        if (!user->pendingRequests.empty()) {
            cout << "    +------------ PENDING REQUESTS ----------------|" << endl;
            for (size_t i = 0; i < user->pendingRequests.size(); i++) {
                cout << "    | " << setw(2) << (i+1) << ". User #" << setw(3) << user->pendingRequests[i] 
                     << setw(35) << " |" << endl;
            }
        }
        
        cout << "    +--------------------------------------------------+" << endl;
    }

    // Utility functions
    TreeNode* getRoot() { return root; }
    int getTotalUsers() { return totalUsers; }
    int getTotalConnections() { return totalConnections; }

private:
    TreeNode* insert(TreeNode* node, int userId) {
        if (node == NULL) {
            return new TreeNode(userId);
        }
        
        if (userId < node->userId) {
            node->left = insert(node->left, userId);
        } else if (userId > node->userId) {
            node->right = insert(node->right, userId);
        }
        return node;
    }
    
    TreeNode* search(TreeNode* node, int userId) {
        if (node == NULL || node->userId == userId) {
            return node;
        }
        
        if (userId < node->userId) {
            return search(node->left, userId);
        }
        return search(node->right, userId);
    }

    void inOrderTraversal(TreeNode* node) {
        if (node != NULL) {
            inOrderTraversal(node->left);
            cout << "    +- User #" << setw(3) << node->userId << " ---- " 
                 << setw(15) << left << node->profile.username 
                 << " Friends: " << setw(3) << node->friends.size() << " -+" << endl;
            
            if (!node->friends.empty()) {
                cout << "    | Connected to: ";
                int displayCount = 3;
                if (node->friends.size() < 3) {
                    displayCount = node->friends.size();
                }
                for (int i = 0; i < displayCount; i++) {
                    cout << "#" << node->friends[i];
                    if (i < displayCount - 1) cout << ", ";
                }
                if (node->friends.size() > 3) {
                    cout << " +" << (node->friends.size() - 3) << " more";
                }
                cout << setw(15) << " |" << endl;
            } else {
                cout << "    | No connections yet" << setw(30) << " |" << endl;
            }
            cout << "    +--------------------------------------------------+" << endl;
            inOrderTraversal(node->right);
        }
    }

    void findMostConnectedHelper(TreeNode* node, TreeNode*& mostConnected) {
        if (node != NULL) {
            if (!mostConnected || node->friends.size() > mostConnected->friends.size()) {
                mostConnected = node;
            }
            findMostConnectedHelper(node->left, mostConnected);
            findMostConnectedHelper(node->right, mostConnected);
        }
    }

    void clearTree(TreeNode* node) {
        if (node != NULL) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }
};

//  Comparison function instead of lambda
bool comparePairsByScore(const pair<int, int>& a, const pair<int, int>& b) {
    return a.second > b.second;
}

#endif
