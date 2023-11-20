/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:17:32 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/19 15:51:48 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./socket.hpp"


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
        ServerSocket *server_sock;
        ServerConfigs *own_conf;
        Request *req;
        Client *next;
    public:
        class ClientRemovalFailed : public ClientExceptions
        {
            public :
                ClientRemovalFailed(const string addr, const Client *del);
        };
        class ClientReceiveFailed : public ClientExceptions
        {
            public :
                ClientReceiveFailed(const string addr);
        };
        class ClientSendFailed : public ClientExceptions
        {
            public :
                ClientSendFailed(const string addr);
        };
        Client();
        Client(Socket *new_socket, ServerSocket *new_serv, ServerConfigs *new_conf);
        void add_client(Client *new_client);
        Client *get_next() const;
        Socket *get_socket() const;
        SOCKET_ID get_socketid() const;
        void receive();
        void send_response();
        static void remove_client(Client **root, Client *del_client);
        static void remove_all(Client *root_client);
        static Client *find_client_by_socketid(Client *root, SOCKET_ID id);
        ~Client();
};
