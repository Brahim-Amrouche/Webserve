/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:16:19 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/17 01:55:42 by bamrouch         ###   ########.fr       */
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
    exit(signum);
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