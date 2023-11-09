/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadbalancer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:56:06 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/09 13:11:01 by bamrouch         ###   ########.fr       */
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
        if (events_trigered)
        {
            new_client();
            break;
        }
        else
            cout << "listenning..." << endl;
    }
}

void LoadBalancer::new_client()
{
    if (connections[0].events & EPOLLIN)
    {
        try
        {
            Socket *new_client_socket = listener->sockAccept();
            EPOLL_EVENT *new_connections = new EPOLL_EVENT[load + 1];
            if (!new_connections)
                throw LoadBalancerOutOfMemory("Couldn't allocate for new events");
            ft_memcpy(new_connections, connections, sizeof(EPOLL_EVENT) * load);
            delete[] connections;
            connections = new_connections;
            if (!clients)
            {
                clients = new Client(new_client_socket, load, new_connections);
                if (!clients)
                    throw CreatingClientFailed("Couldn't allocate for a new client");
                ++load;
            }
            else
            {
                Client *new_client = new Client(new_client_socket, load, new_connections);
                if (!new_client)
                    throw CreatingClientFailed("Couldn't add a new client");
                clients->add_client(new_client);
                ++load;
            }
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

EPOLL_EVENT *LoadBalancer::get_connections() const
{
    return connections;
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
