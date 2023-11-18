/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbarakat <nbarakat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:16:28 by nbarakat          #+#    #+#             */
/*   Updated: 2023/11/18 03:49:30 by nbarakat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/stat.h>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string httpVersion;
};



std::istringstream get_headers(char   *request, std::map<std::string, std::string> &headers);
bool is_valid_method(std::string& method);
bool is_valid_http_version(std::string& version);
bool is_path_valid(std::string &path);
bool doesPathExist(std::string &path, const std::string& base);
bool isFileReadable(std::string& path, const std::string& base);
void check_path(std::string &path, std::string const &base);
void    parse_req_line(char *request, HttpRequest &request_obj);
void parse_request(char *request,  std::map<std::string, std::string> &headers);
bool isHttpRequestComplete(const std::string& receivedData);
