CFLAGS=-DHAVE_CONFIG_H -D_LARGEFILE64_SOURCE=1 -g -Wall -fPIC -D_GNU_SOURCE -g -O2 -Werror


all: LustreOss 
LustreOss: lustreapi 
		g++ -g3 -fPIC  -I./src/ -c *.cc -std=c++14
		g++ -shared  -Wl,-soname,LibXrdLustreOss.so.4,--export-dynamic -o LibXrdLustreOss.so *.o -lXrdCl 
clean:
	-rm *.o *.a LibXrdLustreOss.so test
clang-format:
	clang-format -i -style=file  LustreOss.cc LustreOss.hh
