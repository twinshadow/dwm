# dwm version
VERSION = 6.1

# Customize below to fit your system

# paths
PREFIX = ${HOME}
MANPREFIX =

X11INC = /usr/include/X11
X11LIB = /usr/lib

# Xinerama, comment if you don't want it
XINERAMALIBS = -lXinerama
XINERAMAFLAGS = -DXINERAMA

# includes and libs
INCS = -I${X11INC}
LIBS = -L${X11LIB} -lX11 ${XINERAMALIBS}

# System flags
CFLAGS = -O2 -fomit-frame-pointer -pipe
CPPFLAGS = -O2 -fomit-frame-pointer -pipe
LDFLAGS = -Wl,-O1 -Wl,--sort-common -Wl,--as-needed

# Build flags
CPPFLAGS += -D_BSD_SOURCE -D_POSIX_C_SOURCE=2 -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}
CFLAGS += -g -std=c99 -pedantic -Wall ${INCS} ${CPPFLAGS}
#CFLAGS = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS += -g ${LIBS}
#LDFLAGS = -s ${LIBS}

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
