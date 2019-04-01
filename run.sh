if gcc -O0 -g *.c -o tach.exe -lgc
then
    if python3 main.py -ec $1
    then
        time ./tach.exe
        # sudo perf record ./tach.exe
        # sudo perf report
        # sudo valgrind --tool=callgrind ./tach.exe
        # sudo valgrind ./tach.exe
    fi
    rm out.bytecode
fi
rm tach.exe