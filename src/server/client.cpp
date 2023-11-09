/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:28:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/09 13:12:37 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

ClientExceptions::ClientExceptions(const string addr, const string msg, const Client *del)
{
    client_msg = new string(msg);
    client_msg->append(addr);
    client_msg->push_back(')');
    if (del)
        delete del;
}

const char *ClientExceptions::what() const throw()
{
    return client_msg->c_str();
}

ClientExceptions::~ClientExceptions() throw()
{
    delete client_msg;
}

Client::ClientRemovalFailed::ClientRemovalFailed(const string addr, const Client *del):ClientExceptions(addr, "Couldn't remove client (", del)
{};

Client::Client():socket(NULL), received(0), next(NULL), event_index(-1)
{};

Client::Client(Socket *new_socket, int new_event_index, EPOLL_EVENT *events): socket(new_socket), received(0)
    , next(NULL), event_index(new_event_index)
{
    socket->fill_epoll_event(&(events[event_index]), EPOLLIN | EPOLLOUT);
};

void Client::add_client(Client *new_client)
{
    Client *p = this;
    const string client_info = new_client->socket->getSocketInfo();
    while (p->next)
        p = this->next;
    p->next = new_client;
    cout << "Added client to the list(" << client_info << ")" << endl;
}

void Client::remove_client(Client *del_client)
{
    Client **p_clients = &(this->next);
    const string client_info = del_client->socket->getSocketInfo();
    while (*p_clients)
    {
        if (*p_clients == del_client)
        {
            cout << "Disconected client (" << client_info << ")" << endl;
            *p_clients = del_client->next;
            delete del_client;
            return;
        }
        p_clients = &((*p_clients)->next);
    }
    throw ClientRemovalFailed(client_info, this);
}

void Client::reset_request()
{
    received = 0;
}

void    Client::remove_all(Client *root_client)
{
    Client *next_p;
    while (root_client)
    {
        next_p = root_client->next;
        delete root_client;
        root_client = next_p;
    }
    cout << "All clients deleted succesfully" << endl;
}

Client::~Client()
{
    if (socket)
        delete socket;
}
