target := replicaters

c_opts = -Wall
c_opts += -I include/
c_opts += -ggdb
c_opts += -lpthread
c_opts += -z execstack
c_opts += -masm=intel

lib = -lrt

ifeq (${DEBUG}, 1)
	c_opts += -DDEBUG
endif

src := $(wildcard src/*.c)

$target:
	gcc $(c_opts) $(src) -o $(target) $(lib)

clean:
	-@rm $(target) 2>/dev/null || true
