/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relamine <relamine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 09:35:42 by relamine          #+#    #+#             */
/*   Updated: 2025/06/30 23:52:16 by relamine         ###   ########.fr       */
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


void BitcoinExchange::parseDateTime(const char* datetimeString, const char* format, bool checkFormat)
{
    struct tm tmStruct;
    char *c = strptime(datetimeString, format, &tmStruct);
    if (c == NULL || *c != '\0') {
        throw std::invalid_argument("Invalid date format for date: " + std::string(datetimeString));
    }

    
    if (checkFormat) {
        char buffer[12];
        if (strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tmStruct) == 0) {
            throw std::invalid_argument("Failed to format date: " + std::string(datetimeString));
        }
        buffer[10] = ' ';
        buffer[11] = '\0';
        std::string t = buffer;
        if (t != std::string(datetimeString)) {
            throw std::invalid_argument("Date does not match format: " + std::string(datetimeString) + ",");
        }
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
    std::string t;
    ss << line;

    std::string date;
    std::string exchange_rate;
    if (!std::getline(ss, date, delimiter) || date != Fieldate ||
    !(getline(ss, exchange_rate)) || exchange_rate != Fieldvalue) {
        throw std::invalid_argument("Invalid header in file or could not read header: " + filename);
    }
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

    try
    {
        validateHeaderLine(filename, file, '|', "date ", " value");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    for (; std::getline(file, line); )
    {
        ss.clear();
        ss.str("");
        ss << line;
        ss >> std::noskipws;
        char space;
        
        std::string date;
        double exchange_rate;

        if (std::getline(ss, date, '|') && ss >> space && ss >> exchange_rate && ss.eof())
        {
            if (exchange_rate < 0)
            {
                std::cerr << "Error: not a positive number." << date << std::endl;
                continue;
            }
            if (exchange_rate > std::numeric_limits<int>::max())
            {
                std::cerr << "Error: too large a number." << std::endl;
                continue;
            }
            parseDateTime(date.c_str(), "%Y-%m-%d ", true);
            exchangeRates[date] = exchange_rate;
        }
        else
        {
            throw std::invalid_argument("Invalid line format in file: " + filename + " / " + line);
        }
    }
}
    