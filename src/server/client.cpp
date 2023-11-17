/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbarakat <nbarakat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:28:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/17 04:00:41 by nbarakat         ###   ########.fr       */
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

Client::Client():socket(NULL), received(0), next(NULL)
{};

Client::Client(Socket *new_socket): socket(new_socket), received(0)
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


void Client::reset_request()
{
    received = 0;
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

void get_headers(char   *request, std::map<std::string, std::string> &headers)
{
    std::string line;
    std::istringstream requestStream(request);
    while (getline(requestStream, line) && line != "\r")
    {
        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            headers[key] = value;
        }
    }
}

bool is_valid_method(std::string& method)
{
    return method == "GET" || method == "POST" || method == "DELETE";
}

bool is_valid_http_version(std::string& version)
{
    return version == "HTTP/1.1";
}

void    parse_req_line(char *request)
{
    std::istringstream requestStream(request);
    HttpRequest request_obj;
    std::getline(requestStream, request_obj.method, ' ');
    std::getline(requestStream, request_obj.path, ' ');
    std::getline(requestStream, request_obj.httpVersion, '\r');
    cout << request_obj.path<< endl<< endl<< endl<< endl<< endl<< endl;
    if (!is_valid_method(request_obj.method) || !is_valid_http_version(request_obj.httpVersion))
        throw Client::ClientReceiveFailed("http version / method error");
}

void parse_request(char *request,  std::map<std::string, std::string> &headers)
{
   get_headers(request, headers);
   parse_req_line(request);
}
bool isHttpRequestComplete(const std::string& receivedData)
{
    return receivedData.find("\r\n\r\n") != std::string::npos;
}
void Client::receive()
{
    std::map<std::string, std::string> headers;
    
    cout << "Receiving request on socket (" << socket->get_sockid() << ")" << endl;
    int r = recv(socket->get_sockid(), request, MAX_REQUEST_SIZE - received, 0);
    if (r <= 0)
        throw ClientReceiveFailed("Wrong Receive");
    else
    {
        received += r;
        request[received] = 0;
    }
    if (r == MAX_REQUEST_SIZE)
        throw ClientReceiveFailed("Max Headers size");
    if (isHttpRequestComplete(request))
        parse_request(request, headers);

}

void Client::send_response()
{
    cout << "Sending response on socket (" << socket->get_sockid() << ")" << endl;
    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
    send(socket->get_sockid(), response, ft_strlen(response), 0);
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
}
