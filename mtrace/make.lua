os.execute("gcc -o m5 memleak5.c -g -lmcheck  -rdynamic  -no-pie -Wl,-rpath=./test.log")
os.execute("g++ -std=c++11 -o logri mtracegori.cc -g")
