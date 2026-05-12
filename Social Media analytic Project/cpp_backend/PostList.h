#ifndef POSTLIST_H
#define POSTLIST_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

class PostList {
private:
    struct Comment {
        int commentId;
        int userId;
        string content;
        string timestamp;
        Comment* next;
        
        Comment(int cid, int uid, string cont) : commentId(cid), userId(uid), content(cont), next(NULL) {
            // Direct time code instead of calling getCurrentTime()
            time_t now = time(0);
            char* dt = ctime(&now);
            timestamp = string(dt);
            // Remove newline character
            if (!timestamp.empty() && timestamp[timestamp.length()-1] == '\n') {
                timestamp = timestamp.substr(0, timestamp.length()-1);
            }
        }
    };

    struct PostNode {
        int postId;
        int userId;
        string content;
        string author;
        int likes;
        int shares;
        int views;
        string timestamp;
        string category; // "general", "tech", "academic", "social"
        bool isPinned;
        Comment* comments;
        PostNode* next;
        
        PostNode(int id, int uid, string cont, string cat = "general") {
            postId = id;
            userId = uid;
            content = cont;
            // Replace to_string with stringstream
            stringstream ss;
            ss << uid;
            author = "User_" + ss.str();
            likes = 0;
            shares = 0;
            views = 0;
            category = cat;
            isPinned = false;
            comments = NULL;
            // Direct time code
            time_t now = time(0);
            char* dt = ctime(&now);
            timestamp = string(dt);
            // Remove newline character
            if (!timestamp.empty() && timestamp[timestamp.length()-1] == '\n') {
                timestamp = timestamp.substr(0, timestamp.length()-1);
            }
            next = NULL;
        }
        
        string getCategoryIcon() {
            if (category == "tech") return "[TECH]";
            else if (category == "academic") return "[EDU]";
            else if (category == "social") return "[SOCIAL]";
            else return "[POST]";
        }
        
        string getEngagementRate() {
            if (views == 0) return "0%";
            double rate = (double(likes + shares) / views) * 100;
            stringstream ss;
            ss << fixed << setprecision(1) << rate << "%";
            return ss.str();
        }
    };
    
    PostNode* head;
    int postCount;
    int totalLikes;
    int totalViews;

    // Comparison function as private member
    static bool comparePostsByLikes(PostNode* a, PostNode* b) {
        return a->likes > b->likes;
    }

public:
    PostList() : head(NULL), postCount(0), totalLikes(0), totalViews(0) {}
    
    ~PostList() {
        // Clean up all posts and comments
        PostNode* current = head;
        while (current != NULL) {
            PostNode* next = current->next;
            deletePostComments(current);
            delete current;
            current = next;
        }
    }

    // Enhanced post creation with categories
    void addPost(int userId, string content, string category = "general") {
        PostNode* newPost = new PostNode(++postCount, userId, content, category);
        
        if (head == NULL) {
            head = newPost;
        } else {
            // Insert at beginning for chronological order (newest first)
            newPost->next = head;
            head = newPost;
        }
        
        cout << "    [SUCCESS] Post published successfully!" << endl;
        cout << "    |- Post ID: #" << newPost->postId << endl;
        cout << "    |- Category: " << newPost->getCategoryIcon() << " " << newPost->category << endl;
        cout << "    |- Timestamp: " << newPost->timestamp << endl;
    }

    // Pin important posts
    void pinPost(int postId) {
        PostNode* post = findPost(postId);
        if (post != NULL) {
            post->isPinned = true;
            cout << "    [PIN] Post #" << postId << " pinned to top" << endl;
        } else {
            cout << "    [ERROR] Post not found!" << endl;
        }
    }

    void unpinPost(int postId) {
        PostNode* post = findPost(postId);
        if (post != NULL) {
            post->isPinned = false;
            cout << "    [UNPIN] Post #" << postId << " unpinned" << endl;
        } else {
            cout << "    [ERROR] Post not found!" << endl;
        }
    }

    // Enhanced display with professional formatting
    void displayPosts(bool showPinnedFirst = true) {
        if (head == NULL) {
            cout << "    [INFO] No posts available in feed" << endl;
            return;
        }

        cout << "    +------------------ POSTS FEED -------------------+" << endl;
        cout << "    | Showing " << setw(2) << postCount << " posts | " << totalLikes << " total likes |" << endl;
        cout << "    +--------------------------------------------------+" << endl;

        // Display pinned posts first
        if (showPinnedFirst) {
            PostNode* current = head;
            bool hasPinned = false;
            
            while (current != NULL) {
                if (current->isPinned) {
                    if (!hasPinned) {
                        cout << "\n    [PINNED] PINNED POSTS:" << endl;
                        hasPinned = true;
                    }
                    displaySinglePost(current);
                }
                current = current->next;
            }
            
            if (hasPinned) {
                cout << "    ---------------------------------------------------" << endl;
            }
        }

        // Display all posts
        cout << "\n    [RECENT] RECENT POSTS:" << endl;
        PostNode* current = head;
        int displayCount = 0;
        
        while (current != NULL) {
            if (!current->isPinned || !showPinnedFirst) {
                displaySinglePost(current);
                displayCount++;
            }
            current = current->next;
        }
    }

    // Display single post with beautiful formatting
    void displaySinglePost(PostNode* post) {
        cout << "\n    +------------------ POST #" << setw(3) << post->postId << " -----------------+" << endl;
        cout << "    | " << post->getCategoryIcon() << " " << setw(15) << left << post->category 
             << " [PIN] " << (post->isPinned ? "YES" : "NO ") 
             << "                  |" << endl;
        cout << "    | [USER] " << setw(45) << left << post->author << "|" << endl;
        cout << "    | [TIME] " << setw(45) << left << post->timestamp << "|" << endl;
        cout << "    +--------------------------------------------------|" << endl;
        
        // Display content with word wrapping
        string content = post->content;
        int pos = 0;
        while (pos < content.length()) {
            int chunkSize = 44;
            if (pos + chunkSize > content.length()) {
                chunkSize = content.length() - pos;
            }
            string line = content.substr(pos, chunkSize);
            cout << "    | " << setw(45) << left << line << " |" << endl;
            pos += chunkSize;
        }
        
        cout << "    +--------------------------------------------------|" << endl;
        cout << "    | [LIKES] " << setw(4) << post->likes << "   [SHARES] " << setw(4) << post->shares 
             << "   [VIEWS] " << setw(4) << post->views << "   [RATE] " << setw(6) << post->getEngagementRate() << " |" << endl;
        
        // Display comment count
        int commentCount = getCommentCount(post);
        if (commentCount > 0) {
            cout << "    | [COMMENTS] " << commentCount << " comments                              |" << endl;
        }
        
        cout << "    +--------------------------------------------------+" << endl;
    }

    // Advanced post search and filtering
    void searchPosts(string keyword) {
        cout << "    [SEARCH] Searching posts for: '" << keyword << "'" << endl;
        
        PostNode* current = head;
        bool found = false;
        
        while (current != NULL) {
            if (current->content.find(keyword) != string::npos) {
                if (!found) {
                    cout << "    +------------- SEARCH RESULTS -------------+" << endl;
                    found = true;
                }
                displaySinglePost(current);
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "    [ERROR] No posts found matching your search" << endl;
        }
    }

    void filterByCategory(string category) {
        cout << "    [FILTER] Showing posts in category: " << category << endl;
        
        PostNode* current = head;
        bool found = false;
        
        while (current != NULL) {
            if (current->category == category) {
                if (!found) {
                    found = true;
                }
                displaySinglePost(current);
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "    [ERROR] No posts found in this category" << endl;
        }
    }

    // Enhanced like system
    void likePost(int postId) {
        PostNode* post = findPost(postId);
        if (post != NULL) {
            post->likes++;
            totalLikes++;
            post->views++; // Count as view when liked
            cout << "    [SUCCESS] Post #" << postId << " liked! Total: " << post->likes << " likes" << endl;
        } else {
            cout << "    [ERROR] Post not found!" << endl;
        }
    }

    void unlikePost(int postId) {
        PostNode* post = findPost(postId);
        if (post != NULL && post->likes > 0) {
            post->likes--;
            totalLikes--;
            cout << "    [SUCCESS] Post #" << postId << " unliked! Total: " << post->likes << " likes" << endl;
        } else {
            cout << "    [ERROR] Post not found or no likes to remove!" << endl;
        }
    }

    // Share functionality
    void sharePost(int postId) {
        PostNode* post = findPost(postId);
        if (post != NULL) {
            post->shares++;
            cout << "    [SHARE] Post #" << postId << " shared! Total shares: " << post->shares << endl;
        } else {
            cout << "    [ERROR] Post not found!" << endl;
        }
    }

    // View tracking
    void viewPost(int postId) {
        PostNode* post = findPost(postId);
        if (post != NULL) {
            post->views++;
            totalViews++;
        }
    }

    // Comment system
    void addComment(int postId, int userId, string comment) {
        PostNode* post = findPost(postId);
        if (post != NULL) {
            Comment* newComment = new Comment(getCommentCount(post) + 1, userId, comment);
            
            if (post->comments == NULL) {
                post->comments = newComment;
            } else {
                Comment* current = post->comments;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newComment;
            }
            
            cout << "    [COMMENT] Comment added to post #" << postId << endl;
        } else {
            cout << "    [ERROR] Post not found!" << endl;
        }
    }

    void displayComments(int postId) {
        PostNode* post = findPost(postId);
        if (post != NULL && post->comments != NULL) {
            cout << "    +------------ COMMENTS ON POST #" << postId << " ------------+" << endl;
            
            Comment* current = post->comments;
            while (current != NULL) {
                cout << "    | [USER] User_" << setw(3) << current->userId << ": " 
                     << setw(35) << left << current->content << "|" << endl;
                cout << "    |       " << setw(41) << left << current->timestamp << "|" << endl;
                
                if (current->next != NULL) {
                    cout << "    |                                                  |" << endl;
                }
                current = current->next;
            }
            
            cout << "    +--------------------------------------------------+" << endl;
        } else {
            cout << "    [INFO] No comments on this post" << endl;
        }
    }

    // Analytics and statistics
    void displayPostStats() {
        cout << "    +----------------- POST ANALYTICS -----------------+" << endl;
        cout << "    | Total Posts:    " << setw(30) << right << postCount << " |" << endl;
        cout << "    | Total Likes:    " << setw(30) << right << totalLikes << " |" << endl;
        cout << "    | Total Views:    " << setw(30) << right << totalViews << " |" << endl;
        cout << "    | Avg Likes/Post: " << setw(30) << right 
             << (postCount > 0 ? totalLikes/postCount : 0) << " |" << endl;
        
        // Category breakdown
        cout << "    +----------------- CATEGORIES ---------------------|" << endl;
        displayCategoryStats();
        
        cout << "    +--------------------------------------------------+" << endl;
    }

    // Find most popular post
    void displayTopPosts(int count = 3) {
        vector<PostNode*> posts;
        PostNode* current = head;
        
        while (current != NULL) {
            posts.push_back(current);
            current = current->next;
        }
        
        // Use member comparison function
        sort(posts.begin(), posts.end(), comparePostsByLikes);
        
        cout << "    +--------------- TOP " << count << " POSTS ----------------+" << endl;
        int displayCount = count;
        if (posts.size() < count) {
            displayCount = posts.size();
        }
        for (int i = 0; i < displayCount; i++) {
            cout << "    | " << (i+1) << ". Post #" << setw(3) << posts[i]->postId 
                 << " - " << setw(4) << posts[i]->likes << " likes" 
                 << setw(24) << " |" << endl;
        }
        cout << "    +--------------------------------------------------+" << endl;
    }

    // Utility functions
    PostNode* findPost(int postId) {
        PostNode* current = head;
        while (current != NULL) {
            if (current->postId == postId) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    int getPostCount() { return postCount; }
    int getTotalLikes() { return totalLikes; }

private:
    int getCommentCount(PostNode* post) {
        int count = 0;
        Comment* current = post->comments;
        while (current != NULL) {
            count++;
            current = current->next;
        }
        return count;
    }

    void deletePostComments(PostNode* post) {
        Comment* current = post->comments;
        while (current != NULL) {
            Comment* next = current->next;
            delete current;
            current = next;
        }
    }

    void displayCategoryStats() {
        // Initialize vector properly for C++98
        vector<string> categories;
        categories.push_back("general");
        categories.push_back("tech");
        categories.push_back("academic");
        categories.push_back("social");
        
        vector<int> counts(4, 0);
        vector<int> likes(4, 0);
        
        PostNode* current = head;
        while (current != NULL) {
            for (int i = 0; i < (int)categories.size(); i++) {
                if (current->category == categories[i]) {
                    counts[i]++;
                    likes[i] += current->likes;
                    break;
                }
            }
            current = current->next;
        }
        
        for (int i = 0; i < (int)categories.size(); i++) {
            if (counts[i] > 0) {
                cout << "    | " << setw(10) << left << categories[i] << ": " 
                     << setw(3) << counts[i] << " posts, " 
                     << setw(4) << likes[i] << " likes" 
                     << setw(15) << " |" << endl;
            }
        }
    }
};

#endif
