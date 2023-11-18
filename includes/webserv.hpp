/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbarakat <nbarakat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 19:03:03 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 00:17:38 by nbarakat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./config/config.hpp"
#include "./helpers/helpers.hpp"
#include "./server/server.hpp"
#include "./server/request.hpp"


typedef struct s_cleanupdata
{
    Socket *server_sock;
    LoadBalancer *load_balancer;
} t_cleanupdata;

extern t_cleanupdata cleanup;