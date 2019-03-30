if clang -O3 *.c -o tach.exe -lgc
then
    if python3 main.py -ec $1
    then
        ./tach.exe
    fi
fi