if gcc -O3 *.c -o tach.exe -lgc -lm
then
    # if python3 main.py -ec $1
    # then

    # time ./tach.exe tach/parse.tach
    # sudo perf record ./tach.exe
    # sudo perf report
    sudo valgrind --tool=callgrind ./tach.exe tach/parse.tach
    # sudo valgrind ./tach.exe
    
    # fi
    # rm out.bytecode
    rm tach.exe
    sudo rm callgrind*
fi