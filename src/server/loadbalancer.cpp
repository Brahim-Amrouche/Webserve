/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadbalancer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:56:06 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/09 19:01:42 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

LoadBalancerExceptions::LoadBalancerExceptions(const string msg, const LoadBalancer *del):load_msg(msg)
{
    if (del)
        delete del;
};

const char *LoadBalancerExceptions::what() const throw()
{
    return load_msg.c_str();  
};

LoadBalancerExceptions::~LoadBalancerExceptions() throw()
{}

LoadBalancer::EpollInitFailed::EpollInitFailed(const string msg, const LoadBalancer *del):LoadBalancerExceptions(msg, del)
{};

LoadBalancer::EpollCtlFailed::EpollCtlFailed(const string  msg, const LoadBalancer *del):LoadBalancerExceptions(msg, del)
{};

LoadBalancer::EpollWaitFailed::EpollWaitFailed(const string msg, const LoadBalancer *del):LoadBalancerExceptions(msg, del)
{};

LoadBalancer::CreatingClientFailed::CreatingClientFailed(const string msg): LoadBalancerExceptions(msg, NULL)
{};

LoadBalancer::LoadBalancerOutOfMemory::LoadBalancerOutOfMemory(const string msg) : LoadBalancerExceptions(msg, NULL)
{};

LoadBalancer::LoadBalancer(Socket *server):listener(server), connections(NULL), load(0), clients(NULL)
{
    cout << "Initiating The Load Balancer" << endl;
    epoll_fd = epoll_create(1);
    if (epoll_fd == -1)
        throw EpollInitFailed("Couldn't create an epoll instance", this);
    connections = new EPOLL_EVENT[1];
    if (!connections)
        throw EpollInitFailed("Couldn't create an epoll event", this);
    listener->fill_epoll_event(&(connections[0]), EPOLLIN);
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener->get_sockid(), &(connections[0])) == -1)
        throw EpollCtlFailed("Couldn't add the servers events listener", this);
    ++load;
    cout << "Load Balancer Initiated." << endl;
}

void LoadBalancer::loop()
{
    int events_trigered;
    while(true)
    {
        events_trigered = 0;
        events_trigered = epoll_wait(epoll_fd, connections, load, 1000);
        if (events_trigered == -1)
            throw EpollWaitFailed("Epoll wait failed", this);
        if (events_trigered > 0)
        {
            new_connection();
            receive();
            // return;
        }
        else
            cout << "listenning..." << endl;
    }
}

int LoadBalancer::extend_capacity()
{
    cout << "extending server capacity" << endl;
    EPOLL_EVENT *new_connections = new EPOLL_EVENT[load + 1];
    if (!new_connections)
        throw LoadBalancerOutOfMemory("Couldn't allocate for new events");
    ft_memcpy(new_connections, connections, sizeof(EPOLL_EVENT) * load);
    delete[] connections;
    connections = new_connections;
    return load;
}

void LoadBalancer::new_connection()
{
    if (connections[0].events & EPOLLIN)
    {
        try
        {
            int event_index = -1;
            Socket *new_client_socket = listener->sockAccept();
            if (free_connections.empty())
            {
                event_index = extend_capacity();
                ++load;
            }
            else
            {
                cout << "from the free events" << endl;
                event_index = free_connections.front();
                free_connections.pop();
            }
            Client *new_client = new Client(new_client_socket, event_index, connections);
            if (!new_client)
                throw CreatingClientFailed("Couldn't add a new client");
            if (clients)
                clients->add_client(new_client);
            else
                clients = new_client;
            cout << "New client at (" << event_index << ")" << endl;
        }
        catch(const Socket::SocketAcceptFailed &e)
        {
            cerr << e.what() << endl;
        }
        catch(const LoadBalancerOutOfMemory &e)
        {
            cerr << e.what() << endl;
        }
        catch(const CreatingClientFailed &e)
        {
            cerr << e.what() << endl;
        }
    }
}

void    LoadBalancer::receive()
{
    Client *senders = clients;
    while (senders)
    {
        EPOLL_EVENT *client_event = senders->get_event(connections);
        if (client_event->events & EPOLLIN)
        {
            try 
            {
                senders->receive();
            }
            catch (const Client::ClientReceiveFailed &e)
            {
                cerr << e.what() << endl;
                senders = remove_connection(senders);
                continue ;
            }
        }
        senders = senders->get_next(); 
    }
}

Client *LoadBalancer::remove_connection(Client *connection)
{
    free_connections.push(connection->get_event_index());
    Client *next = connection->get_next();
    try
    {
        Client::remove_client(&clients, connection);
    }
    catch (const Client::ClientRemovalFailed &e)
    {
        cerr << e.what() << endl;
    }
    return next;
}

LoadBalancer::~LoadBalancer()
{
    if (listener)
        delete listener;
    if (epoll_fd >= 0)
        close(epoll_fd);
    if (connections)
        delete[] connections;
    if (clients)
        Client::remove_all(clients);
}
