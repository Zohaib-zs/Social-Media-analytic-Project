#ifndef ACTIONSTACK_H
#define ACTIONSTACK_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

// Comparison functions (replaces lambdas)
bool compareUserActivity(const pair<int, int>& a, const pair<int, int>& b);
bool comparePostInteractions(const pair<int, int>& a, const pair<int, int>& b);

class ActionStack {
private:
    struct ActionNode {
        string actionType;     // "like", "unlike", "share", "comment", "post"
        int postId;
        int userId;
        string timestamp;
        string details;
        int actionValue;       // For analytics (likes count, etc.)
        ActionNode* next;
        
        ActionNode(string type, int pid, int uid, string det = "", int value = 0) 
            : actionType(type), postId(pid), userId(uid), details(det), actionValue(value), next(NULL) {
            //  Direct time code instead of method call
            time_t now = time(0);
            char* dt = ctime(&now);
            timestamp = string(dt);
            // Remove newline character
            if (!timestamp.empty() && timestamp[timestamp.length()-1] == '\n') {
                timestamp = timestamp.substr(0, timestamp.length()-1);
            }
        }
        
        string getActionIcon() {
            if (actionType == "like") return "[LIKE]";
            else if (actionType == "unlike") return "[UNLIKE]";
            else if (actionType == "share") return "[SHARE]";
            else if (actionType == "comment") return "[COMMENT]";
            else if (actionType == "post") return "[POST]";
            else if (actionType == "delete") return "[DELETE]";
            else if (actionType == "edit") return "[EDIT]";
            else if (actionType == "pin") return "[PIN]";
            else return "[ACTION]";
        }
        
        string getActionDescription() {
            if (actionType == "like") return "Liked post";
            else if (actionType == "unlike") return "Unliked post";
            else if (actionType == "share") return "Shared post";
            else if (actionType == "comment") return "Commented on post";
            else if (actionType == "post") return "Created post";
            else if (actionType == "delete") return "Deleted post";
            else if (actionType == "edit") return "Edited post";
            else if (actionType == "pin") return "Pinned post";
            else return "Performed action on post";
        }
        
        string getFormattedAction() {
            stringstream ss;
            ss << getActionIcon() << " " << getActionDescription() << " #" << postId;
            if (!details.empty()) {
                ss << " - " << details;
            }
            if (actionValue > 0) {
                ss << " (Value: " << actionValue << ")";
            }
            return ss.str();
        }
    };
    
    ActionNode* top;
    int size;
    int maxSize;
    int totalActions;
    
    // Analytics data
    map<string, int> actionCounts;
    map<int, int> userActivity;
    map<int, int> postInteractions;

public:
    ActionStack(int max = 100) : top(NULL), size(0), maxSize(max), totalActions(0) {}
    
    ~ActionStack() {
        clearStack();
    }
    
    // Enhanced push with analytics
    void push(string actionType, int postId, int userId = 0, string details = "", int actionValue = 0) {
        if (size >= maxSize) {
            // Remove oldest action if stack is full
            removeOldestAction();
        }
        
        ActionNode* newNode = new ActionNode(actionType, postId, userId, details, actionValue);
        newNode->next = top;
        top = newNode;
        size++;
        totalActions++;
        
        // Update analytics
        actionCounts[actionType]++;
        userActivity[userId]++;
        postInteractions[postId]++;
        
        cout << "    [ACTION] Action recorded: " << newNode->getFormattedAction() << endl;
    }
    
    // Enhanced pop with undo confirmation
    string pop() {
        if (isEmpty()) {
            return "STACK_EMPTY";
        }
        
        ActionNode* temp = top;
        string undoneAction = top->getFormattedAction();
        
        // Update analytics
        actionCounts[top->actionType]--;
        userActivity[top->userId]--;
        postInteractions[top->postId]--;
        
        top = top->next;
        delete temp;
        size--;
        
        return undoneAction;
    }
    
    // Batch undo multiple actions
    vector<string> undoMultiple(int count) {
        vector<string> undoneActions;
        int actualCount = count;
        if (size < count) {
            actualCount = size;
        }
        
        cout << "    [UNDO] Undoing " << actualCount << " action(s)..." << endl;
        
        for (int i = 0; i < actualCount; i++) {
            if (!isEmpty()) {
                string undone = pop();
                undoneActions.push_back(undone);  // Fixed: Use 'undone' instead of 'undoAction'
                cout << "    | " << (i+1) << ". " << undone << endl;
            }
        }
        
        cout << "    [SUCCESS] " << undoneActions.size() << " action(s) undone successfully" << endl;
        return undoneActions;
    }
    
    // Smart undo - undo last action of specific type
    bool undoLastOfType(string actionType) {
        if (isEmpty()) return false;
        
        // If top is the desired type, pop it
        if (top->actionType == actionType) {
            string undone = pop();
            cout << "    [SUCCESS] Undid: " << undone << endl;
            return true;
        }
        
        // Search for the action type in stack
        ActionNode* current = top;
        ActionNode* prev = NULL;
        
        while (current != NULL && current->actionType != actionType) {
            prev = current;
            current = current->next;
        }
        
        if (current != NULL) {
            // Remove the node
            prev->next = current->next;
            if (current == top) top = current->next;
            
            // Update analytics
            actionCounts[current->actionType]--;
            userActivity[current->userId]--;
            postInteractions[current->postId]--;
            
            string undone = current->getFormattedAction();
            delete current;
            size--;
            
            cout << "    [SUCCESS] Undid: " << undone << endl;
            return true;
        }
        
        cout << "    [ERROR] No " << actionType << " actions found to undo" << endl;
        return false;
    }
    
    // Enhanced display with professional formatting
    void display(bool showDetailed = false) {
        if (isEmpty()) {
            cout << "    [INFO] Action history is empty" << endl;
            return;
        }
        
        cout << "    +---------------- ACTION HISTORY ----------------+" << endl;
        cout << "    | Total: " << setw(3) << size << " actions | " << setw(5) << totalActions << " total |" << endl;
        cout << "    +------------------------------------------------+" << endl;
        
        ActionNode* current = top;
        int count = 1;
        
        while (current != NULL) {
            if (showDetailed) {
                displayDetailedAction(current, count);
            } else {
                displayCompactAction(current, count);
            }
            
            current = current->next;
            count++;
            
            if (current != NULL) {
                cout << "    |                                                  |" << endl;
            }
        }
        
        if (!showDetailed) {
            cout << "    +------------------------------------------------+" << endl;
        }
    }
    
    // Display recent actions (last N)
    void displayRecent(int count) {
        int displayCount = count;
        if (size < count) {
            displayCount = size;
        }
        cout << "    [RECENT] Showing last " << displayCount << " actions:" << endl;
        
        ActionNode* current = top;
        int displayed = 0;
        
        while (current != NULL && displayed < displayCount) {
            displayCompactAction(current, displayed + 1);
            current = current->next;
            displayed++;
            
            if (current != NULL && displayed < displayCount) {
                cout << "    |                                                  |" << endl;
            }
        }
    }
    
    // Search actions by criteria
    void searchActions(string keyword) {
        cout << "    [SEARCH] Searching actions for: '" << keyword << "'" << endl;
        
        ActionNode* current = top;
        bool found = false;
        int count = 1;
        
        while (current != NULL) {
            if (current->actionType.find(keyword) != string::npos ||
                current->details.find(keyword) != string::npos) {
                // Replace to_string with stringstream
                stringstream ss;
                ss << current->postId;
                if (ss.str().find(keyword) != string::npos) {
                
                    if (!found) {
                        cout << "    +------------- SEARCH RESULTS -------------+" << endl;
                        found = true;
                    }
                    displayCompactAction(current, count);
                    count++;
                }
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "    [ERROR] No actions found matching your search" << endl;
        } else {
            cout << "    +------------------------------------------------+" << endl;
        }
    }
    
    void filterByType(string actionType) {
        cout << "    [FILTER] Showing " << actionType << " actions:" << endl;
        
        ActionNode* current = top;
        bool found = false;
        int count = 1;
        
        while (current != NULL) {
            if (current->actionType == actionType) {
                if (!found) {
                    found = true;
                }
                displayCompactAction(current, count);
                count++;
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "    [ERROR] No " << actionType << " actions found" << endl;
        }
    }
    
    void filterByUser(int userId) {
        cout << "    [FILTER] Showing actions by User #" << userId << ":" << endl;
        
        ActionNode* current = top;
        bool found = false;
        int count = 1;
        
        while (current != NULL) {
            if (current->userId == userId) {
                if (!found) {
                    found = true;
                }
                displayCompactAction(current, count);
                count++;
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "    [ERROR] No actions found for User #" << userId << endl;
        }
    }
    
    // Advanced analytics
    void displayActionAnalytics() {
        cout << "    +-------------- ACTION ANALYTICS ---------------+" << endl;
        cout << "    | Total Actions:      " << setw(27) << right << totalActions << " |" << endl;
        cout << "    | Current Stack:      " << setw(27) << right << size << " |" << endl;
        cout << "    | Stack Capacity:     " << setw(27) << right << maxSize << " |" << endl;
        cout << "    | Utilization:        " << setw(27) << right 
             << fixed << setprecision(1) << (double(size) / maxSize * 100) << "% |" << endl;
        cout << "    +----------------- BREAKDOWN -------------------|" << endl;
        
        // Display action type breakdown
        //  Replace range-based for with iterator
        for (map<string, int>::iterator it = actionCounts.begin(); it != actionCounts.end(); ++it) {
            if (it->second > 0) {
                cout << "    | " << setw(12) << left << it->first << ": " 
                     << setw(4) << right << it->second 
                     << setw(25) << " |" << endl;
            }
        }
        
        cout << "    +------------------------------------------------+" << endl;
    }
    
    void displayUserActivity() {
        cout << "    +-------------- USER ACTIVITY ---------------+" << endl;
        
        // Convert to vector and sort by activity
        //  Add space in template
        vector<pair<int, int> > sortedUsers;
        for (map<int, int>::iterator it = userActivity.begin(); it != userActivity.end(); ++it) {
            sortedUsers.push_back(make_pair(it->first, it->second));
        }
        
        //  Replace lambda with function pointer
        sort(sortedUsers.begin(), sortedUsers.end(), compareUserActivity);
        
        int displayCount = 5;
        if (sortedUsers.size() < 5) {
            displayCount = sortedUsers.size();
        }
        for (int i = 0; i < displayCount; i++) {
            if (sortedUsers[i].second > 0) {
                cout << "    | User #" << setw(3) << sortedUsers[i].first 
                     << ": " << setw(4) << sortedUsers[i].second << " actions" 
                     << setw(20) << " |" << endl;
            }
        }
        
        if (sortedUsers.empty()) {
            cout << "    |           No user activity data           |" << endl;
        }
        
        cout << "    +--------------------------------------------+" << endl;
    }
    
    void displayPostInteractions() {
        cout << "    +------------ POST INTERACTIONS -------------+" << endl;
        
        // Convert to vector and sort by interactions
        // Add space in template
        vector<pair<int, int> > sortedPosts;
        for (map<int, int>::iterator it = postInteractions.begin(); it != postInteractions.end(); ++it) {
            sortedPosts.push_back(make_pair(it->first, it->second));
        }
        
        // Replace lambda with function pointer
        sort(sortedPosts.begin(), sortedPosts.end(), comparePostInteractions);
        
        int displayCount = 5;
        if (sortedPosts.size() < 5) {
            displayCount = sortedPosts.size();
        }
        for (int i = 0; i < displayCount; i++) {
            if (sortedPosts[i].second > 0) {
                cout << "    | Post #" << setw(3) << sortedPosts[i].first 
                     << ": " << setw(4) << sortedPosts[i].second << " actions" 
                     << setw(20) << " |" << endl;
            }
        }
        
        if (sortedPosts.empty()) {
            cout << "    |         No post interaction data         |" << endl;
        }
        
        cout << "    +--------------------------------------------+" << endl;
    }
    
    // Utility functions
    string peek() {
        if (isEmpty()) return "No actions available";
        return top->getFormattedAction();
    }
    
    //Add missing getTopAction method
    string getTopAction() {
        if (isEmpty()) return "";
        stringstream ss;
        ss << top->postId;
        return top->actionType + " " + ss.str();
    }
    
    void getTopActionDetails() {
        if (isEmpty()) {
            cout << "    [INFO] No actions in stack" << endl;
            return;
        }
        
        cout << "    +------------- LATEST ACTION --------------+" << endl;
        displayDetailedAction(top, 1);
    }
    
    bool isEmpty() { return top == NULL; }
    int getSize() { return size; }
    int getTotalActions() { return totalActions; }
    int getMaxSize() { return maxSize; }
    
    // Clear entire stack
    void clearStack() {
        while (!isEmpty()) {
            pop();
        }
        cout << "    [CLEAR] Action stack cleared successfully" << endl;
    }
    
    // Export actions to string (for logging)
    string exportActions(int count = 10) {
        stringstream ss;
        ActionNode* current = top;
        int exported = 0;
        
        while (current != NULL && exported < count) {
            ss << current->getFormattedAction() << " | " << current->timestamp << "\n";
            current = current->next;
            exported++;
        }
        
        return ss.str();
    }

private:
    void displayCompactAction(ActionNode* action, int number) {
        cout << "    | " << setw(2) << number << ". " << setw(45) << left 
             << action->getFormattedAction() << "|" << endl;
    }
    
    void displayDetailedAction(ActionNode* action, int number) {
        cout << "    | " << setw(2) << number << ". " << action->getActionIcon() 
             << " " << action->getActionDescription() << "         |" << endl;
        cout << "    |     Post: #" << setw(39) << left << action->postId << "|" << endl;
        cout << "    |     User: #" << setw(39) << left << action->userId << "|" << endl;
        if (!action->details.empty()) {
            cout << "    |     Details: " << setw(34) << left << action->details << "|" << endl;
        }
        if (action->actionValue > 0) {
            cout << "    |     Value: " << setw(36) << left << action->actionValue << "|" << endl;
        }
        cout << "    |     Time: " << setw(37) << left << action->timestamp << "|" << endl;
        cout << "    +------------------------------------------------+" << endl;
    }
    
    void removeOldestAction() {
        if (isEmpty() || top->next == NULL) {
            if (!isEmpty()) pop();
            return;
        }
        
        ActionNode* current = top;
        ActionNode* prev = NULL;
        
        while (current->next != NULL) {
            prev = current;
            current = current->next;
        }
        
        if (prev != NULL) {
            prev->next = NULL;
        }
        
        // Update analytics
        actionCounts[current->actionType]--;
        userActivity[current->userId]--;
        postInteractions[current->postId]--;
        
        delete current;
        size--;
    }
};

// Comparison functions instead of lambdas
bool compareUserActivity(const pair<int, int>& a, const pair<int, int>& b) {
    return a.second > b.second;
}

bool comparePostInteractions(const pair<int, int>& a, const pair<int, int>& b) {
    return a.second > b.second;
}

#endif
