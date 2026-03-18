CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Sources
SOURCES = $(SRCDIR)/est_ABR.c $(SRCDIR)/test_est_ABR.c
OBJECTS = $(OBJDIR)/est_ABR.o $(OBJDIR)/test_est_ABR.o
EXECUTABLE = $(BINDIR)/test_est_ABR

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) $(OBJECTS) -o $(EXECUTABLE)
	@echo "✓ Compilation réussie: $(EXECUTABLE)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -c $< -o $@

run: $(EXECUTABLE)
	@echo "Exécution des tests..."
	./$(EXECUTABLE)

clean:
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "✓ Nettoyage terminé"

.PHONY: all run clean
