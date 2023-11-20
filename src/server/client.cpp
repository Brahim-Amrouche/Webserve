/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:28:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/20 05:24:23 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

ClientExceptions::ClientExceptions(const string addr, const string msg, const Client *del)
{
    client_msg = new string(msg);
    client_msg->append(addr);
    client_msg->push_back(')');
    if (del)
        delete del;
}

const char *ClientExceptions::what() const throw()
{
    return client_msg->c_str();
}

ClientExceptions::~ClientExceptions() throw()
{
    delete client_msg;
}

Client::ClientRemovalFailed::ClientRemovalFailed(const string addr, const Client *del):ClientExceptions(addr, "Couldn't remove client (", del)
{};

Client::ClientReceiveFailed::ClientReceiveFailed(const string addr): ClientExceptions(addr, "Receive request interrupted (", NULL)
{};

Client::ClientSendFailed::ClientSendFailed(const string addr): ClientExceptions(addr, "Send Request failed (", NULL)
{}

Client::Client():socket(NULL), server_sock(NULL), own_conf(NULL), req(NULL), next(NULL)
{};

Client::Client(Socket *new_socket, ServerSocket *new_serv, ServerConfigs *new_conf): socket(new_socket), server_sock(new_serv), own_conf(new_conf), req(NULL)
    , next(NULL)
{
    cout << "Created Client with socket id (" << socket->get_sockid() << ")" << endl;
};

void Client::add_client(Client *new_client)
{
    Client *p = this;
    while (p->next)
        p = this->next;
    p->next = new_client;
    cout << "Added client to the list (" << socket->get_sockid() << ")" << endl;
}




Client *Client::get_next() const
{
    return (next);
}

Socket *Client::get_socket() const
{
    return socket;
}

SOCKET_ID Client::get_socketid() const
{
    return socket->get_sockid();
}

void Client::receive()
{
    char rcv_buffer[MAX_HEADERS_SIZE + 1] = {0};
    if (!req)
        req = new Request(0);
    streamsize &headerReceived  = req->getHeaderSize();
    string &headers_content = req->getBuffer();
    ssize_t r = recv(socket->get_sockid(), rcv_buffer, MAX_HEADERS_SIZE - headerReceived , 0);
    if (r <= 0)
        throw ClientReceiveFailed("0.0.0.0");
    else
    {
        rcv_buffer[r] = 0;
        if (!req->headers_done)
        {
            headerReceived += r;
            headers_content += rcv_buffer;
            try {
                if (req->HttpRequestComplete(r))
                    req->parseHttpHeaders();
            }
            catch (const RequestHttpError &e)
            {
                cerr << e.what() << endl;
                throw ClientReceiveFailed("Http error");
            }
        }
        else
        {  
            streamsize &body_read = req->getBodyRead();
            streamsize &body_size = req->getBodySize();
            body_read = r;
            body_size -= body_read;
            (*req) << rcv_buffer;
        }
    }
}

void Client::send_response()
{
    cout << "Sending response on socket (" << socket->get_sockid() << ")" << endl;
    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
    int s = send(socket->get_sockid(), response, ft_strlen(response), 0);
    // if (s < 0)
    //     throw ClientSendFailed("Couldn't send response");
    (void) s;
}

void Client::remove_client(Client **root,  Client *del_client)
{
    while (*root)
    {
        if (*root == del_client)
        {
            cout << "Disconected client (" << del_client->socket->get_sockid() << ")" << endl;
            *root = del_client->next;
            delete del_client;
            return;
        }
        root = &((*root)->next);
    }
    throw ClientRemovalFailed("", NULL);
}

void    Client::remove_all(Client *root_client)
{
    Client *next_p;
    while (root_client)
    {
        next_p = root_client->next;
        delete root_client;
        root_client = next_p;
    }
    cout << "All clients deleted succesfully" << endl;
}

Client *Client::find_client_by_socketid(Client *root, SOCKET_ID id)
{
    while (root)
    {
        if (root->get_socketid() == id)
            return root;
        root = root->get_next();
    }
    return NULL;
}

Client::~Client()
{
    if (socket)
        delete socket;
    if (req)
        delete req;
}
