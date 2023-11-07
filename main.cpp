/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:16:19 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/06 16:04:40 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

using std::cout;
using std::endl;

int main()
{
    Socket *listener = server_listen(NULL, "8080");
    if (!listener)
        return (1);
    
}