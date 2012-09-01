all: all-components libgpredict.a
all-components: sgpsdp/sgp_time.o sgpsdp/sgp_math.o sgpsdp/sgp4sdp4.o sgpsdp/sgp_in.o sgpsdp/sgp_obs.o sgpsdp/solar.o time-tools.o
sgpsdp/solar.o:
	gcc -c sgpsdp/solar.c -Wall -o sgpsdp/solar.o `pkg-config --cflags --libs glib-2.0` -lm
sgpsdp/sgp_obs.o:
	gcc -c sgpsdp/sgp_obs.c -Wall -o sgpsdp/sgp_obs.o `pkg-config --cflags --libs glib-2.0` -lm
sgpsdp/sgp_in.o:
	gcc -c sgpsdp/sgp_in.c -Wall -o sgpsdp/sgp_in.o `pkg-config --cflags --libs glib-2.0` -lm
sgpsdp/sgp_time.o:
	gcc -c sgpsdp/sgp_time.c -Wall -o sgpsdp/sgp_time.o `pkg-config --cflags --libs glib-2.0` -lm
sgpsdp/sgp_math.o:
	gcc -c sgpsdp/sgp_math.c -Wall -o sgpsdp/sgp_math.o `pkg-config --cflags --libs glib-2.0` -lm
sgpsdp/sgp4sdp4.o: sgpsdp/sgp_math.o
	gcc -c sgpsdp/sgp4sdp4.c -Wall -o sgpsdp/sgp4sdp4.o `pkg-config --cflags --libs glib-2.0` -lm
time-tools.o: sgpsdp/sgp4sdp4.o
	gcc -c time-tools.c -Wall -o time-tools.o `pkg-config --cflags --libs glib-2.0`
sgpsdp/test-001: libgpredict.a
	gcc sgpsdp/test-001.c -Wall -o sgpsdp/test-001 -L. -lgpredict `pkg-config --cflags --libs glib-2.0` -lm
sgpsdp/test-002: libgpredict.a
	gcc sgpsdp/test-002.c -Wall -o sgpsdp/test-002 -L. -lgpredict `pkg-config --cflags --libs glib-2.0` -lm
run-tests: sgpsdp/test-001 sgpsdp/test-002
	(cd sgpsdp; ./test-001; ./test-002)
libgpredict.a: all-components
	ar rcs libgpredict.a sgpsdp/sgp_time.o sgpsdp/sgp_math.o sgpsdp/sgp4sdp4.o sgpsdp/sgp_in.o sgpsdp/sgp_obs.o sgpsdp/solar.o time-tools.o

clean:
	rm -f `find . | egrep '(\.o$$|\.a$$)'`
	rm -f sgpsdp/test-001 sgpsdp/test-002
