/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relamine <relamine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 09:35:42 by relamine          #+#    #+#             */
/*   Updated: 2025/07/21 09:19:11 by relamine         ###   ########.fr       */
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
    std::tm tmStruct = {};
    char *c = strptime(datetimeString, format, &tmStruct);
    if (c == NULL || *c != '\0') {
        throw std::invalid_argument("Invalid date format for date: " + std::string(datetimeString));
    }

    tmStruct.tm_isdst = -1;
    struct tm norm_data = tmStruct;
    if (mktime(&norm_data) == -1 ||
        norm_data.tm_year != tmStruct.tm_year ||
        norm_data.tm_mon != tmStruct.tm_mon ||
        norm_data.tm_mday != tmStruct.tm_mday) {
        throw std::invalid_argument("Invalid date for date: " + std::string(datetimeString));
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
        throw std::invalid_argument("No valid exchange rates found in file: " + filename);
    }
    
}


void BitcoinExchange::displayExchangeRates(const std::string &filename)
{
    std::ifstream       file(filename.c_str());
    std::stringstream   ss;
    std::string         line;

    validateHeaderLine(filename, file, '|', "date ", " value");
    for (; std::getline(file, line); )
    {
        if (line.empty())
            continue;
        ss.clear();
        ss.str("");
        ss << line;
        ss >> std::noskipws;
        char space;
        
        std::string date;
        double exchange_rate;

        if (std::getline(ss, date, '|') && ss >> space && space == ' ' && ss >> exchange_rate && ss.eof())
        {
            if (exchange_rate < 0)
            {
                std::cerr << "Error: not a positive number." << std::endl;
                continue;
            }
            if (exchange_rate > 1000.0 || exchange_rate < 0.0)
            {
                std::cerr << "Error: too large a number." << std::endl;
                continue;
            }
            try 
            {
                parseDateTime(date.c_str(), "%Y-%m-%d ", true);
                date.erase(date.length() - 1);
                std::map<std::string, double>::iterator it = exchangeRates.lower_bound(date);
                if (it->first == date)
                {
                    std::cout << date << " => " << exchange_rate << " = " 
                        << exchange_rate * it->second << std::endl;
                }
                else if (it == exchangeRates.begin())
                {
                    std::cerr << "Error: no exchange rate found for date: " << date << std::endl;
                }
                else
                {
                    --it;
                    std::cout << date << " => " << exchange_rate << " = " 
                        << exchange_rate * it->second << std::endl;
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Error: bad input => " << line << std::endl;
                continue;
            }
        }
        else
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
    }
}
    