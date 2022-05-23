# Quick and ok!

ARCH?=x86-64

LIBNAME=libhttp_parser.a
CXXFLAGS=-std=c++17

# Debug build? debug is default
ifdef DEBUG
CXXFLAGS+=-g 
DEST_DIR=lib/${ARCH}/debug
MODE=debug
else
CXXFLAGS+=-O2
DEST_DIR=lib/${ARCH}/release
MODE=release
endif
OBJ_DEST_DIR=build/${DEST_DIR}


COMMON=http_parser.c
SRC=${COMMON} 
OBJS=$(addprefix ${OBJ_DEST_DIR}/,$(notdir ${SRC:%.c=%.o}))
DEPS=$(addprefix ${OBJ_DEST_DIR}/,$(notdir ${SRC:%.c=%.d}))

ifeq (${ARCH},x86-64)
# Include x86-64 specific stuff
include compiler_setup.x86-64
endif

ifeq (${ARCH},arm)
# Include arm specific stuff
include compiler_setup.arm
endif

# Setting compiler flags
CXXFLAGS+= -Wall -Iinc  -D${OS}
VPATH=common linux

all: info ${DEST_DIR}/${LIBNAME}

info:
	@echo "Compiling for '${ARCH}' in '${MODE}' mode..."

${OBJ_DEST_DIR}/%.o: %.c
	@echo "Compiling "$<
	@${CXX} -c $< -o $@ ${CXXFLAGS}

${OBJ_DEST_DIR}/%.d: %.c
	@echo "Generating dependency for $<"
	@mkdir -p ${OBJ_DEST_DIR}
	@$(CXX) -MT${OBJ_DEST_DIR}/$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

${DEST_DIR}:
	@mkdir -p ${DEST_DIR}

${OBJ_DEST_DIR}:
	@mkdir -p ${OBJ_DEST_DIR}

${DEST_DIR}/${LIBNAME}: ${OBJ_DEST_DIR} ${DEST_DIR} $(DEPS) ${OBJS} 
	@echo "Linking "$<
	@${AR} cr $@ ${OBJS}


clean:
	@echo "Cleaning up..."
	@rm -f ${DEST_DIR}/${LIBNAME} $(OBJS) $(DEPS)

-include $(DEPS)
