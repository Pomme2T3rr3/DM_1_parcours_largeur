CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Test executable (verification functions)
TEST_EXECUTABLE = $(BINDIR)/test_est_ABR
TEST_OBJECTS = $(OBJDIR)/est_ABR.o $(OBJDIR)/test_est_ABR.o

# Main executable (verification + generation)
MAIN_EXECUTABLE = $(BINDIR)/main
MAIN_OBJECTS = $(OBJDIR)/est_ABR.o $(OBJDIR)/genere_arbre_binaire.o $(OBJDIR)/main.o

# Default target
all: $(TEST_EXECUTABLE) $(MAIN_EXECUTABLE)

# Test executable
$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) $(TEST_OBJECTS) -o $(TEST_EXECUTABLE)
	@echo "✓ Compilation réussie: $(TEST_EXECUTABLE)"

# Main executable
$(MAIN_EXECUTABLE): $(MAIN_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) $(MAIN_OBJECTS) -o $(MAIN_EXECUTABLE)
	@echo "✓ Compilation réussie: $(MAIN_EXECUTABLE)"

# Generic rule for object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -c $< -o $@

# Convenient targets
run: $(MAIN_EXECUTABLE)
	@echo "Génération des mesures de complexité..."
	./$(MAIN_EXECUTABLE)

test: $(TEST_EXECUTABLE)
	@echo "Exécution des tests..."
	./$(TEST_EXECUTABLE)

clean:
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "✓ Nettoyage terminé"

.PHONY: all run test clean
