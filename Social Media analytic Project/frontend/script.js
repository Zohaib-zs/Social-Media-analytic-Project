const API_BASE_URL = window.location.protocol === "file:"
    ? "http://localhost:5000/api"
    : `${window.location.origin}/api`;

let systemStatus = "offline";

const teamMembers = [
    { id: 1, name: "Haris", initials: "HA", friends: [2, 3, 4] },
    { id: 2, name: "Zohaib", initials: "ZO", friends: [1, 3, 5] },
    { id: 3, name: "Zechriah", initials: "ZE", friends: [1, 2, 6] },
    { id: 4, name: "Azlan", initials: "AZ", friends: [1, 5] },
    { id: 5, name: "Kashan", initials: "KA", friends: [2, 4, 6] },
    { id: 6, name: "Ismail", initials: "IS", friends: [3, 5] }
];

const sampleData = {
    posts: [
        {
            id: 6,
            userId: 6,
            content: "Analytics page is ready with simple stats for our six-member sample network.",
            likes: 5,
            comments: 1,
            shares: 1,
            category: "tech",
            createdAt: "2026-05-09T10:42:00"
        },
        {
            id: 5,
            userId: 5,
            content: "Cleaned the feed categories so the linked list post view looks more organized.",
            likes: 4,
            comments: 2,
            shares: 0,
            category: "social",
            createdAt: "2026-05-09T10:34:00"
        },
        {
            id: 4,
            userId: 4,
            content: "Stack-based undo is now easy to explain on the dashboard during presentation.",
            likes: 6,
            comments: 1,
            shares: 1,
            category: "general",
            createdAt: "2026-05-09T10:26:00"
        },
        {
            id: 3,
            userId: 3,
            content: "Binary tree view makes the mutual friend part much clearer for the viva.",
            likes: 7,
            comments: 2,
            shares: 1,
            category: "academic",
            createdAt: "2026-05-09T10:18:00"
        },
        {
            id: 2,
            userId: 2,
            content: "Queue notifications tested successfully with sample alerts from our project group.",
            likes: 5,
            comments: 1,
            shares: 2,
            category: "tech",
            createdAt: "2026-05-09T10:10:00"
        },
        {
            id: 1,
            userId: 1,
            content: "Finished the semester project landing page and connected it with the demo dashboard.",
            likes: 8,
            comments: 3,
            shares: 2,
            category: "academic",
            createdAt: "2026-05-09T10:02:00"
        }
    ],
    notifications: [
        { id: 1, message: "Haris refreshed the demo dashboard.", type: "system", timestamp: "10:10 AM" },
        { id: 2, message: "Zohaib checked the queue notifications.", type: "social", timestamp: "10:18 AM" },
        { id: 3, message: "Zechriah ran the mutual friends test.", type: "update", timestamp: "10:26 AM" },
        { id: 4, message: "Demo data synced for all six team profiles.", type: "system", timestamp: "10:34 AM" }
    ],
    actions: [
        { id: 1, action: "Updated landing page", user: "Haris", timestamp: "10:42 AM" },
        { id: 2, action: "Reviewed queue alerts", user: "Zohaib", timestamp: "10:34 AM" },
        { id: 3, action: "Checked mutual friends", user: "Zechriah", timestamp: "10:26 AM" },
        { id: 4, action: "Updated analytics panel", user: "Azlan", timestamp: "10:18 AM" },
        { id: 5, action: "Reviewed posts feed", user: "Kashan", timestamp: "10:10 AM" },
        { id: 6, action: "Prepared final demo", user: "Ismail", timestamp: "10:02 AM" }
    ],
    network: {
        users: teamMembers,
        total_users: teamMembers.length,
        total_connections: 8
    }
};

const elements = {
    totalPosts: document.getElementById("totalPosts"),
    totalUsers: document.getElementById("totalUsers"),
    totalLikes: document.getElementById("totalLikes"),
    queueSize: document.getElementById("queueSize"),
    recentActions: document.getElementById("recentActions"),
    notificationsList: document.getElementById("notificationsList"),
    stackSize: document.getElementById("stackSize"),
    queueStatus: document.getElementById("queueStatus"),
    treeNodes: document.getElementById("treeNodes"),
    listCount: document.getElementById("listCount"),
    systemStatus: document.getElementById("systemStatus"),
    lastUpdated: document.getElementById("lastUpdated"),

    postsContainer: document.getElementById("postsContainer"),
    postsCount: document.getElementById("postsCount"),
    totalComments: document.getElementById("totalComments"),
    avgLikes: document.getElementById("avgLikes"),
    engagementRate: document.getElementById("engagementRate"),
    topPosts: document.getElementById("topPosts"),

    networkGraph: document.getElementById("networkGraph"),
    networkUsers: document.getElementById("networkUsers"),
    networkConnections: document.getElementById("networkConnections"),
    networkDensity: document.getElementById("networkDensity"),
    avgFriends: document.getElementById("avgFriends"),
    usersList: document.getElementById("usersList"),
    mutualResults: document.getElementById("mutualResults"),
    suggestionsResults: document.getElementById("suggestionsResults"),

    systemHealth: document.getElementById("systemHealth"),
    memoryUsage: document.getElementById("memoryUsage"),
    responseTime: document.getElementById("responseTime"),
    activeSessions: document.getElementById("activeSessions"),
    stackMetric: document.getElementById("stackMetric"),
    queueMetric: document.getElementById("queueMetric"),
    treeMetric: document.getElementById("treeMetric"),
    listMetric: document.getElementById("listMetric"),
    totalLikesCount: document.getElementById("totalLikesCount"),
    totalShares: document.getElementById("totalShares"),
    totalCommentsCount: document.getElementById("totalCommentsCount"),
    engagementRateAnalytics: document.getElementById("analyticsEngagementRate"),
    stackSizeDetail: document.getElementById("stackSizeDetail"),
    queueSizeDetail: document.getElementById("queueSizeDetail"),
    treeNodesDetail: document.getElementById("treeNodesDetail"),
    treeConnectionsDetail: document.getElementById("treeConnectionsDetail"),
    listNodesDetail: document.getElementById("listNodesDetail"),
    avgLikesDetail: document.getElementById("avgLikesDetail")
};

function formatTime(date) {
    return date.toLocaleTimeString([], { hour: "2-digit", minute: "2-digit" });
}

function formatDate(date) {
    return date.toISOString().split("T")[0];
}

function shortenText(text, maxLength) {
    return text.length > maxLength ? `${text.slice(0, maxLength - 3)}...` : text;
}

function getUserById(userId) {
    return sampleData.network.users.find(user => user.id === userId);
}

function getUserName(userId) {
    return getUserById(userId)?.name || `User ${userId}`;
}

function getUserInitials(userId) {
    return getUserById(userId)?.initials || `U${userId}`;
}

function getUserNames(userIds) {
    return userIds.map(getUserName).join(", ");
}

function calculateAverageFriends() {
    const total = sampleData.network.users.reduce((sum, user) => sum + user.friends.length, 0);
    return sampleData.network.users.length ? total / sampleData.network.users.length : 0;
}

function calculateNetworkDensityValue() {
    const users = sampleData.network.total_users;
    const maxConnections = users * (users - 1) / 2;
    return maxConnections ? sampleData.network.total_connections / maxConnections : 0;
}

function calculateTotalLikes(posts = sampleData.posts) {
    return posts.reduce((sum, post) => sum + post.likes, 0);
}

function calculateTotalComments(posts = sampleData.posts) {
    return posts.reduce((sum, post) => sum + post.comments, 0);
}

function calculateTotalShares(posts = sampleData.posts) {
    return posts.reduce((sum, post) => sum + post.shares, 0);
}

function calculateEngagementRate(posts = sampleData.posts) {
    if (!posts.length) {
        return "0%";
    }

    const totalInteractions = calculateTotalLikes(posts) + calculateTotalComments(posts) + calculateTotalShares(posts);
    return `${Math.round(totalInteractions / posts.length)}%`;
}

function getVisiblePosts() {
    let posts = [...sampleData.posts];

    const category = document.getElementById("categoryFilter")?.value || "all";
    const sortBy = document.getElementById("sortFilter")?.value || "recent";
    const query = (document.getElementById("searchInput")?.value || "").trim().toLowerCase();

    if (category !== "all") {
        posts = posts.filter(post => post.category === category);
    }

    if (query) {
        posts = posts.filter(post =>
            post.content.toLowerCase().includes(query) ||
            getUserName(post.userId).toLowerCase().includes(query)
        );
    }

    if (sortBy === "popular") {
        posts.sort((a, b) => b.likes - a.likes);
    } else if (sortBy === "oldest") {
        posts.sort((a, b) => new Date(a.createdAt) - new Date(b.createdAt));
    } else {
        posts.sort((a, b) => new Date(b.createdAt) - new Date(a.createdAt));
    }

    return posts;
}

function ensureNotificationStyles() {
    if (document.getElementById("notification-styles")) {
        return;
    }

    const style = document.createElement("style");
    style.id = "notification-styles";
    style.textContent = `
        .notification-toast {
            position: fixed;
            top: 18px;
            right: 18px;
            z-index: 1500;
            min-width: 260px;
            max-width: 340px;
            padding: 14px 16px;
            border-radius: 14px;
            background: #fffdfa;
            border: 1px solid #d9cfbf;
            box-shadow: 0 14px 30px rgba(25, 33, 44, 0.12);
            display: flex;
            align-items: center;
            gap: 10px;
            color: #1f2430;
            animation: toastSlide 0.2s ease;
        }
        .notification-toast.success { border-left: 4px solid #3d7554; }
        .notification-toast.error { border-left: 4px solid #a14444; }
        .notification-toast.warning { border-left: 4px solid #b55a36; }
        .notification-toast.info { border-left: 4px solid #1f4e5f; }
        .close-notification {
            margin-left: auto;
            border: none;
            background: none;
            color: #62675e;
            cursor: pointer;
            font-size: 1.2rem;
        }
        @keyframes toastSlide {
            from {
                opacity: 0;
                transform: translateY(-8px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }
    `;
    document.head.appendChild(style);
}

function showNotification(message, type = "info") {
    document.querySelectorAll(".notification-toast").forEach(notification => notification.remove());

    ensureNotificationStyles();

    const notification = document.createElement("div");
    notification.className = `notification-toast ${type}`;
    notification.innerHTML = `
        <i class="fas fa-${getNotificationIcon(type)}"></i>
        <span>${message}</span>
        <button class="close-notification" onclick="this.parentElement.remove()">&times;</button>
    `;

    document.body.appendChild(notification);

    setTimeout(() => {
        if (notification.parentElement) {
            notification.remove();
        }
    }, 3500);
}

function getNotificationIcon(type) {
    const icons = {
        success: "check-circle",
        error: "exclamation-circle",
        warning: "triangle-exclamation",
        info: "circle-info"
    };

    return icons[type] || "circle-info";
}

function updateSystemStatusLabel(label, isOnline) {
    if (!elements.systemStatus) {
        return;
    }

    elements.systemStatus.innerHTML = `
        <i class="fas fa-circle ${isOnline ? "online" : "offline"}"></i>
        ${label}
    `;
}

async function checkBackendHealth() {
    const endpoints = [`${API_BASE_URL}/health`, `${API_BASE_URL}/stats`];

    for (const endpoint of endpoints) {
        try {
            const response = await fetch(endpoint);
            if (!response.ok) {
                continue;
            }

            const data = await response.json();
            const backendAvailable =
                data.status === "healthy" ||
                data.cpp_backend === "Connected";

            if (backendAvailable) {
                systemStatus = "healthy";
                updateSystemStatusLabel("Backend connected", true);
                return true;
            }

            if (data.status === "running") {
                updateSystemStatusLabel("Server running, using demo data", false);
            }
        } catch (error) {
            // Continue to fallback endpoint.
        }
    }

    systemStatus = "offline";
    updateSystemStatusLabel("Demo mode (local sample data)", false);
    return false;
}

function updateDashboardStats() {
    if (elements.totalPosts) {
        elements.totalPosts.textContent = sampleData.posts.length;
    }
    if (elements.listCount) {
        elements.listCount.textContent = sampleData.posts.length;
    }
    if (elements.totalUsers) {
        elements.totalUsers.textContent = sampleData.network.total_users;
    }
    if (elements.treeNodes) {
        elements.treeNodes.textContent = sampleData.network.total_users;
    }
    if (elements.totalLikes) {
        elements.totalLikes.textContent = calculateTotalLikes();
    }
    if (elements.queueSize) {
        elements.queueSize.textContent = sampleData.notifications.length;
    }
    if (elements.queueStatus) {
        elements.queueStatus.textContent = sampleData.notifications.length;
    }
    if (elements.stackSize) {
        elements.stackSize.textContent = sampleData.actions.length;
    }
    if (elements.lastUpdated) {
        elements.lastUpdated.textContent = `Last updated: ${formatTime(new Date())}`;
    }
}

function updateRecentActions() {
    if (!elements.recentActions) {
        return;
    }

    elements.recentActions.innerHTML = sampleData.actions.slice(0, 5).map(action => `
        <div class="action-item">
            <div class="action-icon">
                <i class="fas fa-${getActionIcon(action.action)}"></i>
            </div>
            <div class="action-details">
                <div class="action-text">${action.action}</div>
                <div class="action-meta">${action.user} | ${action.timestamp}</div>
            </div>
        </div>
    `).join("");
}

function updateNotifications() {
    if (!elements.notificationsList) {
        return;
    }

    elements.notificationsList.innerHTML = sampleData.notifications.slice(0, 5).map(notification => `
        <div class="notification-item ${notification.type}">
            <div class="notification-icon">
                <i class="fas fa-${getNotificationTypeIcon(notification.type)}"></i>
            </div>
            <div class="notification-content">
                <div class="notification-text">${notification.message}</div>
                <div class="notification-time">${notification.timestamp}</div>
            </div>
        </div>
    `).join("");
}

function updatePostsPage() {
    if (!elements.postsContainer) {
        return;
    }

    const visiblePosts = getVisiblePosts();

    if (!visiblePosts.length) {
        elements.postsContainer.innerHTML = `<div class="empty-state">No posts match the current filters.</div>`;
    } else {
        elements.postsContainer.innerHTML = visiblePosts.map(post => `
            <div class="post-card">
                <div class="post-header">
                    <div class="post-avatar">${getUserInitials(post.userId)}</div>
                    <div>
                        <div class="post-author">${getUserName(post.userId)}</div>
                        <div class="post-time">${formatTime(new Date(post.createdAt))} | ${post.category.toUpperCase()}</div>
                    </div>
                </div>
                <div class="post-content">${post.content}</div>
                <div class="post-actions">
                    <button class="action-btn" onclick="likePost(${post.id})">
                        <i class="fas fa-thumbs-up"></i> ${post.likes}
                    </button>
                    <button class="action-btn" onclick="sharePost(${post.id})">
                        <i class="fas fa-share"></i> ${post.shares} Shares
                    </button>
                    <button class="action-btn" onclick="commentOnPost(${post.id})">
                        <i class="fas fa-comment"></i> ${post.comments} Comments
                    </button>
                </div>
            </div>
        `).join("");
    }

    if (elements.postsCount) {
        elements.postsCount.textContent = visiblePosts.length;
    }
    if (elements.totalComments) {
        elements.totalComments.textContent = calculateTotalComments(visiblePosts);
    }
    if (elements.avgLikes) {
        elements.avgLikes.textContent = visiblePosts.length ? Math.round(calculateTotalLikes(visiblePosts) / visiblePosts.length) : 0;
    }
    if (elements.engagementRate) {
        elements.engagementRate.textContent = calculateEngagementRate(visiblePosts);
    }

    if (elements.topPosts) {
        const topPosts = [...visiblePosts].sort((a, b) => b.likes - a.likes).slice(0, 3);

        elements.topPosts.innerHTML = topPosts.length ? topPosts.map((post, index) => `
            <div class="top-post-card">
                <div class="top-post-rank">#${index + 1}</div>
                <div class="top-post-content">
                    <div class="top-post-text">${shortenText(post.content, 65)}</div>
                    <div class="top-post-stats">
                        <span><i class="fas fa-user"></i> ${getUserName(post.userId)}</span>
                        <span><i class="fas fa-thumbs-up"></i> ${post.likes}</span>
                        <span class="post-category">${post.category}</span>
                    </div>
                </div>
            </div>
        `).join("") : `<div class="empty-state">No top posts available for the current view.</div>`;
    }
}

function updateNetworkPage() {
    const density = calculateNetworkDensityValue();
    const avgFriends = calculateAverageFriends();

    if (elements.networkUsers) {
        elements.networkUsers.textContent = sampleData.network.total_users;
    }
    if (elements.networkConnections) {
        elements.networkConnections.textContent = sampleData.network.total_connections;
    }
    if (elements.networkDensity) {
        elements.networkDensity.textContent = density.toFixed(2);
    }
    if (elements.avgFriends) {
        elements.avgFriends.textContent = avgFriends.toFixed(2);
    }

    if (elements.usersList) {
        elements.usersList.innerHTML = sampleData.network.users.map(user => `
            <div class="user-item">
                <div class="user-avatar-small">${user.initials}</div>
                <div class="user-details">
                    <div class="user-name">${user.name}</div>
                    <div class="user-friends">${user.friends.length} friends</div>
                </div>
            </div>
        `).join("");
    }

    if (elements.networkGraph) {
        elements.networkGraph.innerHTML = `
            <div class="network-map">
                ${sampleData.network.users.map(user => `
                    <div class="graph-user-card">
                        <div class="graph-user-header">
                            <div class="user-avatar-small">${user.initials}</div>
                            <div>
                                <div class="user-name">${user.name}</div>
                                <div class="user-friends">${user.friends.length} direct links</div>
                            </div>
                        </div>
                        <div class="connection-list">Connected with: ${getUserNames(user.friends)}</div>
                    </div>
                `).join("")}
            </div>
        `;
    }
}

function updateAnalyticsPage() {
    const totalLikes = calculateTotalLikes();
    const totalComments = calculateTotalComments();
    const totalShares = calculateTotalShares();
    const averageLikes = sampleData.posts.length ? Math.round(totalLikes / sampleData.posts.length) : 0;
    const density = calculateNetworkDensityValue();
    const memoryEstimate = 46 + sampleData.posts.length + sampleData.notifications.length + sampleData.actions.length;
    const responseEstimate = 26 + sampleData.actions.length;

    if (elements.systemHealth) {
        elements.systemHealth.textContent = "Stable";
    }
    if (elements.memoryUsage) {
        elements.memoryUsage.textContent = `${memoryEstimate}%`;
    }
    if (elements.responseTime) {
        elements.responseTime.textContent = `${responseEstimate}ms`;
    }
    if (elements.activeSessions) {
        elements.activeSessions.textContent = sampleData.network.total_users;
    }

    if (elements.stackMetric) {
        elements.stackMetric.textContent = `${sampleData.actions.length}/100 (${Math.round(sampleData.actions.length)}%)`;
    }
    if (elements.queueMetric) {
        elements.queueMetric.textContent = `${sampleData.notifications.length}/50 (${Math.round(sampleData.notifications.length * 2)}%)`;
    }
    if (elements.treeMetric) {
        elements.treeMetric.textContent = `${sampleData.network.total_users} nodes`;
    }
    if (elements.listMetric) {
        elements.listMetric.textContent = `${sampleData.posts.length} posts`;
    }

    if (elements.totalLikesCount) {
        elements.totalLikesCount.textContent = totalLikes;
    }
    if (elements.totalShares) {
        elements.totalShares.textContent = totalShares;
    }
    if (elements.totalCommentsCount) {
        elements.totalCommentsCount.textContent = totalComments;
    }
    if (elements.engagementRateAnalytics) {
        elements.engagementRateAnalytics.textContent = calculateEngagementRate();
    }

    if (elements.stackSizeDetail) {
        elements.stackSizeDetail.textContent = `${sampleData.actions.length} actions`;
    }
    if (elements.queueSizeDetail) {
        elements.queueSizeDetail.textContent = `${sampleData.notifications.length} notifications`;
    }
    if (elements.treeNodesDetail) {
        elements.treeNodesDetail.textContent = `${sampleData.network.total_users} users`;
    }
    if (elements.treeConnectionsDetail) {
        elements.treeConnectionsDetail.textContent = `${sampleData.network.total_connections} friendships`;
        elements.treeConnectionsDetail.setAttribute("data-density", density.toFixed(2));
    }
    if (elements.listNodesDetail) {
        elements.listNodesDetail.textContent = `${sampleData.posts.length} posts`;
    }
    if (elements.avgLikesDetail) {
        elements.avgLikesDetail.textContent = `${averageLikes} likes/post`;
    }

    const stackPercent = sampleData.actions.length;
    const queuePercent = sampleData.notifications.length * 2;
    const treePercent = Math.round((sampleData.network.total_users / 10) * 100);
    const listPercent = Math.round((sampleData.posts.length / 10) * 100);

    if (document.getElementById("stackBar")) {
        document.getElementById("stackBar").style.width = `${stackPercent}%`;
    }
    if (document.getElementById("queueBar")) {
        document.getElementById("queueBar").style.width = `${queuePercent}%`;
    }
    if (document.getElementById("treeBar")) {
        document.getElementById("treeBar").style.width = `${treePercent}%`;
    }
    if (document.getElementById("listBar")) {
        document.getElementById("listBar").style.width = `${listPercent}%`;
    }
}

function getActionIcon(action) {
    const lowerAction = action.toLowerCase();

    if (lowerAction.includes("post")) return "pen";
    if (lowerAction.includes("queue") || lowerAction.includes("alert")) return "stream";
    if (lowerAction.includes("mutual") || lowerAction.includes("network")) return "users";
    if (lowerAction.includes("analytics")) return "chart-column";
    if (lowerAction.includes("demo")) return "play";
    return "circle";
}

function getNotificationTypeIcon(type) {
    const icons = {
        system: "gear",
        social: "users",
        update: "bell",
        alert: "triangle-exclamation"
    };

    return icons[type] || "bell";
}

function createNewPost() {
    const modal = document.getElementById("postModal");
    if (modal) {
        modal.style.display = "flex";
    }
}

function closeModal() {
    const modal = document.getElementById("postModal");
    if (modal) {
        modal.style.display = "none";
    }

    const postContent = document.getElementById("postContent");
    if (postContent) {
        postContent.value = "";
    }
}

async function submitPost() {
    const contentInput = document.getElementById("postContent");
    const categorySelect = document.getElementById("postCategory");

    if (!contentInput || !categorySelect) {
        return;
    }

    const content = contentInput.value.trim();
    const category = categorySelect.value;

    if (!content) {
        showNotification("Please enter post content.", "warning");
        return;
    }

    const newPost = {
        id: sampleData.posts.length + 1,
        userId: 1,
        content,
        likes: 0,
        comments: 0,
        shares: 0,
        category,
        createdAt: new Date().toISOString()
    };

    sampleData.posts.unshift(newPost);
    sampleData.actions.unshift({
        id: sampleData.actions.length + 1,
        action: "Created post",
        user: "Haris",
        timestamp: formatTime(new Date())
    });
    sampleData.notifications.unshift({
        id: sampleData.notifications.length + 1,
        message: "Haris published a new project post.",
        type: "update",
        timestamp: formatTime(new Date())
    });

    closeModal();
    refreshData(false);
    showNotification("Post created successfully.", "success");
}

function likePost(postId) {
    const post = sampleData.posts.find(item => item.id === postId);
    if (!post) {
        return;
    }

    post.likes += 1;
    sampleData.actions.unshift({
        id: sampleData.actions.length + 1,
        action: "Liked post",
        user: "Haris",
        timestamp: formatTime(new Date())
    });

    refreshData(false);
    showNotification("Post liked.", "success");
}

function sharePost(postId) {
    const post = sampleData.posts.find(item => item.id === postId);
    if (!post) {
        return;
    }

    post.shares += 1;
    sampleData.actions.unshift({
        id: sampleData.actions.length + 1,
        action: "Shared post",
        user: "Haris",
        timestamp: formatTime(new Date())
    });

    refreshData(false);
    showNotification("Post shared.", "success");
}

function commentOnPost(postId) {
    const comment = prompt("Enter your comment:");
    const post = sampleData.posts.find(item => item.id === postId);

    if (!comment || !post) {
        return;
    }

    post.comments += 1;
    sampleData.actions.unshift({
        id: sampleData.actions.length + 1,
        action: "Commented on post",
        user: "Haris",
        timestamp: formatTime(new Date())
    });

    refreshData(false);
    showNotification("Comment added.", "success");
}

function undoLastAction() {
    if (!sampleData.actions.length) {
        showNotification("No actions left to undo.", "warning");
        return;
    }

    sampleData.actions.shift();
    refreshData(false);
    showNotification("Last action removed from history.", "success");
}

function renderResultCard(target, title, body) {
    if (!target) {
        return;
    }

    target.innerHTML = `
        <div class="result-card">
            <h4>${title}</h4>
            ${body}
        </div>
    `;
}

function findMutualFriends() {
    findMutual();
}

function findMutual() {
    const userOneId = parseInt(document.getElementById("user1")?.value || "1", 10);
    const userTwoId = parseInt(document.getElementById("user2")?.value || "2", 10);

    if (userOneId === userTwoId) {
        showNotification("Please choose two different users.", "warning");
        return;
    }

    const userOne = getUserById(userOneId);
    const userTwo = getUserById(userTwoId);
    const mutualFriends = userOne.friends.filter(friendId => userTwo.friends.includes(friendId));

    if (!mutualFriends.length) {
        renderResultCard(
            elements.mutualResults,
            `${userOne.name} and ${userTwo.name}`,
            "<p>No mutual friends found in the current demo dataset.</p>"
        );
        showNotification("No mutual friends found.", "info");
        return;
    }

    renderResultCard(
        elements.mutualResults,
        `${userOne.name} and ${userTwo.name}`,
        `
            <div class="result-badges">
                ${mutualFriends.map(friendId => `<span class="result-badge">${getUserName(friendId)}</span>`).join("")}
            </div>
            <p>${mutualFriends.length} mutual connection(s) found.</p>
        `
    );

    showNotification(`Found ${mutualFriends.length} mutual friend(s).`, "success");
}

function getSuggestions() {
    const userId = parseInt(document.getElementById("suggestForUser")?.value || "1", 10);
    const currentUser = getUserById(userId);

    const suggestions = sampleData.network.users
        .filter(user => user.id !== userId && !currentUser.friends.includes(user.id))
        .map(user => ({
            user,
            score: user.friends.filter(friendId => currentUser.friends.includes(friendId)).length
        }))
        .filter(item => item.score > 0)
        .sort((a, b) => b.score - a.score || a.user.name.localeCompare(b.user.name));

    if (!suggestions.length) {
        renderResultCard(
            elements.suggestionsResults,
            `Suggestions for ${currentUser.name}`,
            "<p>No strong suggestions available right now.</p>"
        );
        showNotification("No friend suggestions available.", "info");
        return;
    }

    renderResultCard(
        elements.suggestionsResults,
        `Suggestions for ${currentUser.name}`,
        suggestions.map(item => `
            <div class="suggestion-user">
                <span>${item.user.name} | ${item.score} mutual link(s)</span>
                <button class="btn btn-secondary btn-small" onclick="sendFriendRequest(${item.user.id})">Add</button>
            </div>
        `).join("")
    );

    showNotification(`Found ${suggestions.length} suggestion(s).`, "success");
}

function sendFriendRequest(userId) {
    showNotification(`Friend request sent to ${getUserName(userId)}.`, "success");
}

function displayTree() {
    const treeDiagram = document.getElementById("treeDiagram");
    if (treeDiagram) {
        treeDiagram.scrollIntoView({ behavior: "smooth", block: "center" });
    }
    showNotification("Tree layout highlighted below.", "info");
}

function findMostConnected() {
    const mostConnectedUser = [...sampleData.network.users].sort((a, b) => b.friends.length - a.friends.length)[0];

    renderResultCard(
        elements.suggestionsResults,
        "Most Connected Member",
        `<p>${mostConnectedUser.name} has the highest number of connections with ${mostConnectedUser.friends.length} friend link(s).</p>`
    );

    showNotification(`${mostConnectedUser.name} is the most connected member.`, "success");
}

function calculateDensity() {
    const density = calculateNetworkDensityValue().toFixed(2);
    showNotification(`Current network density: ${density}`, "info");
}

function filterPosts() {
    updatePostsPage();
    showNotification("Post filter applied.", "info");
}

function sortPosts() {
    updatePostsPage();
    showNotification("Post order updated.", "info");
}

function searchPosts() {
    updatePostsPage();
    showNotification("Search results refreshed.", "info");
}

function exportData() {
    const dataStr = JSON.stringify(sampleData, null, 2);
    const dataUri = `data:application/json;charset=utf-8,${encodeURIComponent(dataStr)}`;
    const fileName = `dsa_social_lab_${formatDate(new Date())}.json`;

    const link = document.createElement("a");
    link.setAttribute("href", dataUri);
    link.setAttribute("download", fileName);
    link.click();

    showNotification("JSON export created.", "success");
}

function markAllAsRead() {
    document.querySelectorAll(".notification-item").forEach(item => item.classList.add("read"));
    showNotification("Notifications marked as read.", "success");
}

function clearHistory() {
    if (!confirm("Clear the action history from this demo?")) {
        return;
    }

    sampleData.actions = [];
    refreshData(false);
    showNotification("Action history cleared.", "success");
}

function runDemo() {
    showNotification("Running short demo sequence...", "info");

    setTimeout(() => {
        sampleData.actions.unshift({
            id: sampleData.actions.length + 1,
            action: "Demo: Added sample post",
            user: "System",
            timestamp: formatTime(new Date())
        });
        refreshData(false);
    }, 700);

    setTimeout(() => {
        sampleData.actions.unshift({
            id: sampleData.actions.length + 1,
            action: "Demo: Reviewed queue alerts",
            user: "System",
            timestamp: formatTime(new Date())
        });
        refreshData(false);
    }, 1400);

    setTimeout(() => {
        sampleData.actions.unshift({
            id: sampleData.actions.length + 1,
            action: "Demo: Updated network links",
            user: "System",
            timestamp: formatTime(new Date())
        });
        refreshData(false);
        showNotification("Demo sequence completed.", "success");
    }, 2100);
}

function refreshData(showToast = true) {
    updateDashboardStats();
    updateRecentActions();
    updateNotifications();
    updatePostsPage();
    updateNetworkPage();
    updateAnalyticsPage();

    if (showToast) {
        showNotification("Data refreshed.", "success");
    }
}

function viewNetwork() {
    window.location.href = "network.html";
}

function showAnalytics() {
    window.location.href = "analytics.html";
}

document.addEventListener("DOMContentLoaded", async () => {
    await checkBackendHealth();
    refreshData(false);
    setInterval(() => refreshData(false), 30000);
});

window.onclick = function(event) {
    const modal = document.getElementById("postModal");
    if (modal && event.target === modal) {
        closeModal();
    }
};

document.addEventListener("keydown", event => {
    if (event.ctrlKey && event.key.toLowerCase() === "r") {
        event.preventDefault();
        refreshData();
    }
});
