/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:20:54 by maboulkh          #+#    #+#             */
/*   Updated: 2023/11/13 23:22:07 by maboulkh         ###   ########.fr       */
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

Cgi::Cgi(void)
{
    in.open("inFile");
    in.rdbuf()->operator=;
    if (!in.is_open())
    {
        //
    }
    out.open("outFile");
    if (!out.is_open())
    {
        in.close();
        //
    }
    

}

Cgi::Cgi(const char *host, const char *port)
{
}

Cgi::~Cgi()
{
}