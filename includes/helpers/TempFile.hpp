/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TempFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:48:12 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/19 16:18:08 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include <string>
// #include <iostream>
// #include <fstream>
// #include <stdexcept>
// #include <sstream>
// #include <cstdio>

// using std::string;
// using std::FILE;
// using std::stringstream;
// using std::freopen;
// using std::exception;
// using std::fclose;
// using std::remove;
// using std::fread;
// using std::fwrite;

// class FileException : public exception
// {
//     public:
//         FileException();
//         const char *what() const throw();
//         ~FileException() throw();
// };

// class TempFile {
//     private:
//     int fd;
//     FILE* fs;
//     FILE* std;
//     string fileName;
    
//     public:
//     TempFile(string& file, FILE* std);
//     ~TempFile();
//     int write(string &str);
//     int read(string &str);
//     void rewind();
//     int resetFd();
// };
