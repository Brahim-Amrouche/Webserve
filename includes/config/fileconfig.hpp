/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileconfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 15:54:49 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/17 17:50:29 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stdexcept>

using std::ifstream;
using std::string;
using std::map;
using std::exception;
using std::ios;
using std::getline;
using std::vector;

class ConfigException : public exception
{
    private :
        string msg;
    public :
        ConfigException(const string new_msg);
        virtual const char *what() const throw();
        virtual ~ConfigException() throw();
};

class WrongConfigFile : public ConfigException
{
    public:
        WrongConfigFile();
};

class ConfigParsingError : public ConfigException
{
    public:
        ConfigParsingError(const string parsing_error);
};

class ConfigFile
{
    private:
        ifstream config_file;
        vector<string> tokens;
        int brakets;
    public:
        ConfigFile();
        ConfigFile(const string config_path);
        void          tokenizeLine(const string line);
        void          validateTokens();
        vector<string> &get_tokens();
        ~ConfigFile();
};
