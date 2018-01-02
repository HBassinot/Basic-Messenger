CC = gcc
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS = -Wall -ansi -pedantic -g
else
	CFLAGS = -Wall -Werror -O3 -ansi -pedantic
endif
ARCHIVE_DIR = twitter
ARCHIVE = twitter.tar.gz
SRC = src
INC = inc
OBJ = obj
BIN = bin
INC_FLAG = -I./inc
EXEC = $(BIN)/server $(BIN)/client $(BIN)/killserver
SRCS = $(SRC)/server.c $(SRC)/argumentServer.c $(SRC)/socketClient.c \
       $(SRC)/listServer.c $(SRC)/client.c $(SRC)/socketServer.c \
       $(SRC)/argumentClient.c $(SRC)/applicationClient.c \
       $(SRC)/commandeServer.c $(SRC)/applicationServer.c  \
       $(SRC)/displayServer.c $(SRC)/logServer.c $(SRC)/listUserServer.c \
       $(SRC)/add.c $(SRC)/killServer.c

all: $(EXEC)

$(BIN)/killserver : $(OBJ)/killServer.o
	@$(CC) $^ -o $@

$(BIN)/server : $(OBJ)/server.o $(OBJ)/argumentServer.o $(OBJ)/socketServer.o  \
	 $(OBJ)/listServer.o $(OBJ)/applicationServer.o \
	 $(OBJ)/displayServer.o $(OBJ)/commandeServer.o  $(OBJ)/logServer.o \
         $(OBJ)/listUserServer.o $(OBJ)/add.o
	@$(CC) $^ -o $@

$(BIN)/client : $(OBJ)/client.o $(OBJ)/socketClient.o \
	 $(OBJ)/argumentClient.o  $(OBJ)/applicationClient.o
	@$(CC) $^ -o $@ 

# Règle de compilation générique des objets
$(OBJ)/%.o : $(SRC)/%.c
	@echo "module $@"
	@$(CC) $(CFLAGS) $(INC_FLAG) -c $< -o $@

depend:
	makedepend -- $(CFLAGS) $(INC_FLAG) -- $(SRCS)

# Nettoyage
clean :
	@rm -rf $(OBJ)/*.o *~ $(SRC)/*~ $(INC)/*~ makefile.bak

mrproper: clean
	@rm -f $(EXEC)

archive : clean
	@rm -rf $(ARCHIVE_DIR)
	@mkdir $(ARCHIVE_DIR)
	@mkdir $(ARCHIVE_DIR)/$(SRC)
	@mkdir $(ARCHIVE_DIR)/$(INC)
	@mkdir $(ARCHIVE_DIR)/$(OBJ)
	@mkdir $(ARCHIVE_DIR)/$(BIN)
	@ln makefile $(ARCHIVE_DIR)
	@ln $(SRC)/* $(ARCHIVE_DIR)/$(SRC)
	@ln $(INC)/* $(ARCHIVE_DIR)/$(INC)
	@ln $(OBJ)/* $(ARCHIVE_DIR)/$(OBJ)
	@ln $(BIN)/* $(ARCHIVE_DIR)/$(BIN)
	@tar -cvzf $(ARCHIVE) $(ARCHIVE_DIR)
	@rm -rf $(ARCHIVE_DIR)
