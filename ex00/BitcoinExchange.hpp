/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relamine <relamine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 09:35:23 by relamine          #+#    #+#             */
/*   Updated: 2025/06/28 23:39:49 by relamine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

class BitcoinExchange
{
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);
        ~BitcoinExchange();
        void displayExchangeRates(const std::string &filename);
    private:
        void loadDataCsv(const std::string &filename);
        void parseDateTime(const char* datetimeString, const char* format);
        bool has_whitespace(const std::string& s);
        void validateHeaderLine(const std::string& filename, std::ifstream &file, char delimiter,
            const std::string& expectedField1, const std::string& expectedField2);
        std::map<std::string, double> exchangeRates;
};
