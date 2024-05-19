#include "Response.hpp"


std::string Response::generateDirectoryListing(const std::string& path, const std::string& uri) {
    std::string body = "<!DOCTYPE html><html><head><style>"
    "ul { list-style-type: none; }"
    "li { margin-bottom: 10px; }"
    "img { vertical-align: middle; margin-left: 10px; }"
    "</style></head>"
    "<body><h1>Index of " + path + "</h1>"
    "<script>"
    "function deleteFile(path) {"
    "  if (confirm('Are you sure you want to delete this file?')) {"
    "    var xhr = new XMLHttpRequest();"
    "    xhr.open('DELETE', path, true);"
    "    xhr.onload = function () {"
    "      if (xhr.readyState == 4 && xhr.status == '204') {"
    "        location.reload();"
    "      } else {alert('Upload failed!')}"
    "    };"
    "    xhr.send();"
    "  }"
    "}"
    "</script><script>"
    "function submitForm(event) {"
    "  event.preventDefault();"
    "  var form = event.target;"
    "  var xhr = new XMLHttpRequest();"
    "  xhr.open(form.method, form.action, true);"
    "  xhr.onload = function () {"
    "    if (xhr.readyState == 4 && xhr.status == '201') {"
    "      alert('Upload successful!');"
    "      location.reload();"
    "    } else {"
    "      alert('Upload failed!');"
    "    }"
    "  };"
    "  xhr.onerror = function () {"
    "    alert('Upload failed!');"
    "  };"
    "  xhr.send(new FormData(form));"
    "}"
    "</script>"
    "<form action='" + uri + "' method='post' enctype='multipart/form-data' onsubmit='submitForm(event);'>"
    "<input type='file' name='file'>"
    "<input type='submit' value='Upload'>"
    "</form><br><ul><br>";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = std::string(ent->d_name);
            std::string escapedFilename = filename;
            body += "<li><a href='" + uri + "/" + std::string(ent->d_name) + "'>" + std::string(ent->d_name) + "</a>";
            body += " <a href='#' onclick='deleteFile(\"" + uri + "/" + escapedFilename + "\")'>";
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
    std::string body = "<!DOCTYPE html><html><body><h1>301 Moved Permanently</h1>"
        "<script>window.location.replace('" + path + "');</script>"
        "<h1>Moved Permanently</h1><p>The document has moved <a href='" + path + "'>here</a>.</p>"
        "</body></html>";
    _responseContentType = mimeType.get(".html");
    setStatus(301);
    return body; 
}
