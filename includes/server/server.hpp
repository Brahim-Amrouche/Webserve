/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:42:29 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/06 16:25:34 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./socket.hpp"
#include "./loadbalancer.hpp"
#include "./client.hpp"

Socket   *server_listen(const string host,const string port);