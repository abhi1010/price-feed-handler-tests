
#g++ test.cpp -lboost_system test.cpp -lpthread

_GPPFLAGS=-std=gnu++11 -I /usr/local/include/
_BOOSTFLAGS=-I /home/dev/boost_1_54_0/include -L /home/dev/boost_1_54_0/lib
_BOOSTLIBS=-lboost_chrono -lboost_date_time -lboost_filesystem -lboost_graph -lboost_iostreams -lboost_locale -lboost_log_setup -lboost_log -lboost_math_c99f -lboost_math_c99l -lboost_math_c99 -lboost_math_tr1f -lboost_math_tr1l -lboost_math_tr1 -lboost_prg_exec_monitor -lboost_program_options -lboost_python -lboost_random -lboost_regex -lboost_serialization -lboost_signals -lboost_system -lboost_thread -lboost_timer -lboost_unit_test_framework -lboost_wave -lboost_wserialization -lpthread -D__STRICT_ANSI__ 

all: clean hello mcr test log.basic log.trivial log.async b_regex b_ref b_server

b: b_server b_client

CC_CLANG = clang++
CC_GCC = g++

CXX='/usr/bin/g++'
g: CXX='/usr/bin/g++'
c: CXX='/home/dev/clang_3.2/bin/clang++'

hello:
	g++ $(_GPPFLAGS) hello_world.cpp  -lboost_system -lpthread -o hello.world.out


mcr:
	g++ $(_GPPFLAGS) multicast_receiver.cpp  -lboost_system -lpthread -o mcr.out

mc_sender:
	g++ $(_GPPFLAGS) multicast_sender.cpp  -lboost_system -lpthread -o mc_sender.out

mcpub:
	g++ $(_GPPFLAGS) mcpub.cpp  -lboost_system -lpthread -o mcpub

test:
	g++ $(_GPPFLAGS) test.cpp  -lboost_system -lpthread -o test.out

log.basic:
	$(CC_GCC) $(_GPPFLAGS) log_basic.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o log.basic.out 

log.trivial:
	g++ $(_GPPFLAGS) log_trivial.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o log.trivial.out 

log.async:
	g++ $(_GPPFLAGS) log_async.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o log.async.out

log.thread:
	g++ $(_GPPFLAGS) log_threads.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o log.thread.out

b_ref:
	g++ $(_GPPFLAGS) reference_counted.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o b_ref.out

b_server:
	g++ $(_GPPFLAGS) async_tcp_server.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o b_server.out

b_client:
	g++ $(_GPPFLAGS) async_tcp_client.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o b_client.out

b_regex:
	g++ $(_GPPFLAGS) boost_regex_ex.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o b_regex.out

b.lockfree:
	g++ $(_GPPFLAGS) samplelockfree.cpp $(_BOOSTLIBS) $(_BOOSTFLAGS) -o b.lockfree.out


clean:
	rm -f *.out
#c++ test.cpp -I /usr/local/include -lboost_system -lpthread

