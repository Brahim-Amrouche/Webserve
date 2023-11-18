/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:56:18 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 00:29:08 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// Socket  *server_init(const string host,const string port)
// {
//     Socket *server_listen = NULL;
//     try 
//     {
//         if (host == "")
//             server_listen = new Socket(NULL, port.c_str());
//         else
//             server_listen = new Socket(host.c_str(), port.c_str());
//         cleanup.server_sock = server_listen;

//         return server_listen;
//     }
//     catch (const Socket::AddressLookUpFailed &e)
//     {
//         cerr << e.what() << endl;
//     }
//     catch (const Socket::SocketOpenFailed &e)
//     {
//         cerr << e.what() << endl;
//     }
//     catch (const Socket::SocketBindFailed &e)
//     {
//         cerr << e.what() << endl;
//     }
//     catch (const Socket::SocketListenFailed &e)
//     {
//         cerr << e.what() << endl;
//     }
//     return NULL;
// }