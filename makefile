build:
	gcc -o UnixLs UnixLs.c
clean:
	rm UnixLs
test:
	make build
	./UnixLs
	./UnixLs -l
	./UnixLs -i
	./UnixLs -l -i
	./UnixLs -i -l
	./UnixLs -il
	./UnixLs -li
	./UnixLs /home
	./UnixLs -l /home
	./UnixLs -i /home
	./UnixLs -l -i /home
	./UnixLs -i -l /home
	./UnixLs -il /home
	./UnixLs -li /home
	./UnixLs . .. /home
	./UnixLs -l . .. /home
	./UnixLs -i . .. /home
	./UnixLs -l -i . .. /home
	./UnixLs -i -l . .. /home
	./UnixLs -il . .. /home
	./UnixLs -li . .. /home
	./UnixLs /home -li
output:
	make test > sampleoutput.txt