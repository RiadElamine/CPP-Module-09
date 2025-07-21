NAME = btc

SOURCE = main.cpp BitcoinExchange.cpp

OBJECT = $(SOURCE:.cpp=.o)

HEARDER = BitcoinExchange.hpp

CFLAGS = 

all : $(NAME)

$(NAME) : $(OBJECT)
	c++ $(CFLAGS) $(OBJECT) -o $(NAME)

%.o: %.cpp $(HEARDER)
	c++ $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECT)

fclean: clean
	rm -f $(NAME)

re: fclean all