/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 17:21:01 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/04 20:39:39 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Socket::Socket(char *host, char *port)
{
    *this = Socket((const char *) host, (const char *)port);
}

Socket::Socket(const char *host, const char *port)
{
    ft_memset(&hints, 0, sizeof(ADDRESS_INFO));
    hints.ai_family = IPV4_AND_6;
    hints.ai_socktype = TCP_SOCK;
    hints.ai_flags = LISTEN_ON_HOST_NET; 
    
    bind_address = NULL;
    int dns_lookup_res = getaddrinfo(host, port, &hints, &bind_address);
    if (dns_lookup_res > 0)
        throw Socket::AddressLookUpFailed();
}

Socket &Socket::operator=(const Socket &eq_sock)
{
    sock_id = eq_sock.sock_id;
    hints = eq_sock.hints;
    bind_address = eq_sock.bind_address;
    return (*this);
}

