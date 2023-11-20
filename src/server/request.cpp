/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:16:40 by nbarakat          #+#    #+#             */
/*   Updated: 2023/11/20 05:32:33 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// void saveBodyToFile(string body, string filename)
// {
//     std::ofstream   outputfile(filename.c_str(), std::ios::binary);
//     if (outputfile.is_open())
//     {
//         outputfile << body;
//         outputfile.close();
//     }
//     else
//         throw Client::ClientReceiveFailed("Couldn't open request file ");
// }

// void getHeaders(char   *request, map<string, string> &headers)
// {
//     string line;
//     std::istringstream requestStream(request);
    
//     while (getline(requestStream, line) && line != "\r")
//     {
//         size_t colonPos = line.find(": ");
//         if (colonPos != string::npos)
//         {
//             string key = line.substr(0, colonPos);
//             string value = line.substr(colonPos + 2);
//             if (!value.empty() && value[value.length() - 1] == '\r')
//                 value.erase(value.length() - 1);
//             headers[key] = value;
//         }
//     }
//     std::ostringstream bodyStream;
//     string body;
    
//     bodyStream << requestStream.rdbuf();
//     body = bodyStream.str();
//     if (!body.empty())
//         saveBodyToFile(body, "requestBody.txt");
// }

// bool isValidMethod(string& method)
// {
//     return method == "GET" || method == "POST" || method == "DELETE";
// }

// bool isValidHttpVersion(string& version)
// {
//     return version == "HTTP/1.1";
// }

// bool isPathValid(string &path)
// {
//     return path.find("..") == string::npos;
// }

// bool doesPathExist(string &path, const string& base)
// {
//     struct stat buffer;
//     string fullPath = base + "/" + path;
//     return stat(fullPath.c_str(), &buffer) == 0;
// }

// bool isFileReadable(string& path, const string& base)
// {
//     string fullPath = base + "/" + path;
//     return access(fullPath.c_str(), R_OK) == 0;
// }

// void checkPath(string &path, string const &base)
// {
//     if (!isPathValid(path))
//         throw Client::ClientReceiveFailed("Path not valid (..)");
//     if (!doesPathExist(path, base))
//         throw Client::ClientReceiveFailed("Path doesn't exist");
//     if (!isFileReadable(path, base))
//         throw Client::ClientReceiveFailed("Can't read file (rights)");
// }

// void    parseReqLine(char *request, HttpRequest &request_obj)
// {
//     std::istringstream requestStream(request);
//     std::getline(requestStream, request_obj.method, ' ');
//     std::getline(requestStream, request_obj.path, ' ');
//     std::getline(requestStream, request_obj.httpVersion, '\r');
//     if (!isValidMethod(request_obj.method) || !isValidHttpVersion(request_obj.httpVersion))
//         throw Client::ClientReceiveFailed("http version / method error");
// }

// bool isHttpRequestComplete(const string& receivedData)
// {
//     return receivedData.find("\r\n\r\n") != string::npos;
// }

// void checkHost(string host, map<string, string> &headers)
// {
//     map<string, string>::iterator it = headers.find("Host");

//     if (it == headers.end() || it->second != host)
//         throw Client::ClientReceiveFailed("Wrong host / no host"); 
// }


// void parseRequest(char *request,  map<string, string> &headers)
// {
//     HttpRequest request_obj;
//     string host;
    
//     host = "127.0.0.0:8080";

//     getHeaders(request, headers);
//     parseReqLine(request, request_obj);
//     checkPath(request_obj.path, "/nfs/homes/nbarakat/Desktop/bamrouch/fake_dir");
//     checkHost(host, headers);

// }


// void treatHeader(bool &header, char *request, map<string, string> &headers)
// {
//      if (isHttpRequestComplete(request))
//     {
//         header = 1;
//         parseRequest(request, headers);
//     }
// }

// void addToBody(char *request)
// {
//     std::ostringstream bodyStream;
//     string body;
//     std::istringstream requestStream(request);
//     bodyStream << requestStream.rdbuf();
//     body = bodyStream.str();
//     if (!body.empty())
//         saveBodyToFile(body, "requestBody.txt");
// }

RequestException::RequestException(const string msg): message(msg)
{}

const char *RequestException::what() const throw()
{
    return message.c_str();
}

RequestException::~RequestException() throw()
{}

RequestCreationError::RequestCreationError(): RequestException("Couldn't open temp file")
{}

RequestWriteError::RequestWriteError(): RequestException("Counldn't write into the temp file")
{}

RequestHttpError::RequestHttpError(const string msg): RequestException(msg)
{}

Request::Request(unsigned long bd_size):headers_size(0), body_size(bd_size), body_read(0), headers_done(false)
{
    randomUUID();
}

string &Request::getBuffer()
{
    return  headers_content;
}

streamsize &Request::getHeaderSize()
{
    return headers_size;
}

streamsize &Request::getBodyRead()
{
    return body_read;
}

streamoff &Request::getBodySize()
{
    return body_size;
}

void Request::randomUUID()
{
    static const char* hexChars = "0123456789ABCDEF";
    const int uuidLength = 36;

    std::srand(std::time(0));
    stringstream ss;
    ss << "./tmp/";
    for (int i = 0; i < uuidLength; ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            ss << '-';
        } else {
            ss << hexChars[std::rand() % 16];
        }
    }
    ss << ".tmp";
    request_id = ss.str();
    size_t last_dot = request_id.find_last_of('.');
    stringstream num;
    for (size_t i = 0; access(request_id.c_str(), F_OK) == 0; i++)
    {
        request_id = ss.str();
        num << i;
        request_id.insert(last_dot, num.str());
        num.str("");
    }
    ofstream tmp_f(request_id.c_str());
    if (!tmp_f.is_open())
        throw RequestCreationError();
    tmp_f.close();
    cout << "request_id:" << request_id << endl;
}

string  &Request::getMethod()
{
    return method;
}

string  &Request::getPath()
{
    return path;
}


bool Request::HttpRequestComplete(ssize_t r)
{
    if (headers_done)
        return headers_done;
    size_t end_pos = headers_content.find("\r\n\r\n");
    if (end_pos != string::npos && end_pos + 4 >= MAX_HEADERS_SIZE)
        throw RequestHttpError("Max Headers limit exceeded");
    else if (end_pos != string::npos)
    {
        headers_done = true;
        body_read = r - (end_pos + 4);
        headers_size = 0;
        return true;
    }
    return false;
}

void Request::parseHttpHeaders()
{
    string line;
    std::istringstream requestStream(headers_content, ios::binary);
    
    while (getline(requestStream, line) && line != "\r")
    {
        size_t colonPos = line.find(": ");
        if (colonPos != string::npos)
        {
            string key = line.substr(0, colonPos);
            string value = line.substr(colonPos + 2, line.size() - 1);
            headers[key] = value;
        }
    }
    
    char buff[body_read];
    requestStream.read(buff, body_read);
    (*this) << buff;
}

void Request::operator<<(const char *buffer)
{
    ofstream tmp(request_id.c_str(), ios::app | ios::binary);
    
    if (!tmp.is_open())
        throw RequestWriteError();
    tmp.write(buffer , body_read);
    tmp.close();
}

Request::~Request()
{
    // remove(request_id.c_str());
}
