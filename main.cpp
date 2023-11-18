/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:16:19 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 19:04:32 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

t_cleanupdata cleanup = {NULL, NULL};

void    sigIntHandler(int signum)
{
    cout << "Server Shuting Down." << endl;
    if (cleanup.servers)
        delete cleanup.servers;
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
    Server *merged_servers = NULL;
    LoadBalancer *load = NULL;
    try {
        merged_servers = configuration(argc, argv);
        cleanup.servers = merged_servers;
        load = new LoadBalancer(&merged_servers->server_sockets);
        cleanup.load_balancer = load;
        load->loop();
    }
    catch (const WrongConfigFile &e)
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
    sigIntHandler(1);
}