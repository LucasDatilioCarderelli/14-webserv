#include "Response.hpp"

// GET
std::string Response::openFile(const std::string& path) {
    std::ifstream file;
    std::string extension;
    std::string convertedPath = convertPercentTwenty(path);
    file.open(convertedPath.c_str());
    if (!file.good()) {
        logger.log("Error: could not open file", Logger::ERROR);
        std::string error_page = (_config->errorPage == "") ? "./www/statusCode/404.html" : _config->errorPage;
        file.open(error_page.c_str());
        extension = error_page.substr(error_page.find_last_of("."));
    } else {
        extension = convertedPath.substr(convertedPath.find_last_of("."));
    }
    _responseContentType = mimeType.get(extension);
    std::string body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 
    file.close();
    return body;
}

// POST
void Response::saveFileFromRequestBody(const std::string& requestBody) {
    std::istringstream requestStream(requestBody);

    std::string boundary;
    std::getline(requestStream, boundary);
    boundary = boundary.substr(0, boundary.size() - 1);

    std::string contentDisposition;
    std::getline(requestStream, contentDisposition);

    std::string fileName = contentDisposition.substr(contentDisposition.find("filename=") + 10);
    fileName = fileName.substr(0, fileName.size() - 2); // remove the trailing quote

    std::string contentType;
    std::getline(requestStream, contentType);

    std::string blankLine;
    std::getline(requestStream, blankLine);

    std::ofstream outputFile;
    std::string filePath = "./www/uploads/" + fileName;
    std::string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);
    if (fileExtension == "txt" || fileExtension == "html" || fileExtension == "css" || fileExtension == "js") {
        outputFile.open(filePath.c_str());
    } else {
        outputFile.open(filePath.c_str(), std::ios::binary);
    }

    std::string body;
    std::getline(requestStream, body);

    size_t body_pos = requestBody.find("\r\n\r\n", boundary.size() + 2) + 4; // 4 is the length of "\r\n\r\n"
    size_t boundary_end_pos = requestBody.find(boundary + "--", body_pos);

    std::string fileContent = requestBody.substr(body_pos, boundary_end_pos - body_pos);
    outputFile.write(fileContent.c_str(), fileContent.size());

    outputFile.close();
    setStatus(201);
}

// DELETE
void Response::deleteFile(const std::string& path) {
    std::string convertedPath = convertPercentTwenty(path);
    if (isDirectory(convertedPath)) {
        setStatus(403);
        return;
    } else if (remove(convertedPath.c_str()) != 0) {
        setStatus(404);
    } else {
        setStatus(204);
    }
}
