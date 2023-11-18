/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serversconfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 01:30:05 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 19:04:08 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
using std::pair;
using std::make_pair;

ServerConfigError::ServerConfigError(const string server_error):ConfigException(server_error)
{}

Location::Location(const string new_route, map<int, TServerConfig *> *orig, map<int, TServerConfig *> *loc):origin(orig), overload(loc),route(new_route)
{};

map<int, TServerConfig *> *Location::getLocationConf() const
{
    return overload;
}

Location::~Location()
{
    for (map<int, TServerConfig *>::const_iterator it = overload->begin(); it != overload->end(); it++)
        delete it->second;
    delete overload;
}

UServerConfig::UServerConfig(const bool is_vec):u_vec(is_vec)
{
    if (u_vec)
        vec = new vector<string>;
    else
        obj = new vector<Location *>;
}

UServerConfig::~UServerConfig()
{
    if (u_vec)
    {   
        delete vec;
    }
    else
    {
        for (vector<Location *>::const_iterator it = obj->begin(); it != obj->end(); it++)
            delete *it;
        delete obj;
    }
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
    directives[EXEC_CGI] = add_dir_str("exec_cgi", false, 0);
    
}

TServerConfig *ServerConfigs::add_dir_str(const string dir_str ,bool reoccures ,int params)
{
    TServerConfig *conf= new TServerConfig(true);
    conf->vec->push_back(dir_str);
    conf->reoccures = reoccures;
    conf->params = params;
    return conf;
}

TServerConfig *ServerConfigs::add_location(const string location_path, map<int, TServerConfig *> *origin)
{
    TServerConfig *loc = new TServerConfig(false);
    loc->obj->push_back(new Location(location_path, origin, new map<int , TServerConfig *>));
    return loc;
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
        mergeServers();
    }
    catch (const ServerConfigError &e)
    {
        cerr << e.what() << endl;
    }
    catch (const Socket::AddressLookUpFailed &e)
    {
        cerr << e.what() << endl;
    }
    catch (const SockExceptions &e)
    {
        cerr << e.what() << endl;
    }
}

bool Server::config_exists(string &host, string &port) const
{
    size_t i = -1;
    while (++i < server_sockets.size())
    {
        string &cmp_host = server_sockets[i]->configs[0]->directives[LISTEN]->vec->at(0);
        string &cmp_port = server_sockets[i]->configs[0]->directives[LISTEN]->vec->at(1);
        if (cmp_host == host && cmp_port == port)
            return true;
    }
    return false;
}

void Server::mergeServers()
{
    size_t i = -1,j;
    while (++i < servers.size())
    {
        j = i;
        string &n_host = servers[i]->directives[LISTEN]->vec->at(0);
        string &n_port = servers[i]->directives[LISTEN]->vec->at(1);
        if(config_exists(n_host,n_port))
            continue;
        ServerSocket *n_srv_sock = new ServerSocket(n_host.c_str(), n_port.c_str());
        n_srv_sock->sockBind();
        n_srv_sock->sockListen();
        n_srv_sock->configs.push_back(servers[i]);
        while(++j < servers.size())
        {
            string &o_host= servers[j]->directives[LISTEN]->vec->at(0);
            string &o_port= servers[j]->directives[LISTEN]->vec->at(1);
            if (o_host == n_host && o_port == o_port)
                n_srv_sock->configs.push_back(servers[j]);
        }
        server_sockets.push_back(n_srv_sock);
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
    bool   in_location = false;
    ServerConfigs *current_server = NULL;
    map<int, TServerConfig *> *server_directives = NULL;
    while (++i < tokens.size())
    {
        if (tokens[i].find_first_of(";{\n", 0) != string::npos)
            continue;
        else if (tokens[i].find_first_of("}") != string::npos)
        {
            in_location = false;
            server_directives = &current_server->directives;
        }
        else if (tokens[i] == "server")
        {
            current_server = new ServerConfigs(true);
            server_directives = &current_server->directives;
            servers.push_back(current_server);
        }
        else if (!in_location && validate_directive(tokens, i, LISTEN,*server_directives, *(directives[LISTEN]), ";"))
        {
            string info[2];
            parse_host_string(tokens[++i], info);
            (*server_directives)[LISTEN] = add_dir_str(info[0], false, 2);
            (*server_directives)[LISTEN]->vec->push_back(info[1]);
        }
        else if (!in_location && validate_directive(tokens, i,SERVER_NAME ,*server_directives, *(directives[SERVER_NAME]), ";"))
            (*server_directives)[SERVER_NAME] = add_dir_str(tokens[++i], false, 1);
        else if (validate_directive(tokens, i, BODY_SIZE,*server_directives, *(directives[BODY_SIZE]), ";"))
            (*server_directives)[BODY_SIZE] = add_dir_str(parse_size_string(tokens[++i]), false, 1);
        else if (validate_directive(tokens, i, ERROR_PAGE,*server_directives, *(directives[ERROR_PAGE]), ";"))
        {
            if (!parse_codes(tokens[++i], CLIENT_ERROR_CODES) || !string_is_dir(tokens[++i]))
                throw ServerConfigError("Directive `error_page` wrong error code or path not given");
            (*server_directives)[ERROR_PAGE] = add_dir_str(tokens[i - 1], true, 2);
            (*server_directives)[ERROR_PAGE]->vec->push_back(tokens[i]);
        }
        else if (validate_directive(tokens, i, ALLOWED_METHODS,*server_directives, *(directives[ALLOWED_METHODS]),";"))
            validate_allowed_methods(tokens, i, *server_directives);
        else if (validate_directive(tokens, i, RETURN, *server_directives, *(directives[RETURN]), ";"))
        {
            if (!parse_codes(tokens[++i], REDIRECTION_CODES) || !string_is_dir(tokens[++i]))
                throw ServerConfigError("Directive `return` wrong redirect code or path not given");
            (*server_directives)[RETURN] = add_dir_str(tokens[i - 1], false, 2);
            (*server_directives)[RETURN]->vec->push_back(tokens[i]);
        }
        else if (validate_directive(tokens, i, ROOT, *server_directives, *(directives[ROOT]), ";"))
        {
            if (!string_is_dir(tokens[++i]))
                throw ServerConfigError("Directive `root` path not given");
            (*server_directives)[ROOT] = add_dir_str(tokens[i], false, 1);
        }
        else if (validate_directive(tokens, i, AUTOINDEX,  *server_directives, *(directives[AUTOINDEX]), ";"))
            (*server_directives)[AUTOINDEX] = add_dir_str(tokens[i], false, 1);
        else if (validate_directive(tokens, i, INDEX,*server_directives ,*(directives[INDEX]), ";"))
        {
            if (!string_is_dir(tokens[++i]))
                throw ServerConfigError("Directive `index` path not give");
            (*server_directives)[INDEX] = add_dir_str(tokens[i], false, 1);
        }
        else if (!in_location && validate_directive(tokens, i, LOCATION, *server_directives, *(directives[LOCATION]), "{"))
        {
            if (!string_is_dir(tokens[++i]))
                throw ServerConfigError("Directive `location` path not given");
            if (server_directives->find(LOCATION) == server_directives->end())
                (*server_directives)[LOCATION] = add_location(tokens[i], server_directives);
            else
                (*server_directives)[LOCATION]->obj->push_back(new Location(tokens[i], server_directives, new map<int, TServerConfig *>));
            in_location = true;
            server_directives = (*server_directives)[LOCATION]->obj->back()->getLocationConf();
        }
        else if (validate_directive(tokens, i, EXEC_CGI, *server_directives, *(directives[EXEC_CGI]), ";"))
            (*server_directives)[EXEC_CGI] = add_dir_str(tokens[i], false, 0);
        else
            throw ServerConfigError(string("Unknown token `") + tokens[i] + string("`"));
    }
}

Server::~Server()
{
    size_t i = -1;
    while (++i < servers.size())
        delete servers[i];
    i = -1;
    while (++i < server_sockets.size())
        delete server_sockets[i];
    cleanup.servers = NULL;
}