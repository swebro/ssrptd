CC = gcc

TARGET = ssrptd
SDIR = src
IDIR = include
ODIR = build
SOURCES = main.c config.c daemon.c
SRCFILES := $(patsubst %, $(SDIR)/%, $(SOURCES))
OBJFILES := $(patsubst %.c,$(ODIR)/%.o,$(SOURCES))

CFLAGS = -I${IDIR}

.PHONY: all clean

all: $(SRCFILES) $(TARGET)

clean:
	$(RM) $(OBJFILES) $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(LDFLAGS) $^ -o $@

$(ODIR)/%.o: $(SDIR)/%.c
	-@mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@

