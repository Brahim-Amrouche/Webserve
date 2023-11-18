/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:41:38 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/18 18:57:26 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


Server *configuration(int argc, char *argv[])
{
    string path("./config/default.conf");
    if (argc == 2)
        path = argv[1];
    else if (argc > 1)
    {
        cerr << "Wrong args number" << endl;
        return (NULL);
    }
    ConfigFile config_file(path);
    Server *servers_config = NULL;
    servers_config = new Server(config_file.get_tokens());

    return  servers_config;
}