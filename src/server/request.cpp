/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbarakat <nbarakat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:16:40 by nbarakat          #+#    #+#             */
/*   Updated: 2023/11/18 03:52:08 by nbarakat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"
#include <cstring>

std::istringstream get_headers(char   *request, std::map<std::string, std::string> &headers)
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
    return (requestStream);
    // put rest in a file 
}

bool is_valid_method(std::string& method)
{
    return method == "GET" || method == "POST" || method == "DELETE";
}

bool is_valid_http_version(std::string& version)
{
    return version == "HTTP/1.1";
}

bool is_path_valid(std::string &path)
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

void check_path(std::string &path, std::string const &base)
{
    if (!is_path_valid(path))
        throw Client::ClientReceiveFailed("Path not valid (..)");
    if (!doesPathExist(path, base))
        throw Client::ClientReceiveFailed("Path doesn't exist");
    if (!isFileReadable(path, base))
        throw Client::ClientReceiveFailed("Can't read file (rights)");
}

void    parse_req_line(char *request, HttpRequest &request_obj)
{
    std::istringstream requestStream(request);
    std::getline(requestStream, request_obj.method, ' ');
    std::getline(requestStream, request_obj.path, ' ');
    std::getline(requestStream, request_obj.httpVersion, '\r');
        cout << request_obj.method << endl;
        cout << request_obj.path << endl;
        cout << request_obj.httpVersion << endl;
    if (!is_valid_method(request_obj.method) || !is_valid_http_version(request_obj.httpVersion))
    {
        throw Client::ClientReceiveFailed("http version / method error");
    }
}

bool isHttpRequestComplete(const std::string& receivedData)
{
    return receivedData.find("\r\n\r\n") != std::string::npos;
}

void check_host(std::string host, std::map<std::string, std::string> &headers)
{
    std::map<std::string, std::string>::iterator it = headers.find("Host");

    if (it == headers.end() || it->second != host)
        throw Client::ClientReceiveFailed("Wrong host / no host"); 
}

int has_header(char *request)
{

}

void parse_request(char *request,  std::map<std::string, std::string> &headers)
{
    HttpRequest request_obj;
    std::string host;
    
    host = "127.0.0.0:8080";
    if (has_header(request))
    {
        get_headers(request, headers);
        parse_req_line(request, request_obj);
        check_path(request_obj.path, "/nfs/homes/nbarakat/Desktop/bamrouch/fake_dir");
        check_host(host, headers);
    }
    else 
    {

    }
}
