target := replicaters

c_opts = -Wall
c_opts += -I include/
c_opts += -ggdb
c_opts += -lpthread
c_opts += -z execstack

src := $(wildcard src/*.c)

$target:
	gcc $(c_opts) $(src) -o $(target)

clean:
	-@rm $(target) 2>/dev/null || true
