/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:20:54 by maboulkh          #+#    #+#             */
/*   Updated: 2023/11/15 13:30:53 by elasce           ###   ########.fr       */
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
    in = new TempFile(inFileName, stdin);
    out = new TempFile(outFileName, stdout);
    in->write(body);
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
            waitpid(pid, &status, WNOHANG);
    }
    in->resetFd();
    out->resetFd();
    out->read(body);
}

void Cgi::lunchScript(void) {
    // in->dup();
    // out->dup();
    if (chdir(path.c_str()) != 0)
        ;//throw
    execve(NULL, NULL, NULL);
}

// void Cgi::createFile(std::string& file, FILE* fileStream) {
//     std::string originalFileName = file;
//     std::string fileName = originalFileName;
//     size_t dotPos = fileName.find_last_of('.');
//     if (fileName.size() == 0)
//         dotPos = 0;
//     else if (dotPos != std::string::npos)
//         dotPos = fileName.size() - 1;
//     std::stringstream ss;
//     for (int i = 1; access(fileName.c_str(), F_OK) == 0; i++)
//     {
//         ss << i;
//         fileName = originalFileName;
//         fileName.insert(dotPos, ss.str());
//     }
//     file = fileName;
//     fileStream = std::freopen(fileName.c_str(), "w+", stdin);
//     std::remove(fileName.c_str());
// }

// void Cgi::prepareFiles() {
//     bodyLength = body.length();
//     in = new TempFile(inFileName, 0);
//     out = new TempFile(outFileName, 1);
//     in->write(body);
//     in->rewind();
// }

void Cgi::makeEnv() {
    env = NULL;
}

Cgi::Cgi(const char *host, const char *port)
{

}

Cgi::~Cgi()
{
    delete in;
    delete out;
}