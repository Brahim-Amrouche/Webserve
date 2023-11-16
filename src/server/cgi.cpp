/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:20:54 by maboulkh          #+#    #+#             */
/*   Updated: 2023/11/16 01:55:49 by elasce           ###   ########.fr       */
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

Cgi::Cgi(void) : requestMap(NULL), envp(NULL) {
    in = new TempFile(inFileName, stdin);
    out = new TempFile(outFileName, stdout);
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
    in->write(body);
    in->rewind();
    if (chdir(path.c_str()) != 0)
        ;//throw
    execve(scriptPath.c_str(), NULL, envp);
}

void Cgi::makeEnv() { 
    std::map<std::string, std::string> envMap;
    
    envMap["AUTH_TYPE"] = ""; //'auth-scheme' token in the request Authorization header field
    envMap["CONTENT_LENGTH"] = ""; //body.length
    envMap["CONTENT_TYPE"] = ""; // The server MUST set this meta-variable if an HTTP Content-Type field is present in the client request header.... otherwise it should omit this meta-variable
    envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
    envMap["PATH_INFO"] = ""; // "" + URI path
    envMap["PATH_TRANSLATED"] = ""; // htdocs + path_info
    // be werry of .. 
    envMap["QUERY_STRING"] = ""; // The server MUST set this variable; if the Script-URI does not include a query component, the QUERY_STRING MUST be defined as an empty string ("").
    envMap["REMOTE_ADDR"] = ""; //MUST be set to the network address of the client
    envMap["REMOTE_HOST"] = ""; // not necessary and can be substituted by REMOTE_ADDR
    envMap["REMOTE_IDENT"] = ""; //    envMap[""] = "";
    envMap[""] = ""; // REQUEST.method (get/post)
    envMap["SCRIPT_NAME"] = envMap["PATH_INFO"];
    envMap["SERVER_NAME"] = ""; // hostname | ipv4-address | ( "[" ipv6-address "]" ) !!!! in case of virtual servers use the request's Host header field
    envMap["SERVER_PORT"] = ""; // config.port
    envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
    envMap["SERVER_SOFTWARE"] = "webserv/1.0";

    // Meta-Variables >> take request headers and preceed them with HTTP_ and upercase key
    // remove Authorization and other already present value
    // for (std::map<std::string, std::string>::iterator i = request.begin())
        // envMap[request.first] = ((std::string) "HTTP_") + request.second.formated

    envp = new char*[envMap.size() + 1];
    int j = -1;
    std::string data;
    for (std::map<std::string, std::string>::iterator i = envMap.begin(); i != envMap.end(); i++) {
        data = i->first + "=" + i->second;
        envp[++j] = new char[data.size() + 1];
        std::memmove(envp[j], data.c_str(), data.size());
    }
    //very confusing check later
    scriptPath = envMap["PATH_TRANSLATED"];
    //very confusing check later
}

Cgi::~Cgi()
{
    if (envp) {
        for (int i = 0; envp[i]; i++)
            delete envp[i];
        delete[] envp;
    }
    delete in;
    delete out;
}