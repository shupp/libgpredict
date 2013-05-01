all: all-components libgpredict.a
all-components: sgpsdp/sgp_time.o sgpsdp/sgp_math.o sgpsdp/sgp4sdp4.o sgpsdp/sgp_in.o sgpsdp/sgp_obs.o sgpsdp/solar.o time-tools.o predict-tools.o gtk-sat-data.o orbit-tools.o qth-data.o sat-vis.o
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
predict-tools.o: sgpsdp/sgp4sdp4.o
	gcc -c predict-tools.c -Wall -o predict-tools.o `pkg-config --cflags --libs glib-2.0`
time-tools.o: sgpsdp/sgp4sdp4.o
	gcc -c time-tools.c -Wall -o time-tools.o `pkg-config --cflags --libs glib-2.0`
orbit-tools.o:
	gcc -c orbit-tools.c -Wall -o orbit-tools.o `pkg-config --cflags --libs glib-2.0`
gtk-sat-data.o:
	gcc -c gtk-sat-data.c -Wall -o gtk-sat-data.o `pkg-config --cflags --libs glib-2.0`
qth-data.o:
	gcc -c qth-data.c -Wall -o qth-data.o `pkg-config --cflags --libs glib-2.0`
sat-vis.o:
	gcc -c sat-vis.c -Wall -o sat-vis.o `pkg-config --cflags --libs glib-2.0`
sgpsdp/test-001: libgpredict.a
	gcc sgpsdp/test-001.c -Wall -o sgpsdp/test-001 -L. -lgpredict `pkg-config --cflags --libs glib-2.0` -lm
sgpsdp/test-002: libgpredict.a
	gcc sgpsdp/test-002.c -Wall -o sgpsdp/test-002 -L. -lgpredict `pkg-config --cflags --libs glib-2.0` -lm
iss-test: libgpredict.a
	gcc iss-test.c -Wall -o iss-test -L. -lgpredict `pkg-config --cflags --libs glib-2.0` -lm
test: sgpsdp/test-001 sgpsdp/test-002 iss-test
	(cd sgpsdp; ./test-001; ./test-002; ../iss-test)
libgpredict.a: all-components
	ar rcs libgpredict.a sgpsdp/sgp_time.o sgpsdp/sgp_math.o sgpsdp/sgp4sdp4.o sgpsdp/sgp_in.o sgpsdp/sgp_obs.o sgpsdp/solar.o predict-tools.o time-tools.o gtk-sat-data.o orbit-tools.o qth-data.o sat-vis.o

clean:
	rm -f `find . | egrep '(\.o$$|\.a$$)'`
	rm -f sgpsdp/test-001 sgpsdp/test-002 iss-test
