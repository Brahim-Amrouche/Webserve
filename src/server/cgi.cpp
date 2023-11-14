/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:20:54 by maboulkh          #+#    #+#             */
/*   Updated: 2023/11/14 23:21:53 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

const char *Cgi::CgiOpenInFileFailed::what() const throw()
{
    return "Cgi open InFile Failed";
}

const char *Cgi::CgiOpenOutFileFailed::what() const throw()
{
    return "Cgi open OutFile Failed";
}

CgiExceptions::CgiExceptions(const string addr, const string msg, const Cgi *del)
{
    Cgi_msg = new string(msg);
    if (del)
        delete del;
}

const char *CgiExceptions::what() const throw()
{
    return Cgi_msg->c_str();
}

CgiExceptions::~CgiExceptions() throw()
{
    delete Cgi_msg;
}

Cgi::Cgi(void) : requestMap(NULL) {
    prepareFiles();
    int status = 0;
    int pid = fork();
    switch (pid)
    {
        case -1:
            // throw
            break;
        case 0:
            lunchScript();
            break;
        default:
            waitpid(pid, &status, 0);
    }
    std::fclose(in);
    
}

void Cgi::lunchScript(void) {
    // int fdIn = std::fileno(in);
    // int fdOut = std::fileno(out);
    // dup2(fdIn, 0);
    // dup2(fdOut, 1);

    char buff[100];
    char buff2[100];

    for (int i = 0; i< 99; i++)
        buff[i] = 'a';
    buff[99] = 0;

    for (int i = 0; i < 10000000; i++)
        write(0, buff, 100);

    if (chdir(path.c_str()) != 0)
        ;//throw
    execve(NULL, NULL, NULL);
}

void Cgi::prepareFiles() {
    std::string originalFileName = "inFile.txt";
    std::string fileName = originalFileName;
    size_t dotPos = fileName.find_last_of('.');
    if (fileName.size() == 0)
        dotPos = 0;
    else if (dotPos != std::string::npos)
        dotPos = fileName.size() - 1;
    std::stringstream ss;
    for (int i = 1; access(fileName.c_str(), F_OK) == 0; i++)
    {
        ss << i;
        fileName = originalFileName;
        fileName.insert(dotPos, ss.str());
    }
    FILE * inFile = std::freopen(fileName.c_str(), "w+", stdin);
    FILE * outFile = std::freopen("outFile", "w+", stdout);
    if (!inFile || !outFile)
        ;//throw
    std::remove(fileName.c_str());
    bodyLength = body.length();
    in = std::tmpfile();
    if (!in)
    {
        //throw
    }
    std::fwrite(body.c_str(), 1, bodyLength, in);
    std::fseek(in, 0, SEEK_SET);
    out = std::tmpfile();
    if (!out)
    {
        //throw
    }
}

void Cgi::makeEnv() {
    env = NULL;

}

Cgi::Cgi(const char *host, const char *port)
{

}

Cgi::~Cgi()
{
    std::fclose(out);
}