/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadbalancer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:56:06 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/07 18:58:31 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

LoadBalancerExceptions::LoadBalancerExceptions(const string msg):load_msg(msg)
{};

const char *LoadBalancerExceptions::what() const throw()
{
    return load_msg.c_str();  
};

LoadBalancerExceptions::~LoadBalancerExceptions() throw()
{}

LoadBalancer::LoadBalancer(Socket *server):listener(server), clients(NULL)
{
    epoll_fd = epoll_create(0);
    if (epoll_fd < 0)
        throw EpollInitFailed("Couldn't create an epoll instance");
}

void LoadBalancer::loop()
{
    
}

LoadBalancer::~LoadBalancer()
{
    if (listener)
        delete listener;
}
