run:
	./logparser.bash /home/2013/jdsilv2/206/mini3/logset1

clean:
	rm logdata.csv stats.csv stats.html 2> /dev/null

check:
	shellcheck logparser.bash

test:
	diff logdata.csv spec/logdata.csv
	diff stats.csv spec/stats.csv
	diff stats.html spec/stats.html

view:
	lynx stats.html
