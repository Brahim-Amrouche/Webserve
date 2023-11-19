/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbarakat <nbarakat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:16:28 by nbarakat          #+#    #+#             */
/*   Updated: 2023/11/18 23:38:19 by nbarakat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/stat.h>
#include <cstring>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string httpVersion;
    
};



void getHeaders(char   *request, std::map<std::string, std::string> &headers);
bool isValidMethod(std::string& method);
bool isValidHttpVersion(std::string& version);
bool isPathValid(std::string &path);
bool doesPathExist(std::string &path, const std::string& base);
bool isFileReadable(std::string& path, const std::string& base);
void checkPath(std::string &path, std::string const &base);
void parseReqLine(char *request, HttpRequest &request_obj);
void parseRequest(char *request,  std::map<std::string, std::string> &headers);
bool isHttpRequestComplete(const std::string& receivedData);
void treatHeader(bool &header, char *request, std::map<std::string, std::string> &headers);
void addToBody(char *request);
