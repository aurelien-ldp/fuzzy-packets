CXX		=	clang++

SRCS	= 	Server/main.cpp		\
			Server/Server.cpp	\
			Server/Client.cpp	\
			Common/UDP.cpp		\
			Common/TCP.cpp

SRCC	=	Client/main.cpp		\
			Client/Client.cpp	\
			Common/UDP.cpp		\
			Common/TCP.cpp

OBJS	=	$(SRCS:.cpp=.o)

OBJC	=	$(SRCC:.cpp=.o)

CXXFLAGS=	-I. -W -Wall -Wextra -std=c++11 $(shell sdl2-config --cflags)

LDFLAGS	=	$(shell sdl2-config --libs) -lSDL2_net

all:		$(OBJS) $(OBJC)
	$(CXX) -o mserver $(OBJS) $(LDFLAGS)
	$(CXX) -o mclient $(OBJC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(OBJC)

fclean:		clean
	rm -f mserver mclient

re:			fclean all
