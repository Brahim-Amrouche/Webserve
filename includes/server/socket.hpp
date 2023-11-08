/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:54:36 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/08 17:36:15 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdexcept>


typedef int SOCKET_ID;
typedef addrinfo ADDRESS_INFO;
typedef sockaddr SOCK_ADDR_STORAGE;
typedef socklen_t SOCK_ADDR_LEN;
typedef epoll_event EPOLL_EVENT;

#define IPV4_AND_6 AF_UNSPEC
#define TCP_SOCK SOCK_STREAM
#define LISTEN_ON_HOST_NET AI_PASSIVE
#define LISTEN_COUNT 1000

#define ISVALIDSOCKET(s) (s >= 0)

using std::string;
using std::exception;
using std::cerr;
using std::cout;
using std::endl;

class Socket;

class SockExceptions : public exception
{
    private:
        string *socket_msg;
    public:
        SockExceptions(const string addr, const string msg, const Socket *del);
        virtual const char *what() const throw();
        virtual ~SockExceptions() throw();
};

class Socket
{
    private:
        SOCKET_ID sock_id;
        SOCK_ADDR_STORAGE sock_addr;
        SOCK_ADDR_LEN   sock_addr_len;
    public:
        class AddressLookUpFailed : public exception
        {
            public:
                const char *what() const throw();
        };
        class SocketOpenFailed: public SockExceptions
        {
            public:
                SocketOpenFailed(const string addr, const Socket *del);
        };
        class SocketBindFailed: public SockExceptions
        {
            public:
                SocketBindFailed(const string addr, const Socket *del);
        };
        class SocketListenFailed: public SockExceptions
        {
            public:
                SocketListenFailed(const string addr,const Socket *del);
        };
        class SocketAcceptFailed: public SockExceptions
        {
            public:
                SocketAcceptFailed(const string addr, const Socket *del);  
        };
        Socket();
        Socket(const char *host, const char *port);
        void    sockBind() const;
        void    sockListen() const;
        Socket  *sockAccept() const;
        void    fill_epoll_event(EPOLL_EVENT *e_event, uint32_t mode) const;
        SOCKET_ID get_sockid() const;
        string  getSocketInfo() const;
        ~Socket();
};