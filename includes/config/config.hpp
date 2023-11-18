/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:42:43 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 03:24:41 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "fileconfig.hpp"
#include "serversconfig.hpp"

class ServerSocket;


class MergedServers
{
    public:
        vector<ServerConfigs *> *configs;
        vector<ServerSocket *> server_sockets;
        ServerSocket *s;
        MergedServers(vector<ServerConfigs *> *s_config);
        void group_servers();
        ~MergedServers();  
};

MergedServers *configuration(int argc, char *argv[]);
