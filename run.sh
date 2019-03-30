if gcc -O3 *.c -o tach.exe -lgc
then
    if python3 main.py -ec $1
    then
        time ./tach.exe
        rm out.bytecode
    fi
    rm tach.exe
fi