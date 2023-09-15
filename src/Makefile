CFLAGS=-DHAVE_CONFIG_H -D_LARGEFILE64_SOURCE=1 -g -Wall -fPIC -D_GNU_SOURCE -g -O2 -Werror


all: CustomQuotaOss 
CustomQuotaOss: 
		g++ -g3 -fPIC  -I./src/ -c *.cc
		g++ -shared  -Wl,-soname,LibXrdCustomQuotaOss.so.4,--export-dynamic -o LibXrdCustomQuotaOss.so *.o -lXrdCl 
clean:
	-rm *.o *.a LibXrdCustomQuotaOss.so test
clang-format:
	clang-format -i -style=file  CustomQuotaOss.cc CustomQuotaOss.hh
