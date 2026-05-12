import http.server
import socketserver
import subprocess
import json
import os
import sys

PORT = 8080

# ==================== PATHS CONFIGURATION ====================
# CHANGE THESE PATHS TO MATCH YOUR SYSTEM!

# Path to your frontend folder (HTML/CSS)
FRONTEND_PATH = r"C:\Users\Harris\Desktop\project\frontend"

# Path to your compiled C++ executable
# First compile main_web.cpp: g++ main_web.cpp -o social_web.exe
CPP_EXE_PATH = r"C:\project\cpp_backend\social_web.exe"

# ==================== WEB SERVER ====================

class SocialMediaHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=FRONTEND_PATH, **kwargs)
    
    # Enable CORS
    def end_headers(self):
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        super().end_headers()
    
    def do_OPTIONS(self):
        self.send_response(200)
        self.end_headers()
    
    def do_GET(self):
        """Handle GET requests - serve HTML or API endpoints"""
        api_routes = {
            '/api/system': self._handle_system,
            '/api/posts': self._handle_posts,
            '/api/friends': self._handle_friends,
            '/api/notifications': self._handle_notifications,
            '/api/stats': self._handle_stats
        }
        
        if self.path in api_routes:
            api_routes[self.path]()
        elif self.path == '/':
            # Serve index.html
            self.path = '/index.html'
            super().do_GET()
        elif self.path.endswith(('.html', '.css', '.js', '.png', '.jpg')):
            # Serve other files
            super().do_GET()
        else:
            # Default API response
            self.send_response(404)
            self.end_headers()
            self.wfile.write(json.dumps({
                "error": "Endpoint not found",
                "available": list(api_routes.keys())
            }).encode())
    
    def do_POST(self):
        """Handle POST requests - send to C++ backend"""
        if self.path == '/api/command':
            try:
                # Read JSON from request
                content_length = int(self.headers['Content-Length'])
                post_data = self.rfile.read(content_length).decode('utf-8')
                
                # Call C++ backend
                result = self._call_cpp_backend(post_data)
                
                # Send response
                self.send_response(200)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                self.wfile.write(result.encode())
                
            except Exception as e:
                self.send_response(500)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                self.wfile.write(json.dumps({
                    "error": str(e),
                    "type": "server_error"
                }).encode())
        else:
            self.send_response(404)
            self.end_headers()
    
    def _call_cpp_backend(self, json_data):
        """Call the C++ executable and get response"""
        try:
            if not os.path.exists(CPP_EXE_PATH):
                return json.dumps({
                    "error": "C++ backend not found",
                    "path": CPP_EXE_PATH,
                    "tip": "Compile with: g++ main_web.cpp -o social_web.exe"
                })
            
            # Run C++ program with JSON input
            process = subprocess.run(
                [CPP_EXE_PATH],
                input=json_data,
                capture_output=True,
                text=True,
                timeout=10,
                encoding='utf-8'
            )
            
            if process.returncode == 0:
                return process.stdout.strip()
            else:
                return json.dumps({
                    "error": "C++ backend error",
                    "stderr": process.stderr,
                    "code": process.returncode
                })
                
        except subprocess.TimeoutExpired:
            return json.dumps({"error": "C++ backend timeout"})
        except Exception as e:
            return json.dumps({"error": f"C++ call failed: {str(e)}"})
    
    # API Handlers
    def _handle_system(self):
        """Get system information"""
        result = self._call_cpp_backend('{"action":"get_system_info"}')
        self._send_json_response(result)
    
    def _handle_posts(self):
        """Get all posts"""
        result = self._call_cpp_backend('{"action":"get_posts"}')
        self._send_json_response(result)
    
    def _handle_friends(self):
        """Get all friends"""
        result = self._call_cpp_backend('{"action":"get_friends"}')
        self._send_json_response(result)
    
    def _handle_notifications(self):
        """Get all notifications"""
        result = self._call_cpp_backend('{"action":"get_notifications"}')
        self._send_json_response(result)
    
    def _handle_stats(self):
        """Get system statistics"""
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps({
            "server": "Python HTTP Server",
            "cpp_backend": "Connected" if os.path.exists(CPP_EXE_PATH) else "Not found",
            "port": PORT,
            "status": "running"
        }).encode())
    
    def _send_json_response(self, data):
        """Helper to send JSON response"""
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(data.encode())

# ==================== STARTUP ====================

def check_paths():
    """Check if all required paths exist"""
    print("=" * 60)
    print("🔍 Checking System Configuration")
    print("=" * 60)
    
    # Check frontend
    frontend_index = os.path.join(FRONTEND_PATH, "index.html")
    if os.path.exists(frontend_index):
        print(f"✅ Frontend: {frontend_index}")
    else:
        print(f"❌ Frontend not found: {frontend_index}")
        print(f"   Creating basic index.html...")
        create_basic_html(frontend_index)
    
    # Check C++ backend
    if os.path.exists(CPP_EXE_PATH):
        print(f"✅ C++ Backend: {CPP_EXE_PATH}")
    else:
        print(f"⚠️  C++ Backend not found: {CPP_EXE_PATH}")
        print(f"   You need to compile it with: g++ main_web.cpp -o social_web.exe")
        print(f"   Expected at: {CPP_EXE_PATH}")
    
    print("=" * 60)
    return True

def create_basic_html(filepath):
    """Create a basic HTML file if none exists"""
    html_content = """<!DOCTYPE html>
<html>
<head>
    <title>Social Media Platform</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            max-width: 1000px;
            margin: 0 auto;
            padding: 20px;
            background: #f0f2f5;
        }
        .header {
            background: #1877f2;
            color: white;
            padding: 20px;
            border-radius: 10px;
            margin-bottom: 20px;
        }
        .container {
            display: grid;
            grid-template-columns: 2fr 1fr;
            gap: 20px;
        }
        .panel {
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        button {
            background: #1877f2;
            color: white;
            border: none;
            padding: 10px 15px;
            border-radius: 5px;
            cursor: pointer;
            margin: 5px;
        }
        button:hover {
            background: #166fe5;
        }
        .post {
            border-bottom: 1px solid #ddd;
            padding: 10px 0;
        }
        .notification {
            background: #e7f3ff;
            padding: 10px;
            margin: 5px 0;
            border-radius: 5px;
        }
        #output {
            background: #f8f9fa;
            padding: 15px;
            border-radius: 5px;
            margin-top: 20px;
            min-height: 100px;
        }
    </style>
</head>
<body>
    <div class="header">
        <h1>📱 Social Media Platform</h1>
        <p>C++ Backend + Web Frontend</p>
    </div>
    
    <div class="container">
        <div class="panel">
            <h2>📊 System Control</h2>
            <div>
                <button onclick="callAPI('system')">Get System Info</button>
                <button onclick="callAPI('posts')">Get Posts</button>
                <button onclick="callAPI('friends')">Get Friends</button>
                <button onclick="callAPI('notifications')">Get Notifications</button>
            </div>
            
            <h2>✍️ Create New</h2>
            <div>
                <input type="text" id="postContent" placeholder="What's on your mind?" style="width: 70%; padding: 8px;">
                <button onclick="createPost()">Create Post</button>
            </div>
            <div style="margin-top: 10px;">
                <input type="text" id="friendName" placeholder="Friend's name" style="width: 70%; padding: 8px;">
                <button onclick="addFriend()">Add Friend</button>
            </div>
            
            <div id="output">
                <h3>System Output</h3>
                <p>Click buttons to interact with C++ backend...</p>
            </div>
        </div>
        
        <div class="panel">
            <h2>👥 Quick Stats</h2>
            <div id="stats">
                <p>Loading stats...</p>
            </div>
            
            <h2>🔔 Recent Activity</h2>
            <div id="activity">
                <div class="notification">System connected successfully</div>
            </div>
        </div>
    </div>
    
    <script>
        async function callAPI(endpoint) {
            const output = document.getElementById('output');
            output.innerHTML = `<p>Loading ${endpoint}...</p>`;
            
            try {
                const response = await fetch(`/api/${endpoint}`);
                const data = await response.json();
                displayResult(data);
                updateStats(data);
            } catch (error) {
                output.innerHTML = `<p style="color: red;">Error: ${error}</p>`;
            }
        }
        
        async function createPost() {
            const content = document.getElementById('postContent').value;
            if (!content) {
                alert('Please enter post content');
                return;
            }
            
            const response = await fetch('/api/command', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({
                    action: 'create_post',
                    content: content
                })
            });
            
            const data = await response.json();
            displayResult(data);
            document.getElementById('postContent').value = '';
        }
        
        async function addFriend() {
            const name = document.getElementById('friendName').value;
            if (!name) {
                alert('Please enter friend name');
                return;
            }
            
            const response = await fetch('/api/command', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({
                    action: 'add_friend',
                    name: name
                })
            });
            
            const data = await response.json();
            displayResult(data);
            document.getElementById('friendName').value = '';
        }
        
        function displayResult(data) {
            const output = document.getElementById('output');
            output.innerHTML = `
                <h3>Response:</h3>
                <pre style="background: white; padding: 10px; border-radius: 5px; overflow: auto;">
${JSON.stringify(data, null, 2)}
                </pre>
            `;
        }
        
        function updateStats(data) {
            const statsDiv = document.getElementById('stats');
            if (data.posts_count !== undefined) {
                statsDiv.innerHTML = `
                    <p>📊 Posts: ${data.posts_count || data.posts?.length || 0}</p>
                    <p>👥 Friends: ${data.friends_count || data.friends?.length || 0}</p>
                    <p>🔔 Notifications: ${data.notifications_count || data.notifications?.length || 0}</p>
                `;
            }
            
            if (data.posts) {
                const activityDiv = document.getElementById('activity');
                let html = '';
                data.posts.slice(0, 3).forEach(post => {
                    html += `<div class="notification">📝 ${post.content}</div>`;
                });
                activityDiv.innerHTML = html;
            }
        }
        
        // Load system info on startup
        window.addEventListener('load', () => {
            callAPI('system');
        });
    </script>
</body>
</html>"""
    
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(html_content)
    print(f"   Created: {filepath}")

# ==================== MAIN ====================

if __name__ == "__main__":
    print("🚀 Starting Social Media Web Server")
    print("=" * 60)
    
    # Check paths
    check_paths()
    
    print(f"🌐 Server URL: http://localhost:{PORT}")
    print(f"📁 Serving from: {FRONTEND_PATH}")
    print("=" * 60)
    print("Press Ctrl+C to stop the server")
    print("=" * 60)
    
    # Start server
    try:
        with socketserver.TCPServer(("", PORT), SocialMediaHandler) as httpd:
            httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n\n👋 Server stopped by user")
    except Exception as e:
        print(f"\n❌ Server error: {e}")