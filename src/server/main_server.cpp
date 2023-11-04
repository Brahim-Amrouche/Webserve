/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:56:18 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/04 19:21:34 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


void    server_listen(string &host, string &port)
{
    try {
        Socket server_listen(host.c_str(), port.c_str());
    }
    catch (const Socket::SocketOpenFailed &socket_open_failed)
    {
        
    }
}
