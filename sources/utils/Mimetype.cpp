#include "MimeType.hpp"


/*
A media type (Multipurpose Internet Mail Extensions or MIME type),
indicates the nature and format of a document, file, or assortment of bytes.
MIME types are defined and standardized in IETF's RFC 6838
*/
MimeType::MimeType() {
    _mimeTypes[".css"]  = "text/css";
    _mimeTypes[".csv"]  = "text/csv";
    _mimeTypes[".html"] = "text/html";
    _mimeTypes[".ics"]  = "text/calendar";
    _mimeTypes[".js"]   = "text/javascript";
    _mimeTypes[".txt"]  = "text/plain";
    _mimeTypes[".bmp"]  = "image/bmp";
    _mimeTypes[".gif"]  = "image/gif";
    _mimeTypes[".ico"]  = "image/vnd.microsoft.icon";
    _mimeTypes[".jpg"]  = "image/jpeg";
    _mimeTypes[".svg"]  = "image/svg+xml";
    _mimeTypes[".aac"]  = "audio/aac";
    _mimeTypes[".mp3"]  = "audio/mpeg";
    _mimeTypes[".avi"]  = "video/x-msvideo";
    _mimeTypes[".mpeg"] = "video/mpeg";
    _mimeTypes[".bin"]  = "application/octet-stream";
    _mimeTypes[".doc"]  = "application/msword";
    _mimeTypes[".gz"]   = "application/gzip";
    _mimeTypes[".jar"]  = "application/java-archive";
    _mimeTypes[".json"] = "application/json";
    _mimeTypes[".azw"]  = "application/vnd.amazon.ebook";
    _mimeTypes[".mpkg"] = "application/vnd.apple.installer+xml";
    _mimeTypes[".odp"]  = "application/vnd.oasis.opendocument.presentation";
    _mimeTypes[".eot"]  = "application/vnd.ms-fontobject";
    _mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
}

MimeType::~MimeType() {}

std::string MimeType::get(std::string extension) {
    return _mimeTypes[extension];
}
