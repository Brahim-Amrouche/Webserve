/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:20:54 by maboulkh          #+#    #+#             */
/*   Updated: 2023/11/14 03:16:38 by elasce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

const char *Cgi::AddressLookUpFailed::what() const throw()
{
    return "Address Lookup Failed";
}

CgiExceptions::CgiExceptions(const string addr, const string msg, const Cgi *del)
{
    Cgi_msg = new string(msg);
    if (del)
        delete del;
}

const char *SockExceptions::what() const throw()
{
    return socket_msg->c_str();
}

SockExceptions::~SockExceptions() throw()
{
    delete socket_msg;
}

Cgi::Cgi(void) {
    body = "hello world";
    body += "\n here we are!"
    bodyLength = body.length();
    in = std::tmpfile();
    if (!in)
    {
        //throw
    }
    std::fwrite(body, 1, bodyLength, in);
    std::fseek(in, 0, SEEK_SET);
    out = std::tmpfile();
    if (!out)
    {
        //throw
    }

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

Cgi::lunchScript(void) {
    int fdIn = std::fileno(in);
    int fdOut = std::fileno(out);

    dup2(fdIn, 0);
    dup2(fdOut, 1);
    if (chdir(path.c_str()) != 0)
        ;//throw
    execve(arg[0], arg, env);
}

Cgi::makeEnv() {
    
}

Cgi::Cgi(const char *host, const char *port)
{

}

Cgi::~Cgi()
{

    std::fclose(out);
}