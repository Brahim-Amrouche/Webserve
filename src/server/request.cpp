/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbarakat <nbarakat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:16:40 by nbarakat          #+#    #+#             */
/*   Updated: 2023/11/18 23:36:09 by nbarakat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"

void saveBodyToFile(std::string body, std::string filename)
{
    std::ofstream   outputfile(filename.c_str(), std::ios::binary);
    if (outputfile.is_open())
    {
        outputfile << body;
        outputfile.close();
    }
    else
        throw Client::ClientReceiveFailed("Couldn't open request file ");
}

void getHeaders(char   *request, std::map<std::string, std::string> &headers)
{
    std::string line;
    std::istringstream requestStream(request);
    
    while (getline(requestStream, line) && line != "\r")
    {
        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            if (!value.empty() && value[value.length() - 1] == '\r')
                value.erase(value.length() - 1);
            headers[key] = value;
        }
    }
    std::ostringstream bodyStream;
    std::string body;
    
    bodyStream << requestStream.rdbuf();
    body = bodyStream.str();
    if (!body.empty())
        saveBodyToFile(body, "requestBody.txt");
}

bool isValidMethod(std::string& method)
{
    return method == "GET" || method == "POST" || method == "DELETE";
}

bool isValidHttpVersion(std::string& version)
{
    return version == "HTTP/1.1";
}

bool isPathValid(std::string &path)
{
    return path.find("..") == std::string::npos;
}

bool doesPathExist(std::string &path, const std::string& base)
{
    struct stat buffer;
    std::string fullPath = base + "/" + path;
    return stat(fullPath.c_str(), &buffer) == 0;
}

bool isFileReadable(std::string& path, const std::string& base)
{
    std::string fullPath = base + "/" + path;
    return access(fullPath.c_str(), R_OK) == 0;
}

void checkPath(std::string &path, std::string const &base)
{
    if (!isPathValid(path))
        throw Client::ClientReceiveFailed("Path not valid (..)");
    if (!doesPathExist(path, base))
        throw Client::ClientReceiveFailed("Path doesn't exist");
    if (!isFileReadable(path, base))
        throw Client::ClientReceiveFailed("Can't read file (rights)");
}

void    parseReqLine(char *request, HttpRequest &request_obj)
{
    std::istringstream requestStream(request);
    std::getline(requestStream, request_obj.method, ' ');
    std::getline(requestStream, request_obj.path, ' ');
    std::getline(requestStream, request_obj.httpVersion, '\r');
    if (!isValidMethod(request_obj.method) || !isValidHttpVersion(request_obj.httpVersion))
        throw Client::ClientReceiveFailed("http version / method error");
}

bool isHttpRequestComplete(const std::string& receivedData)
{
    return receivedData.find("\r\n\r\n") != std::string::npos;
}

void checkHost(std::string host, std::map<std::string, std::string> &headers)
{
    std::map<std::string, std::string>::iterator it = headers.find("Host");

    if (it == headers.end() || it->second != host)
        throw Client::ClientReceiveFailed("Wrong host / no host"); 
}


void parseRequest(char *request,  std::map<std::string, std::string> &headers)
{
    HttpRequest request_obj;
    std::string host;
    
    host = "127.0.0.0:8080";

    getHeaders(request, headers);
    parseReqLine(request, request_obj);
    checkPath(request_obj.path, "/nfs/homes/nbarakat/Desktop/bamrouch/fake_dir");
    checkHost(host, headers);

}


void treatHeader(bool &header, char *request, std::map<std::string, std::string> &headers)
{
     if (isHttpRequestComplete(request))
    {
        header = 1;
        parseRequest(request, headers);
    }
}

void addToBody(char *request)
{
    std::ostringstream bodyStream;
    std::string body;
    std::istringstream requestStream(request);
    bodyStream << requestStream.rdbuf();
    body = bodyStream.str();
    if (!body.empty())
        saveBodyToFile(body, "requestBody.txt");
}
