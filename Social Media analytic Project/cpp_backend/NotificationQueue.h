#ifndef NOTIFICATIONQUEUE_H
#define NOTIFICATIONQUEUE_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

class NotificationQueue {
private:
    struct Node {
        string message;
        string timestamp;
        string type;  // "info", "alert", "success", "warning"
        int priority; // 1 (high) to 3 (low)
        Node* next;
        
        Node(string msg, string msgType = "info", int msgPriority = 2) 
            : message(msg), type(msgType), priority(msgPriority), next(NULL) {
            timestamp = getCurrentTime();
        }
        
        string getCurrentTime() {
            time_t now = time(0);
            char* dt = ctime(&now);
            string timeStr = string(dt);
            return timeStr.substr(0, timeStr.length()-1); // Remove newline
        }
        
        string getPriorityString() {
            switch(priority) {
                case 1: return "HIGH";
                case 2: return "MEDIUM";
                case 3: return "LOW";
                default: return "MEDIUM";
            }
        }
        
        string getTypeIcon() {
            if (type == "alert") return "??";
            else if (type == "success") return "?";
            else if (type == "warning") return "??";
            else return "??";
        }
    };
    
    Node* front;
    Node* rear;
    int size;
    int maxSize; // Maximum notifications to store

public:
    NotificationQueue(int max = 50) : front(NULL), rear(NULL), size(0), maxSize(max) {}
    
    ~NotificationQueue() {
        // Destructor to clean up memory
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    // Enhanced enqueue with priority and type
    void enqueue(string message, string type = "info", int priority = 2) {
        if (size >= maxSize) {
            // Remove oldest low-priority notification if queue is full
            removeOldestLowPriority();
        }
        
        Node* newNode = new Node(message, type, priority);
        
        // Insert based on priority (high priority goes to front)
        if (isEmpty() || priority == 1) {
            newNode->next = front;
            front = newNode;
            if (rear == NULL) {
                rear = newNode;
            }
        } else {
            // Find insertion point for priority-based ordering
            Node* current = front;
            Node* previous = NULL;
            
            while (current != NULL && current->priority <= priority) {
                previous = current;
                current = current->next;
            }
            
            if (previous == NULL) {
                newNode->next = front;
                front = newNode;
            } else {
                previous->next = newNode;
                newNode->next = current;
                
                if (current == NULL) {
                    rear = newNode;
                }
            }
        }
        size++;
    }
    
    string dequeue() {
        if (isEmpty()) {
            return "QUEUE_EMPTY";
        }
        
        Node* temp = front;
        string message = front->message;
        front = front->next;
        
        if (front == NULL) {
            rear = NULL;
        }
        
        delete temp;
        size--;
        return message;
    }
    
    // Peek at front notification without removing
    string peek() {
        if (isEmpty()) {
            return "No notifications available";
        }
        return front->message;
    }
    
    // Get detailed front notification
    void getFrontDetails() {
        if (isEmpty()) {
            cout << "No notifications in queue" << endl;
            return;
        }
        
        cout << "Latest Notification:" << endl;
        cout << "+- Type: " << front->getTypeIcon() << " " << front->type << endl;
        cout << "+- Priority: " << front->getPriorityString() << endl;
        cout << "+- Message: " << front->message << endl;
        cout << "+- Time: " << front->timestamp << endl;
    }
    
    bool isEmpty() {
        return front == NULL;
    }
    
    // Enhanced display with formatting
    void display() {
        if (isEmpty()) {
            cout << "    ?? No notifications in queue" << endl;
            return;
        }
        
        cout << "    +---------------- NOTIFICATION QUEUE ----------------+" << endl;
        cout << "    ¦ Total: " << setw(3) << size << " notifications                            ¦" << endl;
        cout << "    +----------------------------------------------------¦" << endl;
        
        Node* current = front;
        int count = 1;
        
        while (current != NULL) {
            cout << "    ¦ " << setw(2) << count << ". " << current->getTypeIcon() << " " 
                 << setw(8) << left << current->getPriorityString() 
                 << " " << setw(35) << left 
                 << (current->message.length() > 35 ? current->message.substr(0, 32) + "..." : current->message)
                 << "¦" << endl;
                 
            if (current->message.length() > 35) {
                cout << "    ¦     " << setw(43) << left << current->message.substr(32) << "¦" << endl;
            }
            
            cout << "    ¦     " << setw(43) << left << current->timestamp << "¦" << endl;
            
            if (current->next != NULL) {
                cout << "    ¦                                                  ¦" << endl;
            }
            
            current = current->next;
            count++;
        }
        
        cout << "    +----------------------------------------------------+" << endl;
    }
    
    // Display only high priority notifications
    void displayHighPriority() {
        cout << "    +------------- HIGH PRIORITY ALERTS --------------+" << endl;
        
        Node* current = front;
        bool found = false;
        
        while (current != NULL) {
            if (current->priority == 1) {
                if (!found) {
                    found = true;
                }
                cout << "    ¦ ?? " << setw(45) << left << current->message << "¦" << endl;
                cout << "    ¦    " << setw(45) << left << current->timestamp << "¦" << endl;
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "    ¦         No high priority alerts            ¦" << endl;
        }
        cout << "    +------------------------------------------------+" << endl;
    }
    
    // Clear all notifications
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
        cout << "    ? Notification queue cleared successfully" << endl;
    }
    
    // Search notifications by keyword
    void search(string keyword) {
        cout << "    ?? Searching for: '" << keyword << "'" << endl;
        
        Node* current = front;
        bool found = false;
        int count = 1;
        
        while (current != NULL) {
            if (current->message.find(keyword) != string::npos) {
                if (!found) {
                    cout << "    +------------- SEARCH RESULTS --------------+" << endl;
                    found = true;
                }
                cout << "    ¦ " << setw(2) << count << ". " << setw(45) << left 
                     << current->message << "¦" << endl;
                count++;
            }
            current = current->next;
        }
        
        if (found) {
            cout << "    +--------------------------------------------+" << endl;
        } else {
            cout << "    ? No notifications found matching your search" << endl;
        }
    }
    
    // Get notification statistics
    void displayStats() {
        int highCount = 0, mediumCount = 0, lowCount = 0;
        int alertCount = 0, successCount = 0, warningCount = 0, infoCount = 0;
        
        Node* current = front;
        
        while (current != NULL) {
            // Count by priority
            switch(current->priority) {
                case 1: highCount++; break;
                case 2: mediumCount++; break;
                case 3: lowCount++; break;
            }
            
            // Count by type
            if (current->type == "alert") alertCount++;
            else if (current->type == "success") successCount++;
            else if (current->type == "warning") warningCount++;
            else infoCount++;
            
            current = current->next;
        }
        
        cout << "    +------------- NOTIFICATION STATS -------------+" << endl;
        cout << "    ¦ Priority: High(" << highCount << ") Medium(" << mediumCount 
             << ") Low(" << lowCount << ")        ¦" << endl;
        cout << "    ¦ Types: Alert(" << alertCount << ") Success(" << successCount 
             << ") Warning(" << warningCount << ") Info(" << infoCount << ") ¦" << endl;
        cout << "    ¦ Total: " << setw(3) << size << " / " << setw(3) << maxSize 
             << " notifications                  ¦" << endl;
        cout << "    +----------------------------------------------+" << endl;
    }
    
    int getSize() { return size; }
    int getMaxSize() { return maxSize; }
    
private:
    // Helper function to remove oldest low-priority notification
    void removeOldestLowPriority() {
        Node* current = front;
        Node* previous = NULL;
        Node* lowPriorityNode = NULL;
        Node* prevLowPriority = NULL;
        
        // Find the oldest low-priority notification
        while (current != NULL) {
            if (current->priority == 3) { // Low priority
                lowPriorityNode = current;
                prevLowPriority = previous;
                break; // Remove the first low priority found
            }
            previous = current;
            current = current->next;
        }
        
        if (lowPriorityNode != NULL) {
            // Remove the low priority node
            if (prevLowPriority == NULL) {
                front = lowPriorityNode->next;
            } else {
                prevLowPriority->next = lowPriorityNode->next;
            }
            
            if (lowPriorityNode == rear) {
                rear = prevLowPriority;
            }
            
            delete lowPriorityNode;
            size--;
        }
    }
};

#endif
