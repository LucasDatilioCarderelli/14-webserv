#include "Response.hpp"


std::string Response::generateDirectoryListing(const std::string& path) {
    std::string body = "<!DOCTYPE html>";
    body += "<html><head><style>";
    body += "ul { list-style-type: none; }";
    body += "li { margin-bottom: 10px; }";
    body += "img { vertical-align: middle; margin-left: 10px; }";
    body += "</style></head><body><h1>Index of " + path + "</h1>";
    body += "<script>";
    body += "function deleteFile(path) {";
    body += "  if (confirm('Are you sure you want to delete this file?')) {";
    body += "    var xhr = new XMLHttpRequest();";
    body += "    xhr.open('DELETE', path, true);";
    body += "    xhr.onload = function () {";
    body += "      if (xhr.readyState == 4 && xhr.status == '204') {";
    body += "        location.reload();";
    body += "      }";
    body += "    };";
    body += "    xhr.send();";
    body += "  }";
    body += "}";
    body += "</script><ul>";
    body += "<script>";
    body += "function submitForm(event) {";
    body += "  event.preventDefault();";
    body += "  var form = event.target;";
    body += "  var xhr = new XMLHttpRequest();";
    body += "  xhr.open(form.method, form.action, true);";
    body += "  xhr.onload = function () {";
    body += "    if (xhr.readyState == 4 && xhr.status == '201') {";
    body += "      alert('Upload successful!');";
    body += "      location.reload();";
    body += "    } else {";
    body += "      alert('Upload failed!');";
    body += "    }";
    body += "  };";
    body += "  xhr.onerror = function () {";
    body += "    alert('Upload failed!');";
    body += "  };";
    body += "  xhr.send(new FormData(form));";
    body += "}";
    body += "</script>";

    body += "<form action='/uploads' method='post' enctype='multipart/form-data' onsubmit='submitForm(event);'>";
    body += "<input type='file' name='file'>";
    body += "<input type='submit' value='Upload'>";
    body += "</form>";
    body += "<br><br>";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = std::string(ent->d_name);
            std::string escapedFilename = filename;
            body += "<li><a href='/uploads/" + std::string(ent->d_name) + "'>" + std::string(ent->d_name) + "</a>";
            body += " <a href='#' onclick='deleteFile(\"/uploads/" + escapedFilename + "\")'>";
            body += " <img src='/static/assets/red_trash.png' alt='delete' width='20' height='20'></a></li>";
        }
        closedir(dir);
    } else {
        body = openFile("./www/statusCode/404.html");
    }
    body += "</ul></body></html>";
    return body;
}

std::string Response::makeRedirection(const std::string& path) {
    std::string body = "<!DOCTYPE html>";
    body += "<html><body><h1>301 Moved Permanently</h1>";
    body += "<script>window.location.replace('" + path + "');</script>";
    body += "</head><body><h1>Moved Permanently</h1><p>The document has moved <a href=\"";
    body += path + "\">here</a>.</p></body></html>";
    _responseContentType = mimeType.get(".html");
    setStatus(301);
    return body; 
}
