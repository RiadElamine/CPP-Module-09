/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relamine <relamine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 09:35:42 by relamine          #+#    #+#             */
/*   Updated: 2025/06/29 00:25:53 by relamine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() { 
    try 
    {
        loadDataCsv("data.csv");
    } 
    catch (const std::invalid_argument &e)
    {
        std::cerr << "----------------------------Error loading data: ----------------------------\n" 
            << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : exchangeRates(other.exchangeRates) {}
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
    if (this != &other) {
        exchangeRates = other.exchangeRates;
    }
    return (*this);
}
BitcoinExchange::~BitcoinExchange() {}


void BitcoinExchange::parseDateTime(const char* datetimeString, const char* format)
{
    struct tm tmStruct;
    char *c = strptime(datetimeString, format, &tmStruct);
    if (c == NULL || *c != '\0') {
        throw std::invalid_argument("Invalid date format for date: " + std::string(datetimeString));
    }
}

bool BitcoinExchange::has_whitespace(const std::string& s) {
    return (s.find_first_of(" \t\n\r\f\v") != std::string::npos);
}


void BitcoinExchange::validateHeaderLine(const std::string& filename, std::ifstream &file, char delimiter,
        const std::string& Fieldate,const std::string& Fieldvalue)
    {

    if (!file.is_open()) {
        throw std::invalid_argument("Could not open file: " + filename);
    }

    std::string line;
    if (!std::getline(file, line)) {
        throw std::invalid_argument("File is empty: " + filename);
    }

    std::stringstream ss;
    // ss >> std::noskipws;
    std::string t;
    ss << line;
    ss >> t;
    std::cout << "Header line: " << t << std::endl;
     std::string date;
     std::string exchange_rate;;
    if (!std::getline(ss, date, delimiter) ||
    !(ss >> exchange_rate))
    {
        throw std::invalid_argument("Invalid header in file or could not read header: " + filename);
    }

    // std::string date;
    // std::string exchange_rate;
    // if (!std::getline(ss, date, delimiter) || date != Fieldate ||
    // !(ss >> exchange_rate) || exchange_rate != Fieldvalue || !ss.eof()) {
    //     throw std::invalid_argument("Invalid header in file or could not read header: " + filename);
    // }
}


void BitcoinExchange::loadDataCsv(const std::string &filename)
{
    std::ifstream       file(filename.c_str());
    std::stringstream   ss;
    std::string         line;

    validateHeaderLine(filename, file, ',', "date", "exchange_rate");

    for (; std::getline(file, line); )
    {
        ss.clear();
        ss.str("");
        ss << line;
        ss >> std::noskipws;
        
        std::string date;
        double exchange_rate;

        if (std::getline(ss, date, ',') && !has_whitespace(date) && ss >> exchange_rate && ss.eof())
        {
            if (exchange_rate < 0)
                throw std::invalid_argument("Negative exchange rate not allowed" + date);
            parseDateTime(date.c_str(), "%Y-%m-%d");
            exchangeRates[date] = exchange_rate;
        }
        else
        {
            throw std::invalid_argument("Invalid line format in file: " + filename + " / " + line);
        }
    }

    if (exchangeRates.empty()) {
        std::cerr << "No valid exchange rates found in file: " << filename << std::endl;
    }
    
}


void BitcoinExchange::displayExchangeRates(const std::string &filename)
{
    std::ifstream       file(filename.c_str());
    std::stringstream   ss;
    std::string         line;

    // try
    // {
    //     /* code */
    //     validateHeaderLine(filename, file, '|', "date ", " value");
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }
    

    // for (; std::getline(file, line); )
    // {
    //     ss.clear();
    //     ss.str("");
    //     ss << line;
    //     ss >> std::noskipws;
        
    //     std::string date;
    //     double exchange_rate;

    //     if (std::getline(ss, date, ',') && !has_whitespace(date) && ss >> exchange_rate && ss.eof())
    //     {
    //         if (exchange_rate < 0)
    //             throw std::invalid_argument("Negative exchange rate not allowed" + date);
    //         parseDateTime(date.c_str(), "%Y-%m-%d");
    //         exchangeRates[date] = exchange_rate;
    //     }
    //     else
    //     {
    //         throw std::invalid_argument("Invalid line format in file: " + filename + " / " + line);
    //     }
    // }
}
    