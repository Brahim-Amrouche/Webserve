/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:16:28 by nbarakat          #+#    #+#             */
/*   Updated: 2023/11/20 05:20:15 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/stat.h>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <ios>

using std::string;
using std::map;
using std::cout;
using std::endl;
using std::srand;
using std::stringstream;
using std::ofstream;
using std::streamoff;
using std::streamsize;
using std::streampos;

struct HttpRequest {
    string method;
    string path;
    string httpVersion;
};

// void getHeaders(char   *request, std::map<string, string> &headers);
// bool isValidMethod(string& method);
// bool isValidHttpVersion(string& version);
// bool isPathValid(string &path);
// bool doesPathExist(string &path, const string& base);
// bool isFileReadable(string& path, const string& base);
// void checkPath(string &path, string const &base);
// void parseReqLine(char *request, HttpRequest &request_obj);
// void parseRequest(char *request,  std::map<string, string> &headers);
// bool isHttpRequestComplete(const string& receivedData);
// void treatHeader(bool &header, char *request, std::map<string, string> &headers);
// void addToBody(char *request);


#define MAX_HEADERS_SIZE 4096

class RequestException : public exception
{
    private:
        const string message;
    public:
        RequestException(const string msg);
        const char *what() const throw();
        ~RequestException() throw();      
};

class RequestCreationError :public RequestException
{
    public:
        RequestCreationError();
};

class RequestWriteError :public RequestException
{
    public:
        RequestWriteError();
};

class RequestHttpError: public RequestException
{
    public:
        RequestHttpError(const string msg);  
};

class Request
{
    private :
        string headers_content;
        streamsize headers_size;
        streamoff body_size;
        streamsize body_read;
        std::streamsize x;
        string  request_id;
        string  method;
        string  path;
    public :
        map<string, string> headers;
        bool    headers_done;
        Request(unsigned long body_size);
        void randomUUID();
        string &getBuffer();
        streamsize &getHeaderSize();
        streamsize &getBodyRead();
        streamoff &getBodySize();
        string &getMethod();
        string &getPath();
        bool   HttpRequestComplete(ssize_t r);
        void   parseHttpHeaders();
        void operator<<(const char *buffer);
        ~Request();
};

