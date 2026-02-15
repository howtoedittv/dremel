CC = gcc
CFLAGS = -Wall -O2
INSTALL_DIR = $(HOME)/.local/bin

all: cdremel dremel dremel-kill

dremel: dremel.c 
	$(CC) $(CFLAGS) -o dremel dremel.c 

cdremel: cdremel.c
	$(CC) $(CFLAGS) -o cdremel cdremel.c 

install: all
	mkdir -p $(INSTALL_DIR)
	cp cdremel $(INSTALL_DIR)/cdremel
	cp dremel $(INSTALL_DIR)/dremel
	cp dremel-kill $(INSTALL_DIR)/dremel-kill
	chmod +x $(INSTALL_DIR)/cdremel
	chmod +x $(INSTALL_DIR)/dremel
	chmod +x $(INSTALL_DIR)/dremel-kill
	@echo "Installed dremel, cdremel and dremel-kill to $(INSTALL_DIR)"

clean:
	rm -f cdremel dremel

uninstall:
	rm -rf $(INSTALL_DIR)/cdremel $(INSTALL_DIR)/dremel $(INSTALL_DIR)/dremel-kill

.PHONY: all install clean

