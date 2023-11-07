/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadbalancer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:48:34 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/07 18:57:32 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "socket.hpp"
#include "client.hpp"

typedef fd_set SOCKET_LIST;

class LoadBalancerExceptions : public exception
{
    private:
        string load_msg;
    public:
        LoadBalancerExceptions(const string msg);
        virtual const char *what() const throw();
        virtual ~LoadBalancerExceptions() throw();
};


class LoadBalancer
{
    private:
        Socket *listener;
        int     epoll_fd;
        Client  *clients;
    public:
        class EpollInitFailed : public LoadBalancerExceptions
        {
            public :
                EpollInitFailed(const string msg);
        };
        LoadBalancer(Socket *server);
        void loop();
        ~LoadBalancer();
};

