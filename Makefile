target := repnmem

c_opts = -Wall
c_opts += -I include/

src := $(wildcard src/*.c)

$target:
	gcc $(c_opts) $(src) -o $(target)

clean:
	rm $(target)
