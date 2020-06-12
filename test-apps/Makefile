
.DEFAULT_GOAL := all

# constants
IFLAGS = -I. \
	 -I../libsmc

CFLAGS = -g ${IFLAGS} -O0
LFLAGS = 

MKDIR  = mkdir -p

TARGET 			= arm_smc_test.bin
COMPILER_PATH	= /opt/tools/aarch64-none-linux/bin
COMPILER_PREFIX = ${COMPILER_PATH}/aarch64-none-linux-gnu-
CC     			= ${COMPILER_PREFIX}gcc


# objects
test_objs =  \
			main.o

lib_objs =  \
			../libsmc/smc_call.o


# rules
%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@


${TARGET}: ${test_objs} ${lib_objs}
	$(CC) -o ${TARGET} $^ $(CFLAGS) $(LFLAGS)


clean:
	$(RM) ${test_objs} ${lib_objs}
	$(RM) ${TARGET}


all: ${TARGET}
