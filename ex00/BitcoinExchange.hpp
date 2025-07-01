/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 09:35:23 by relamine          #+#    #+#             */
/*   Updated: 2025/07/01 21:09:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <limits>

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
        void parseDateTime(const char* datetimeString, const char* format, bool checkFormat = false);
        bool has_whitespace(const std::string& s);
        void validateHeaderLine(const std::string& filename, std::ifstream &file, char delimiter,
            const std::string& expectedField1, const std::string& expectedField2);
        std::map<std::string, double> exchangeRates;
};
