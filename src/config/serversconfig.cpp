/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serversconfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 01:30:05 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/17 01:53:39 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
using std::pair;
using std::make_pair;


ServerConfigError::ServerConfigError(const string server_error):ConfigException(server_error)
{}

UServerConfig::UServerConfig(const bool is_vec):u_vec(is_vec)
{
    if (u_vec)
        vec = new vector<string>;
    else
        obj = new map<int, vector<string> >;
}

UServerConfig::~UServerConfig()
{
    if (u_vec)
        delete vec;
    else
        delete obj;
}

ServerConfigs::ServerConfigs(bool empty)
{
    if (empty)
        return;
    directives[SERVER_NAME] = add_dir_str("server_name", false, 1);
    directives[LISTEN] = add_dir_str("listen", false, 1);
    directives[BODY_SIZE] = add_dir_str("body_size", false,1);
    directives[ERROR_PAGE] = add_dir_str("error_page", true , 2);
    directives[ALLOWED_METHODS] = add_dir_str("allowed_methods", false , -1);
    directives[RETURN] = add_dir_str("return", false ,2);
    directives[ROOT] = add_dir_str("root", false ,1);
    directives[AUTOINDEX] = add_dir_str("auto_index", false, 0);
    directives[INDEX] = add_dir_str("index", false, 1);
    directives[LOCATION] = add_dir_str("location", true ,1);
}

TServerConfig *ServerConfigs::add_dir_str(const string dir_str ,bool reoccures ,int params)
{
    TServerConfig *conf= new TServerConfig(true);
    conf->vec->push_back(dir_str);
    conf->reoccures = reoccures;
    conf->params = params;
    return conf;
}

ServerConfigs::~ServerConfigs()
{
    for(map<int, TServerConfig *>::iterator it = directives.begin()
        ; it != directives.end(); ++it)
    {
        delete it->second;
    }
}

Server::Server(vector<string> &new_tokens):ServerConfigs(false), tokens(new_tokens)
{
    try 
    {
        parseTokens();
    }
    catch (const ServerConfigError &e)
    {
        cerr << e.what() << endl;
    }
    
}

bool    restrict_params(vector <string> &tokens, size_t &i,size_t params_count ,const string limiters)
{
    size_t count = i;
    while (++count < tokens.size() && count < i + params_count + 1)
    {
        if (tokens[count].find_first_of(limiters.c_str()) != string::npos)
            return false;   
    }
    if (count < tokens.size() && 
        tokens[count].find_first_of(limiters.c_str()) != string::npos)
        return true;
    return false;
}

bool    validate_directive(vector <string> &tokens, size_t i,
    t_global_configs dir,map<int, TServerConfig *> &server_dirs, TServerConfig &cmp_dir,const string limiters)
{
    if (tokens[i] == cmp_dir.vec->at(0))
    {
        if (!cmp_dir.reoccures && server_dirs.find(dir) != server_dirs.end())
            throw ServerConfigError(string("Duplicate `") + cmp_dir.vec->at(0) + string("` directive"));
        else if (cmp_dir.params >= 0 && !restrict_params(tokens, i, cmp_dir.params, limiters))
            throw ServerConfigError(string("Directive `") + cmp_dir.vec->at(0) + string("` not well formated"));
        return true;
    }
    return false;
}

void    Server::validate_allowed_methods(vector<string> &tokens, size_t &i, map<int, TServerConfig *> &dir)
{
    if(!restrict_params(tokens, i, 3, ";") && !restrict_params(tokens, i , 2, ";")
                && !restrict_params(tokens, i, 1, ";"))
        throw ServerConfigError("Directive `allowed_methods` not well formated");
    if (tokens[i + 1] == "GET" || tokens[i + 1] == "POST" || tokens [i + 1] == "DELETE")
        dir[ALLOWED_METHODS] = add_dir_str(tokens[++i], false, -1);
    size_t n = 0;
    while (tokens[++i] != ";")
    {
        if ((tokens[i] != "GET" && tokens[i] != "POST" && tokens [i] != "DELETE")
            || dir[ALLOWED_METHODS]->vec->at(n++) == tokens[i])
            throw ServerConfigError("Unallowed method in `allowed_methods` directive");
        else
            dir[ALLOWED_METHODS]->vec->push_back(tokens[i]);
    }
}


void Server::parseTokens()
{
    size_t i = -1;
    ServerConfigs *current_server = NULL;
    map<int, TServerConfig *> *server_directives = NULL;
    while (++i < tokens.size())
    {
        if (tokens[i].find_first_of(";{}\n", 0) != string::npos)
            continue;
        else if (tokens[i] == "server")
        {
            current_server = new ServerConfigs(true);
            server_directives = &current_server->directives;
            servers.push_back(current_server);
        }
        else if (validate_directive(tokens, i, LISTEN,*server_directives, *(directives[LISTEN]), ";"))
        {
            string info[2];
            parse_host_string(tokens[++i], info);
            (*server_directives)[LISTEN] = add_dir_str(info[0], false, 2);
            (*server_directives)[LISTEN]->vec->push_back(info[1]);
        }
        else if (validate_directive(tokens, i,SERVER_NAME ,*server_directives, *(directives[SERVER_NAME]), ";"))
            (*server_directives)[SERVER_NAME] = add_dir_str(tokens[++i], false, 1);
        else if (validate_directive(tokens, i, BODY_SIZE,*server_directives, *(directives[BODY_SIZE]), ";"))
            (*server_directives)[BODY_SIZE] = add_dir_str(parse_size_string(tokens[++i]), false, 1);
        else if (validate_directive(tokens, i, ERROR_PAGE,*server_directives, *(directives[ERROR_PAGE]), ";"))
        {
            if (!parse_codes(tokens[++i], CLIENT_ERROR_CODES) || !string_is_dir(tokens[++i]))
                throw ServerConfigError("Directive `error_page` wrong error code or path not given");
            (*server_directives)[ERROR_PAGE] = add_dir_str(tokens[i - 2], true, 2);
            (*server_directives)[ERROR_PAGE]->vec->push_back(tokens[i - 1]);
        }
        else if (validate_directive(tokens, i, ALLOWED_METHODS,*server_directives, *(directives[ALLOWED_METHODS]),";"))
            validate_allowed_methods(tokens, i, *server_directives);
        else if (validate_directive(tokens, i, RETURN, *server_directives, *(directives[RETURN]), ";"))
        {
            if (!parse_codes(tokens[++i], REDIRECTION_CODES) || !string_is_dir(tokens[++i]))
                throw ServerConfigError("Directive `return` wrong redirect code or path not given");
            (*server_directives)[RETURN] = add_dir_str(tokens[i - 2], false, 2);
            (*server_directives)[RETURN]->vec->push_back(tokens[i - 1]);
        }
    }
}

Server::~Server()
{
    size_t i = -1;
    while (++i < servers.size())
        delete servers[i];
}