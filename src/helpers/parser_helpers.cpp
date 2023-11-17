/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 21:46:49 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/16 22:14:58 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
using std::stringstream;
using std::ostringstream;
using std::isdigit;

bool    string_is_digit(const string &str)
{
    for (string::const_iterator it = str.begin() ; it != str.end(); it++)
    {
        if (!isdigit(*it))
            return false;
    }
    return true;
}

void    valid_ip(const string &host_str, size_t start, size_t len)
{
    string numb = host_str.substr(start, len);
    if (!string_is_digit(numb))
        throw ServerConfigError("invalid ip address:1");
    stringstream ss(numb);
    int n = 0;
    if (!(ss >> n) || n < 0 || n > 255)
        throw ServerConfigError("invalid ip address: 2");
}

void    parse_host_string(const string str, string (&info)[2])
{
    string host, port;
    size_t colons_pos = str.find_first_of(":");
    if (colons_pos != string::npos)
    {
        size_t point1, point2, point3;
        string numb1, numb2, numb3, numb4;
        host = str.substr(0, colons_pos);
        if (colons_pos + 1 < str.size())
            port = str.substr(colons_pos + 1, string::npos);
        else
            throw ServerConfigError("invalid port format");
        point1 = host.find_first_of('.');
        if (point1 == string::npos)
            throw ServerConfigError("wrong host format");
        point2 = host.find_first_of('.', point1 + 1);
        if (point2 == string::npos)
            throw ServerConfigError("wrong host format");
        point3 = host.find_first_of('.', point2 + 1);
        if (point3 == string::npos)
            throw ServerConfigError("wrong host format");
        valid_ip(host,0, point1);
        valid_ip(host, point1 + 1, point2 - point1 - 1);
        valid_ip(host, point2 + 1, point3 - point2 - 1);
        valid_ip(host, point3 + 1, string::npos);
    }
    else
        port = str;
    stringstream ss(port);
    int port_num = 0;
    if (!string_is_digit(port) || !(ss >> port_num) || port_num < 0)
        throw ServerConfigError("invalid port number");
    if (!host.size())
        info[0] = "0.0.0.0";
    else
        info[0] = host;
    info[1] = port;
}

string    parse_size_string(const string str)
{
    size_t size_pos = str.find_first_of("MK");
    if (size_pos == string::npos || size_pos + 1 < str.size())
        throw ServerConfigError("invalid body size format");  
    string body_size = str.substr(0, size_pos);
    stringstream ss(body_size);
    long long bytes_size = 0;
    if (!(ss >> bytes_size) || bytes_size <= 0)
        throw ServerConfigError("body size is not valid");
    if (str[size_pos] == 'K')
        bytes_size *= 1024;
    else if (str[size_pos] == 'M')
        bytes_size *= 1024 * 1024;
    ostringstream oss;
    oss << bytes_size;
    return oss.str();
}

bool    parse_codes(const string s_code,t_code_type t_code)
{
    if (!string_is_digit(s_code))
        return false;
    stringstream ss(s_code);
    int code = 0;
    if (!(ss >> code) || code < 100)
        return false;
    if (t_code == INFO_CODES && code >= 100 && code <= 199)
        return true;
    else if (t_code == SUCCES_CODES && code >= 200 && code <= 299)
        return true;
    else if (t_code == REDIRECTION_CODES && code >= 300 && code <= 399)
        return true;
    else if (t_code == CLIENT_ERROR_CODES && code >= 400 && code <= 499)
        return true;
    else if (t_code == SERVER_ERROR_CODES && code >= 500 && code <= 599)
        return true;
    return false;
}

bool    string_is_dir(const string str)
{
    if(str[0] == '/')
        return true;
    return false;
}