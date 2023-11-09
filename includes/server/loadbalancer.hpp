/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadbalancer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:48:34 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/09 12:01:28 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "socket.hpp"
#include "client.hpp"

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
        Socket *listener;
        int     epoll_fd;
        EPOLL_EVENT *connections;
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
        LoadBalancer(Socket *server);
        void loop();
        void new_client();
        void receive();
        void send();
        EPOLL_EVENT *get_connections() const;
        ~LoadBalancer();
};

