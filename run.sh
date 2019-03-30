gcc -O3 main.c -o tach.exe -lgc
python3 main.py -ec $1
./tach.exe
rm out.bytecode
rm tach.exe