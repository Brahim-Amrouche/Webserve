/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:56:18 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/12 15:42:08 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Socket  *server_init(const string host,const string port)
{
    Socket *server_listen = NULL;
    try 
    {
        if (host == "")
            server_listen = new Socket(NULL, port.c_str());
        else
            server_listen = new Socket(host.c_str(), port.c_str());
        cleanup.server_sock = server_listen;
        server_listen->sockBind();
        server_listen->sockListen();
        return server_listen;
    }
    catch (const Socket::AddressLookUpFailed &e)
    {
        cerr << e.what() << endl;
    }
    catch (const Socket::SocketOpenFailed &e)
    {
        cerr << e.what() << endl;
    }
    catch (const Socket::SocketBindFailed &e)
    {
        cerr << e.what() << endl;
    }
    catch (const Socket::SocketListenFailed &e)
    {
        cerr << e.what() << endl;
    }
    return NULL;
}

void    server_listen(Socket *server)
{
    try
    {
        LoadBalancer  *load = new LoadBalancer(server);
        cleanup.server_sock = NULL;
        cleanup.load_balancer = load;
        load->loop();
        delete load;
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
}