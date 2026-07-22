NAME = HTTPRequest

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR = src
INC_DIR = include

SRCS = \
	main.cpp \
	$(SRC_DIR)/AMethod.cpp \
	$(SRC_DIR)/Response.cpp \
	$(SRC_DIR)/GET.cpp \
	$(SRC_DIR)/POST.cpp \
	$(SRC_DIR)/DeleteMethod.cpp \
	$(SRC_DIR)/MultipartUploadStrategy.cpp \
	$(SRC_DIR)/MethodFactory.cpp \
	$(SRC_DIR)/RequestAdapter.cpp \
	$(SRC_DIR)/ClientRequest.cpp \
	$(SRC_DIR)/BuffersStrategy.cpp \
	$(SRC_DIR)/helperAutoIndex.cpp

OBJS = $(SRCS:.cpp=.o)

INCLUDES = -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re