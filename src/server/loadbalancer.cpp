/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadbalancer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:56:06 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/20 04:17:12 by bamrouch         ###   ########.fr       */
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

LoadBalancer::LoadBalancer(vector<ServerSocket *> *socks):listeners(socks), events_trigered(0) ,load(0), clients(NULL)
{
    cout << "Initiating The Load Balancer" << endl;
    epoll_fd = epoll_create(1);
    if (epoll_fd == -1)
        throw EpollInitFailed("Couldn't create an epoll instance", this);
    size_t i = -1;
    while (++i < listeners->size())
    {
        ft_memset(&(events[0]), 0, sizeof(EPOLL_EVENT));
        (*listeners)[i]->fill_epoll_event(&(events[0]), EPOLLIN);
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, (*listeners)[i]->get_sockid(), &(events[0])) == -1)
            throw EpollCtlFailed("Couldn't add the servers events listener", this);
        ft_memset(&(events[0]), 0, sizeof(EPOLL_EVENT));
        ++load;
    }
    ft_memset(events, 0, sizeof(EPOLL_EVENT) * MAX_EVENTS);
    cout << "Load Balancer Initiated." << endl;
}

void LoadBalancer::loop()
{
    while(true)
    {
        ft_memset(events, 0, sizeof(EPOLL_EVENT) * load);
        events_trigered = 0;
        events_trigered = epoll_wait(epoll_fd, events, load, -1);
        if (events_trigered == -1)
            throw EpollWaitFailed("Epoll wait failed", this);
        if (events_trigered > 0)
            handle_clients_request();
        else
            cout << "listenning..." << endl;
    }
}

int     find_sock_event(int event_fd, vector<ServerSocket *> *listeners)
{
    size_t i = -1;
    while (++i < listeners->size())
    {
        if ((*listeners)[i]->get_sockid() == event_fd)
        {
            cout << "server sock found" << endl;
            return i;
        }
    }
    return -1;
}

void    LoadBalancer::handle_clients_request()
{
    int     i = -1, server_index;
    Client *event_client;
    cout << "event received (" << events_trigered << ")" << endl;
    while (++i < events_trigered)
    {
        server_index = 0;
        event_client = NULL;
        cout << "event index is:" << i << endl;
        if ((server_index = find_sock_event(events[i].data.fd,listeners)) >= 0)
        {
            cout << "making new connection event :"  << i << endl;
            if (events[i].events & EPOLLIN)
                new_connection(i, server_index);
        }
        else if((event_client = Client::find_client_by_socketid(clients, events[i].data.fd)) != NULL)
        {
            cout << "making exchange event :" << i << endl;
            bool client_alive = true;
            if (events[i].events & EPOLLIN)
                client_alive = receive(event_client);
            (void) client_alive;
            // if (client_alive && (events[i].events & EPOLLOUT))
            //     send(event_client);
        }
    }
}

void LoadBalancer::new_connection(int event_id, int server_index)
{
    try
    {
        Socket *new_client_socket = (*listeners)[server_index]->sockAccept();
        Client *new_client = new Client(new_client_socket, (*listeners)[server_index], NULL);
        if (!new_client)
            throw CreatingClientFailed("Couldn't add a new client");
        ft_memset(&(events[event_id]), 0, sizeof(EPOLL_EVENT));
        new_client_socket->fill_epoll_event(&(events[event_id]), EPOLLIN | EPOLLOUT);
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_client_socket->get_sockid(), &(events[event_id])) == -1)
            throw EpollCtlFailed("Unable to Track a new Client", NULL);
        ft_memset(&(events[event_id]), 0, sizeof(EPOLL_EVENT));
        if (clients)
            clients->add_client(new_client);
        else
            clients = new_client;
        ++load;
        cout << "New client at (" << new_client_socket->get_sockid() << ")" << endl;
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
    catch (const EpollCtlFailed &e)
    {
        cerr << e.what() << endl;
    }
}

bool    LoadBalancer::receive(Client *sender)
{
    try 
    {
        sender->receive();
        return true;
    }
    catch (const Client::ClientReceiveFailed &e)
    {
        cerr << e.what() << endl;
        remove_connection(sender);
    }
    return false;
}

void    LoadBalancer::send(Client *receiver)
{
    try
    {
        receiver->send_response();
    }
    catch(const Client::ClientSendFailed& e)
    {
        std::cerr << e.what() << endl;
        remove_connection(receiver);
    }
    
}

void LoadBalancer::remove_connection(Client *connection)
{
    try
    {
        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, connection->get_socketid(), NULL) == -1)
            throw EpollCtlFailed("Couldn't remove a client event", NULL);
        Client::remove_client(&clients, connection);
        --load;
    }
    catch (const Client::ClientRemovalFailed &e)
    {
        cerr << e.what() << endl;
    }
}

LoadBalancer::~LoadBalancer()
{
    if (epoll_fd >= 0)
        close(epoll_fd);
    if (clients)
        Client::remove_all(clients);
    cleanup.load_balancer = NULL;
}
