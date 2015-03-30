
BINARY := ./build/NanoSVGTranslator

OBJS := shared/TranslatorSettings.o \
		shared/TranslatorWindow.o \
		shared/BaseTranslator.o \
		shared/StreamBuffer.o \
		SVGTranslator.o \
		SVGLoader.o \
		ConfigView.o \
		SVGTranslatorApp.o

OBJDIR := build

RDEFS := ./res/NanoSVGTranslator.rdef
RSRCS := ./build/NanoSVGTranslator.rsrc

OBJS	:= $(addprefix $(OBJDIR)/,$(OBJS))

CC := g++
LD := $(CC)

LIBS := -lbe -ltranslation -lroot
CFLAGS := -w -g -I./src -I./src/shared -I./nanosvg
LDFLAGS := -g

.PHONY : clean build install

default : build

build : $(BINARY)
	
$(BINARY) : $(OBJDIR) $(OBJS) $(RSRCS)	
	$(LD) $(CFLAGS) $(OBJS) -o $(BINARY) $(LDFLAGS) $(LIBS)
	xres -o $(BINARY) $(RSRCS)
	mimeset -f $(BINARY)

clean:
	rm -rf $(OBJDIR)

build/%.o : src/%.cpp	
	@mkdir -p $(OBJDIR)/shared
	$(CC) $(CFLAGS) -c $< -o $@

build/%.rsrc : res/%.rdef
	rc -o $@ $<

install:
	mkdir -p /boot/home/config/add-ons/Translators
	cp $(BINARY) /boot/home/config/add-ons/Translators
