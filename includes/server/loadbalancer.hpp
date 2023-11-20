/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadbalancer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:48:34 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 22:08:52 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "socket.hpp"
#include "client.hpp"
#include <queue>

#define MAX_EVENTS 1024

using std::queue;

class LoadBalancer;

class LoadBalancerExceptions : public exception
{
    private:
        string load_msg;
    public:
        LoadBalancerExceptions(const string msg, const LoadBalancer *del);
        virtual const char *what() const throw();
        virtual ~LoadBalancerExceptions() throw();
};


class LoadBalancer
{
    private:
        vector<ServerSocket *> *listeners;
        int     epoll_fd;
        EPOLL_EVENT events[MAX_EVENTS];
        int  events_trigered;
        int  load;
        Client  *clients;
    public:
        class EpollInitFailed : public LoadBalancerExceptions
        {
            public :
                EpollInitFailed(const string msg, const LoadBalancer *del);
        };
        class EpollCtlFailed : public LoadBalancerExceptions
        {
           public:
                EpollCtlFailed(const string msg, const LoadBalancer *del);   
        };
        class EpollWaitFailed : public LoadBalancerExceptions
        {
            public:
                EpollWaitFailed(const string msg, const LoadBalancer *del);
        };
        class CreatingClientFailed : public LoadBalancerExceptions
        {
            public:
                CreatingClientFailed(const string msg);
        };
        class LoadBalancerOutOfMemory : public LoadBalancerExceptions
        {
            public:
                LoadBalancerOutOfMemory(const string msg);
        };
        LoadBalancer(vector<ServerSocket *> *socks);
        void loop();
        void new_connection(int event_id, int server_index);
        void handle_clients_request();
        bool receive(Client *sender);
        void send(Client *receiver);
        void remove_connection(Client *connection);
        ~LoadBalancer();
};

