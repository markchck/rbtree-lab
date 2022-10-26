## Fragment to pass common settings into arch subdir builds.
## NB: This should eventually be merged into sim/Makefile.am once there are no
## subdir configure scripts.
#
#   Copyright (C) 1993-2022 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

host_alias = x86_64-pc-linux-gnu
target_alias = x86_64-pc-linux-gnu
program_transform_name = s,y,y,

EXEEXT = 

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = /usr/bin/install -c
INSTALL_DATA = /usr/bin/install -c -m 644
LN_S = ln -s

CC = gcc
C_DIALECT = 
CC_FOR_BUILD = gcc
CFLAGS_FOR_BUILD = -g -O2
CFLAGS = -g -O2    
CPPFLAGS = 
CXXFLAGS = @CXXFLAGS@
LDFLAGS =  

AR = ar --plugin /usr/lib/gcc/x86_64-linux-gnu/11/liblto_plugin.so --plugin /usr/lib/gcc/x86_64-linux-gnu/11/liblto_plugin.so
AR_FLAGS = rc
RANLIB = ranlib --plugin /usr/lib/gcc/x86_64-linux-gnu/11/liblto_plugin.so --plugin /usr/lib/gcc/x86_64-linux-gnu/11/liblto_plugin.so
DTC = 

DEPMODE = depmode=gcc3
DEPDIR = .deps

zlibdir = -L$(top_builddir)/../zlib
LIBDL = 
LIBINTL = 
LIBINTL_DEP = 
INTL_CFLAGS = 
SDL_CFLAGS = 
SDL_LIBS = 
TERMCAP_LIB = 
READLINE_LIB = ../../readline/readline/libreadline.a
READLINE_CFLAGS = -I$(READLINE_SRC)/..

COMMON_LIBS = -ldl -lm -lm -lnsl 
WARN_CFLAGS =  -Wall -Wdeclaration-after-statement -Wpointer-arith -Wno-unused -Wunused-value -Wunused-function -Wno-switch -Wno-char-subscripts -Wempty-body -Wunused-but-set-parameter -Wno-error=maybe-uninitialized -Wmissing-declarations -Wmissing-prototypes -Wdeclaration-after-statement -Wmissing-parameter-type -Wpointer-sign -Wold-style-declaration -Wformat-nonliteral
WERROR_CFLAGS = -Werror

# Maintainer support, enable with --enable-maintainer-mode.
# Once all subdirs are moved to automake, we can drop this custom logic and
# switch back to using # directly.
MAINT = ; @true
# The following line is commented depending upon --enable-maintainer-mode.
#MAINT =

# cgen support, enable with --enable-cgen-maint
CGEN_MAINT = ; @true
# The following line is commented in or out depending upon --enable-cgen-maint.
#CGEN_MAINT =
CGENDIR = $(srcdir)/../../cgen

SIM_INLINE = -DDEFAULT_INLINE=0

SIM_HW_CFLAGS = -DWITH_HW=1 -DHAVE_DV_SOCKSER
SIM_HW_SOCKSER = dv-sockser.o
SIM_HW_OBJS = $(SIM_COMMON_HW_OBJS) $(SIM_HW_DEVICES:%=dv-%.o) $(SIM_HW_SOCKSER)
#SIM_HW_OBJS =
