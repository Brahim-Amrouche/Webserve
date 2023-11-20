/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:42:29 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/19 01:48:12 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./request.hpp"
#include "./socket.hpp"
#include "./loadbalancer.hpp"
#include "./client.hpp"


Socket   *server_init(const string host,const string port);
void     server_listen(Socket *server);