/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:20:56 by maboulkh          #+#    #+#             */
/*   Updated: 2023/11/14 22:36:30 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/types.h>
#include <wait.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstdio>
#include <map>

using std::exception;
using std::string;

class Cgi;

class CgiExceptions : public exception
{
    private:
        string *Cgi_msg;
    public:
        CgiExceptions(const string addr, const string msg, const Cgi *del);
        virtual const char *what() const throw();
        virtual ~CgiExceptions() throw();
};

class Cgi
{
    private:
    string PATH_INFO;
    string PATH_TRANSLATED;
    string QUERY_STRING;
    string REMOTE_ADDR;
    string REMOTE_HOST;
    string REQUEST_METHOD;
    string SCRIPT_FILENAME;
    string SCRIPT_NAME;
    string SERVER_NAME;
    string SERVER_SOFTWARE;
    string CONTENT_TYPE;
    string CONTENT_LENGTH;
    string HTTP_COOKIE;
    string HTTP_USER_AGENT;

    std::map<std::string, std::string>* requestMap;
    std::string body;
    size_t bodyLength;
    std::string *arg;
    std::string *env;
    std::string path;
    std::FILE* out;
    std::FILE* in;

    public:
        // class CgiOpenInFileFailed: public CgiExceptions
        // {
        //     public:
        //         CgiOpenFailed(const string addr, const Cgi *del);
        // };
        class CgiOpenInFileFailed: public CgiExceptions
        {
            public:
                const char *what() const throw();
        };
        class CgiOpenOutFileFailed : public exception
        {
            public:
                const char *what() const throw();
        };
        Cgi();
        Cgi(const char *host, const char *port);
        ~Cgi();
        
        void lunchScript();
        void makeEnv();
        void prepareFiles();
};
