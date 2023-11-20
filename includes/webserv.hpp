/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 19:03:03 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/20 04:16:38 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./config/config.hpp"
#include "./helpers/helpers.hpp"
#include "./server/server.hpp"



typedef struct s_cleanupdata
{
    Server *servers;
    LoadBalancer *load_balancer;
} t_cleanupdata;

extern t_cleanupdata cleanup;
void    sigIntHandler(int signum);