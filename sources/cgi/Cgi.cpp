#include "Cgi.hpp"

Cgi::Cgi() {}
Cgi::~Cgi() {
    for (size_t i = 0; i < _envArray.size(); ++i) {
        delete[] _envArray[i];
    }
}

Cgi::Cgi(std::string cgi_path, Request& request) : _cgi_path(cgi_path), _request(request) {}

std::map<std::string, std::string> Cgi::createEnv() {
    std::map<std::string, std::string> env;
    env["REQUEST_METHOD"]           = _request.getMethod();
    env["REQUEST_PATH"]             = _request.getPath();
    env["REQUEST_CONTENTTYPE"]      = _request.getContentType();
    env["REQUEST_CONTENTLENGTH"]    = _request.getContentLength();
    env["REQUEST_BODY"]             = _request.getBody();
    env["PATH_INFO"]                = _cgi_path;

    return env;
}

std::vector<char*> Cgi::createEnvArray() {
    std::map<std::string, std::string> env = createEnv();
    std::vector<char*> envArray;
    
    for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it) {
        std::string envVar = it->first + "=" + it->second;
        char* envStr = new char[envVar.size() + 1];
        std::strcpy(envStr, envVar.c_str());
        envArray.push_back(envStr);
    }
    
    envArray.push_back(NULL);
    
    return envArray;
}

std::string Cgi::executeCgi() {
    std::vector<char*> envArray = createEnvArray();
    std::string output;
    int pipe_stdin[2];
    int pipe_stdout[2];
    int pid;

    if (pipe(pipe_stdin) == -1 || pipe(pipe_stdout) == -1) {
        return "";
    }

    pid = fork();

    if (pid == -1) {
        return "";
    }
    else if (pid == 0) {
        // Child process
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        
        dup2(pipe_stdin[0], STDIN_FILENO);
        dup2(pipe_stdout[1], STDOUT_FILENO);

        char* argv[] = {NULL};

        alarm(5);
        if (execve(_cgi_path.c_str(), argv, envArray.data()) == -1) {
            return "";
        }
    }
    else {
        // Parent process6
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);

        ssize_t bytesWritten = write(pipe_stdin[1], _request.getBody().c_str(), _request.getBody().size());
        if (bytesWritten == -1) {
            return "";
        }
        close(pipe_stdin[1]);

        char buffer[1024];
        int bytesRead;
        while ((bytesRead = read(pipe_stdout[0], buffer, 1024)) > 0) {
            output.append(buffer, bytesRead);
        }
        close(pipe_stdout[0]);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            return "";
        }
    }

    return output;
}
