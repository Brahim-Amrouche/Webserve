/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:16:19 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 03:51:32 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

t_cleanupdata cleanup = {NULL, NULL,  NULL, NULL};

void    sigIntHandler(int signum)
{
    cout << "Server Shuting Down." << endl;
    if (cleanup.servers_conf)
        delete cleanup.servers_conf;
    if (cleanup.merged_serv)
        delete cleanup.merged_serv;
    if (cleanup.server_sock)
        delete cleanup.server_sock;
    if (cleanup.load_balancer)
        delete cleanup.load_balancer;
    exit(signum);
}

int main(int argc, char *argv[])
{

    if (signal(SIGINT, sigIntHandler) == SIG_ERR)
    {
        cerr << "Couldn't add SignInt Handler" << endl;
        return (1);
    }
    try {
        MergedServers *merged_servers = configuration(argc, argv);
        LoadBalancer *load = new LoadBalancer(&merged_servers->server_sockets);
        load->loop();
    }
    catch (const Socket::AddressLookUpFailed &e)
    {
        cerr << e.what() << endl;
    }
    catch (const SockExceptions &e)
    {
        cerr << e.what() << endl;
    }
    catch (const LoadBalancer::EpollInitFailed &e)
    {
        cerr << e.what() << endl;
    }
    catch (const LoadBalancer::EpollCtlFailed &e)
    {
        cerr << e.what() << endl;
    }
    catch (const LoadBalancer::EpollWaitFailed &e)
    {
        cerr << e.what() << endl;
    }
    return (1);
}