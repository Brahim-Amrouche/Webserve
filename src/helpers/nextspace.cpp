/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nextspace.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 12:53:16 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/14 18:05:20 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int next_non_space(const string &line, int start)
{
    if (start == 0 && line.empty())
        return -1;
    int i = start;
    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '\t')
            return i;
        ++i;
    }
    return -1;
}

int next_space(const string &line, int start)
{
    int i = start;
    while(line[i])
    {
        if (line[i] == ' ' || line[i] == '\t')
            break;
        ++i;
    }
    return i;   
}
