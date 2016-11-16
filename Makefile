hevc_parser: *.cpp
	gcc -o hevc_parser *.cpp

clean:
	rm hevc_parser
