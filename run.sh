if tcc -O3 *.c -o tach.exe -lgc
then
    if python3 main.py -ec $1
    then
        time ./tach.exe
    fi
    rm out.bytecode
fi
rm tach.exe