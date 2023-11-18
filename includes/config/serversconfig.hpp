/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serversconfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 23:07:19 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 18:35:01 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "fileconfig.hpp"

class ServerSocket;

typedef enum e_gloabl_configs
{
    SERVER_NAME,
    LISTEN,
    BODY_SIZE,
    ERROR_PAGE,
    ALLOWED_METHODS,
    RETURN,
    ROOT,
    AUTOINDEX,
    INDEX,
    LOCATION,
    EXEC_CGI,
} t_global_configs;


typedef enum e_code_types
{
    INFO_CODES,
    SUCCES_CODES,
    REDIRECTION_CODES,
    CLIENT_ERROR_CODES,
    SERVER_ERROR_CODES
}   t_code_type;

class ServerConfigError : public ConfigException
{
    public:
        ServerConfigError(const string server_error);
};

typedef struct UServerConfig TServerConfig;

class Location
{
    private:
        map<int, TServerConfig *> *origin;
        map<int, TServerConfig *> *overload;
        const string  route;
    public:
        Location(const string new_route, map<int,
            TServerConfig *> *orig, map<int, TServerConfig *> *loc);
        map<int, TServerConfig *> *getLocationConf() const;
        ~Location();  
};

typedef struct UServerConfig
{
    bool u_vec;
    bool reoccures;
    int  params;
    union
    {
        vector<string> *vec;
        vector<Location *> *obj;
    };
    
    UServerConfig(const bool is_vec);
    ~UServerConfig();   
} TServerConfig;

class ServerConfigs
{
    public:
        map<int, TServerConfig *> directives;
        TServerConfig *add_dir_str(const string dir_str ,bool reoccures ,int params);
        TServerConfig *add_location(const string location_path, map<int, TServerConfig *> *origin);
        ServerConfigs(bool empty);
        ~ServerConfigs();
};

class Server : public ServerConfigs
{
    private:
        vector<string> &tokens;
    public:
        vector<ServerConfigs *> servers;
        vector<ServerSocket  *> server_sockets;
        Server(vector<string> &new_tokens);
        void parseTokens();
        void mergeServers();
        bool config_exists(string &host, string &port) const;
        void validate_allowed_methods(vector<string> &tokens, size_t &i, map<int, TServerConfig *> &directive);
        ~Server();
};
