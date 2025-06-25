import http.server
import socketserver
import mimetypes
import os

PORT = 8080
DIRECTORY = "build/web"

class CustomHandler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        # Serve .apk as application/zip for Pygbag compatibility
        if self.path.endswith(".apk"):
            self.send_header("Content-Type", "application/zip")
        super().end_headers()

    def translate_path(self, path):
        # Serve files from the build/web directory
        path = super().translate_path(path)
        relpath = os.path.relpath(path, os.getcwd())
        return os.path.join(DIRECTORY, os.path.relpath(path, os.getcwd()))

if __name__ == "__main__":
    os.chdir(DIRECTORY)
    with socketserver.TCPServer(("", PORT), CustomHandler) as httpd:
        print(f"Serving at http://localhost:{PORT}/index.html")
        httpd.serve_forever()
