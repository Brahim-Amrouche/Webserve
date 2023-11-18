/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:41:38 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 03:56:28 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

MergedServers::MergedServers(vector<ServerConfigs *> *s_config): configs(s_config), s(NULL)
{   }

void MergedServers::group_servers()
{
    size_t i = -1, j;
    while (++i < configs->size())
    {
        j = i;
        if ((*configs)[i] && (*configs)[i]->directives[LISTEN]->u_vec)
        {
            string &host = (*configs)[i]->directives[LISTEN]->vec->at(0);
            string &port = (*configs)[i]->directives[LISTEN]->vec->at(1);
            s = new ServerSocket(host.c_str(), port.c_str());
            s->sockBind();
            s->sockListen();
            s->configs.push_back((*configs)[i]);
            while (++j < configs->size())
            {
                if ((*configs)[j])
                {
                    string &other_host = (*configs)[j]->directives[LISTEN]->vec->at(0);
                    string &other_port = (*configs)[j]->directives[LISTEN]->vec->at(1);
                    if(other_host == host && other_port == port)
                    {
                        cout << "found equality" << endl;
                        s->configs.push_back((*configs)[j]);
                        (*configs)[j] = NULL;
                    }
                }
            }
            server_sockets.push_back(s);       
        }
    }
}

MergedServers::~MergedServers()
{
    size_t i = -1;
    while (++i < server_sockets.size())
        delete server_sockets[i];
}


MergedServers *configuration(int argc, char *argv[])
{
    string path("./config/default.conf");
    if (argc == 2)
        path = argv[1];
    else if (argc > 1)
    {
        cerr << "Wrong args number" << endl;
        return (NULL);
    }
    ConfigFile config_file(path);
    Server *servers_config = new Server(config_file.get_tokens());
    cleanup.servers_conf = servers_config;
    MergedServers *merged_server = new MergedServers(&servers_config->servers);
    cleanup.merged_serv = merged_server;
    merged_server->group_servers();
    return  merged_server;  
}