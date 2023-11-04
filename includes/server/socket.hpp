/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:54:36 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/04 20:39:01 by bamrouch         ###   ########.fr       */
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
#include <string>
#include <iostream>
#include <stdexcept>


typedef int SOCKET_ID;
typedef addrinfo ADDRESS_INFO;

#define IPV4_AND_6 AF_UNSPEC
#define TCP_SOCK SOCK_STREAM
#define LISTEN_ON_HOST_NET AI_PASSIVE


using std::string;
using std::exception;
using std::cerr;
using std::cout;

class Socket
{
    public:
        class SocketOpenFailed: public exception
        {
            const char *what() const throw();
        };
        class AddressLookUpFailed : public exception
        {
            const char *what() const throw();  
        };
        Socket(char *host, char *port);
        Socket(const char *host, const char *port);
        Socket &operator=(const Socket &eq_sock);
        ~Socket();
    private:
        SOCKET_ID sock_id;
        ADDRESS_INFO hints;
        ADDRESS_INFO *bind_address;
        
};