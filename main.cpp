/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbarakat <nbarakat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:16:19 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 21:39:42 by nbarakat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

t_cleanupdata cleanup = {NULL,  NULL};

void    sigIntHandler(int signum)
{
    cout << "Server Shuting Down." << endl;
    if (cleanup.server_sock)
        delete cleanup.server_sock;
    if (cleanup.load_balancer)
        delete cleanup.load_balancer;
    // exit(signum);
    (void) signum;
}

int main()
{
    // ConfigFile x("./config/default.conf");
    // Server s(x.get_tokens());
    if (signal(SIGINT, sigIntHandler) == SIG_ERR)
    {
        cerr << "Couldn't add SignInt Handler" << endl;
        return (1);
    }
    Socket *server = server_init("", "8080");
    if (!server)
        return (1);
    server_listen(server);
}