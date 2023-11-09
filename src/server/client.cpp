/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:28:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/09 19:00:04 by bamrouch         ###   ########.fr       */
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

Client::ClientReceiveFailed::ClientReceiveFailed(const string addr): ClientExceptions(addr, "Receive request interrupted", NULL)
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
    while (p->next)
        p = this->next;
    p->next = new_client;
    cout << "Added client to the list(" << new_client->get_event_index() << ")" << endl;
}

void Client::remove_client(Client **root,  Client *del_client)
{
    while (*root)
    {
        if (*root == del_client)
        {
            cout << "Disconected client worker (" << del_client->get_event_index() << ")" << endl;
            *root = del_client->next;
            delete del_client;
            return;
        }
        root = &((*root)->next);
    }
    throw ClientRemovalFailed("", NULL);
}

void Client::reset_request()
{
    received = 0;
}

EPOLL_EVENT *Client::get_event(EPOLL_EVENT *events) const
{
    return &(events[event_index]);
}

int Client::get_event_index() const
{
    return event_index;
}

Client *Client::get_next() const
{
    return (next);
}

void Client::receive()
{
    cout << "Receiving request on worker (" << event_index << ")" << endl;
    int r = recv(socket->get_sockid(), request, MAX_REQUEST_SIZE - received, 0);
    if (r < 1)
        throw ClientReceiveFailed("0.0.0.0");
    else
    {
        received += r;
        request[received] = 0;
        cout << "Request received from worker (" << event_index << ")" << endl;
        cout << "request is:" << endl;
        cout << request << endl;
    }
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
