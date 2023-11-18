/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 17:21:01 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 00:14:59 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

const char *Socket::AddressLookUpFailed::what() const throw()
{
    return "Address Lookup Failed";
}

SockExceptions::SockExceptions(const string addr, const string msg, const Socket *del)
{
    socket_msg = new string(msg);
    socket_msg->append(addr);
    socket_msg->push_back(')');
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

Socket::SocketOpenFailed::SocketOpenFailed(const string addr, const Socket *del):SockExceptions(addr, "Openning socket failed on (", del)
{}

Socket::SocketBindFailed::SocketBindFailed(const string addr, const Socket *del):SockExceptions(addr, "Binding socket failed on (", del)
{};

Socket::SocketListenFailed::SocketListenFailed(const string addr, const Socket *del):SockExceptions(addr, "Listening failed on (",del)
{};

Socket::SocketAcceptFailed::SocketAcceptFailed(const string addr, const Socket *del):SockExceptions(addr, "Accepting connection failed on (", del)
{};

Socket::Socket():sock_id(-1), sock_addr_len(0)
{
    ft_memset(&(sock_addr),0, sizeof(SOCK_ADDR_STORAGE));
};

Socket::Socket(const char *host, const char *port)
{
    ADDRESS_INFO hints;
    ft_memset(&hints, 0, sizeof(ADDRESS_INFO));
    hints.ai_family = IPV4_AND_6;
    hints.ai_socktype = TCP_SOCK;
    hints.ai_flags = LISTEN_ON_HOST_NET; 
    
    ADDRESS_INFO *bind_address = NULL;
    cout << "Looking for address info..." << endl;
    int dns_lookup_res = getaddrinfo(host, port, &hints, &bind_address);
    if (dns_lookup_res > 0)
        throw AddressLookUpFailed();
    ft_memcpy(&sock_addr, bind_address->ai_addr,sizeof(SOCK_ADDR_STORAGE));
    sock_addr_len = bind_address->ai_addrlen;
    cout << "Address info found (" << getSocketInfo() << ")." << endl;
    cout << "Openning new TCP socket..." << endl;
    sock_id = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(sock_id))
    {
        freeaddrinfo(bind_address);
        throw SocketOpenFailed(getSocketInfo(), this);
    }
    cout << "Socket successfully opened." << endl;
    freeaddrinfo(bind_address);
}

void Socket::sockBind() const
{
    const string sock_info = getSocketInfo(); 
    cout << "Binding Socket on (" << sock_info << ")..." << endl;
    if (bind(sock_id, &sock_addr, sock_addr_len))
        throw SocketBindFailed(sock_info, this);
    cout << "Binding successfully completed on (" << sock_info << ")." << endl;
}

void Socket::sockListen() const
{
    const string sock_info = getSocketInfo(); 
    cout << "Attempting to listen on (" << sock_info << ")..." << endl;
    if (listen(sock_id, SOMAXCONN))
        throw SocketListenFailed(sock_info, this);
    cout << "Socket listening on (" << sock_info << ")." << endl; 
}

Socket *Socket::sockAccept() const
{
    cout << "Accepting a new connection" << endl;
    const string server_info = getSocketInfo();
    Socket *client_socket = new Socket();
    if (!client_socket)
        throw Socket::SocketAcceptFailed(server_info, this);
    client_socket->sock_id = accept(sock_id, &(client_socket->sock_addr), &(client_socket->sock_addr_len));
    if (!ISVALIDSOCKET(client_socket->sock_id))
        throw Socket::SocketAcceptFailed(server_info, this);
    cout << "Client socket accepted at (" << client_socket->getSocketInfo() << ")." << endl;
    return client_socket;
}

void Socket::fill_epoll_event(EPOLL_EVENT *e_event, uint32_t mode) const
{
    e_event->events = mode;
    e_event->data.fd = sock_id;
}

SOCKET_ID Socket::get_sockid() const
{
    return sock_id;
}

string Socket::getSocketInfo() const
{
    char addr_info[100];
    char port_info[100];
    getnameinfo(&sock_addr, sock_addr_len, addr_info, sizeof(addr_info), port_info, sizeof(port_info), NI_NUMERICHOST | NI_NUMERICSERV);
    return string(addr_info) + string(":") + string(port_info);
}

Socket::~Socket()
{
    if (sock_id >= 0)
        close(sock_id);
}


ServerSocket::ServerSocket(const char *host,const char *port):Socket(host, port)
{}

ServerSocket::~ServerSocket()
{
    vector<ServerConfigs *>::const_iterator it = configs.begin();
    vector<ServerConfigs *>::const_iterator end = configs.end();
    while (it != end)
    {
        delete *it;
        it++;
    }
}