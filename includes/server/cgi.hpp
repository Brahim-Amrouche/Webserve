/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:20:56 by maboulkh          #+#    #+#             */
/*   Updated: 2023/11/16 01:58:54 by elasce           ###   ########.fr       */
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
#include <cstring>

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
    std::map<std::string, std::string>* requestMap;
    std::string body;
    size_t bodyLength;
    std::string *arg;
    std::string path;

    
    std::string inFileName;
    std::string outFileName;
    std::string scriptPath;
    char **envp;
    TempFile *in;
    TempFile *out;


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
        ~Cgi();
        
        void lunchScript();
        void makeEnv();
        void prepareFiles();
        void createFile(std::string& file);
};
