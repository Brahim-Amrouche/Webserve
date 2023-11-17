/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileconfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 19:46:43 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/15 18:46:20 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

ConfigException::ConfigException(const string new_msg):msg(new_msg)
{}

const char *ConfigException::what() const throw()
{
   return msg.c_str();
}

ConfigException::~ConfigException() throw()
{};

WrongConfigFile::WrongConfigFile(): ConfigException("Couldn't Open the Config file")
{};

ConfigParsingError::ConfigParsingError(const string parsing_error): ConfigException(parsing_error)
{};

ConfigFile::ConfigFile(const string config_path):config_file(config_path.c_str()) , brakets(0)
{
    if (!config_file.is_open())
        throw WrongConfigFile();
    string line;
    while (getline(config_file, line))
        tokenizeLine(line);
    try
    {
        // size_t i = -1;
        // while (++i < tokens.size())
        //     cout <<  "|" << tokens[i] << "|" << endl;
        validateTokens();
    }
    catch (const ConfigParsingError &e)
    {
        cerr << e.what() << endl;
    }
}

void ConfigFile::tokenizeLine(const string line)
{
    int line_start = next_non_space(line, 0), next_stop;
    size_t semic_pos, begin_semic;
    string token, semic_token;
    while (line_start != -1)
    {
        next_stop = next_space(line, line_start);
        token = line.substr(line_start, next_stop - line_start);
        begin_semic = 0;
        semic_pos = 0;
        while ((semic_pos = token.find_first_of(";{}", begin_semic)) || true)
        {
            if (semic_pos > begin_semic)
            {
                semic_token = token.substr(begin_semic ,semic_pos - begin_semic);
                if (semic_token.size())
                    tokens.push_back(semic_token);
            }
            if (semic_pos != string::npos)
                tokens.push_back(string(1,token[semic_pos]));
            else
                break;
            begin_semic = semic_pos + 1;
        }
        line_start = next_non_space(line, next_stop);
    }
    tokens.push_back("\n");
}

void    serverToken(vector<string> tokens, size_t &i, int &brakets)
{
    if (brakets > 0)
        throw ConfigParsingError("server directive can't be wrapped");
    if (i + 1 >= tokens.size())
        throw ConfigParsingError("server directive isolated");
    while (++i < tokens.size())
    {
        if (tokens[i] == "\n")
            continue;
        else if (tokens[i] != "{")
            throw ConfigParsingError("server directive must be followed by {");
        else
        {
            ++brakets;
            break;
        }
    }
}

void    locationToken(vector<string> tokens, size_t &i, int &brakets)
{
    if (brakets != 1)
        throw ConfigParsingError("location directive must be within a server");
    else if (i + 1 >= tokens.size() || tokens[++i][0] != '/')
        throw ConfigParsingError("location directive must be followed by a path");
    else if (i + 2 >= tokens.size())
        throw ConfigParsingError("Uncomplete location directive");
    while (++i < tokens.size())
    {
        if (tokens[i] == "\n")
            continue;
        else if (tokens[i] != "{")
            throw ConfigParsingError("location directive must be followed by {");
        else
        {
            ++brakets;
            break;
        }
    }
}

void    directivesToken(vector<string> tokens, size_t &i, int &brakets)
{
    if (i == 0 || brakets == 0)
        throw ConfigParsingError("a directive is out of scope");
    size_t j = i;
    while (++j < tokens.size())
    {
        if (tokens[j] == "\n" || tokens[j] == "{")
            throw ConfigParsingError("a directive should be inline");
        if (tokens[j] == ";")
            break;
    }
}

void    ConfigFile::validateTokens()
{
    size_t i = -1;
    while (++i < tokens.size())
    {
        if (tokens[i] == "\n")
            continue;
        else if (tokens[i] == "server")
            serverToken(tokens, i, brakets);
        else if (tokens[i] == "location")
            locationToken(tokens, i, brakets);
        else if (tokens[i] == "}")
        {
            if(brakets <= 0)
                throw ConfigParsingError("Closing Braket `}` can't be self closing");   
            else
                --brakets;
        }
        else if (tokens[i] == ";")
        {
            if (i == 0 || tokens[i - 1] == "\n" || tokens[i - 1] == "{" || tokens[i -1] == "}")
                throw ConfigParsingError("trailing `;`");
        }
        else
            directivesToken(tokens, i, brakets);
    }
    if (brakets)
        throw ConfigException("Uneven Closed brackets");
}

vector<string> &ConfigFile::get_tokens()
{
    return tokens;
}

ConfigFile::~ConfigFile()
{
    if (config_file.is_open())
        config_file.close();
}
