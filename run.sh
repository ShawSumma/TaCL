if gcc -O3 -g *.c -o tach.exe -lgc -lm
then
    time ./tach.exe $1
    # sudo perf record ./tach.exe
    # sudo perf report
    # gdb ./tach.exe
    # sudo valgrind --tool=callgrind ./tach.exe $1
    # sudo rm callgrind*
fi