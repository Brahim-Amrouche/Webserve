/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:56:18 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/05 19:05:51 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


void    server_listen(const string host,const string port)
{
    try {
        Socket server_listen(host.c_str(), port.c_str());
        server_listen.sockBind();
        server_listen.sockListen();
    }
    catch (const Socket::AddressLookUpFailed &e)
    {   
        cout << e.what() << endl;
    }
    catch (const Socket::SocketOpenFailed &e)
    {
        cout << e.what() << endl;
    }
    catch (const Socket::SocketBindFailed &e)
    {
        cout << e.what() << endl;
    }
    catch (const Socket::SocketListenFailed &e)
    {
        cout << e.what() << endl;
    }
}
