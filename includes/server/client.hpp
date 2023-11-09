/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:17:32 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/09 12:15:16 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./socket.hpp"
#define MAX_REQUEST_SIZE 2048

class Client;

class ClientExceptions : public exception
{
    private:
        string *client_msg;
    public:
        ClientExceptions(const string addr, const string msg, const Client *del);
        virtual const char *what() const throw();
        virtual ~ClientExceptions() throw();
};

class Client
{
    private :
        Socket *socket;
        char   request[MAX_REQUEST_SIZE];
        int    received;
        Client *next;
        int    event_index;
    public:
        class ClientRemovalFailed : public ClientExceptions
        {
            public :
                ClientRemovalFailed(const string addr, const Client *del);
        };
        Client();
        Client(Socket *new_socket, int new_event_index, EPOLL_EVENT *events);
        void add_client(Client *new_client);
        void remove_client(Client *del_client);
        void reset_request();
        EPOLL_EVENT *get_event();
        static void remove_all(Client *root_client);
        ~Client();
};
