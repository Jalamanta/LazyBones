CC		= g++
INCLUDES	= -I/usr/include/ -I/home/bmay/programming/bmtplus/ -I/usr/include/mysql++/ -I/home/bmay/programming/LBDC -I/home/bmay/programming/LBMT/
CFLAGS		= -c -fPIC -g -Wall ${INCLUDES}
LDFLAGS		= -lm -lGL -lGLU -ljpeg -lgif -lpng -lftgl -lXxf86vm -L/home/bmay/programming/bmtplus/ -lC_bmt -lmysqlpp -L/home/bmay/programming/LBMT/ -lC_LBMT -L/home/bmay/programming/LBDC/ -lC_LBDC
SOURCES		= \
	GLX_base.cpp \
	GFX_animation.cpp \
	GFX_object.cpp \
	GFX_media.cpp \
	GFX_anchor.cpp \
	GFX_playlist.cpp \
	LazyBones.cpp

OBJECTS		= $(SOURCES:.cpp=.o)
TARGET	= LazyBones

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf *~ *.o $(TARGET) 

distclean:
	@rm -rf $(TARGET) $(OBJS) 

# dependencies
GLX_base.o: GLX_base.cpp GLX_base.h
GFX_object.o: GFX_object.cpp GFX_object.h
GFX_media.o: GFX_media.cpp GFX_media.h
GFX_animation.o: GFX_animation.cpp  GFX_animation.h
GFX_anchor.o: GFX_anchor.cpp GFX_anchor.h GFX_media.h GFX_object.h
GFX_playlist.o: GFX_playlist.cpp GFX_playlist.h GFX_anchor.h GFX_media.h GFX_object.h
LazyBones.o: LazyBones.cpp LazyBones.h GLX_base.h GFX_playlist.h GFX_anchor.h GFX_media.h GFX_object.h
