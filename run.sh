if gcc -O3 *.c -o tach.exe -lgc -lm
then
    time ./tach.exe $1
    # sudo perf record ./tach.exe
    # sudo perf report
fi