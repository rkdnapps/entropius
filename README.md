# entropius

A simple program that generates random numbers and finds potential prime factors.

Started as a pandemic project to test the speed of finding prime factors from
random input on Linux. Picked it back up recently to test the free version
of ChatGPT and its ability to help port/improve code. Expanded it to allow
for multi-threading and variable bit lengths for the input, and ported it
to Windows under MSYS2.

If you've never used MSYS under Windows, you'll want to read these:

https://www.msys2.org
https://learn.microsoft.com/en-us/vcpkg/users/platforms/mingw

Make sure to do:

source shell mingw64 

To get an actual .exe built, instead of just a COFF binary.

Simple build:

Admin@nidhogg MINGW64 /c/users/Admin/source
$ gcc -Wall -static -o entropius_gpt_128 ./entropius_gpt_128.c -lgmp

Some example runs from 32 to 36bits on a Threadripper 3960X:

Admin@nidhogg MINGW64 /c/users/Admin/source
$ time ./entropius_gpt_128 48 32
Input number: 740385957
Thread 1: Largest prime factor so far: 3
Thread 17: Largest prime factor so far: 246795319
Thread 16: Largest prime factor so far: 246795319
Largest prime factor: 246795319

real    0m0.946s
user    0m0.000s
sys     0m0.000s

Admin@nidhogg MINGW64 /c/users/Admin/source
$ time ./entropius_gpt_128 48 33
Input number: 6956946304
Thread 1: Largest prime factor so far: 2
Thread 1: Largest prime factor so far: 7
Thread 1: Largest prime factor so far: 11
Thread 1: Largest prime factor so far: 89
Thread 1: Largest prime factor so far: 103
Largest prime factor: 103

real    0m8.455s
user    0m0.000s
sys     0m0.000s

Admin@nidhogg MINGW64 /c/users/Admin/source
$ time ./entropius_gpt_128 48 34
Input number: 7229084960
Thread 1: Largest prime factor so far: 2
Thread 1: Largest prime factor so far: 5
Thread 1: Largest prime factor so far: 45181781
Largest prime factor: 45181781

real    0m8.637s
user    0m0.000s
sys     0m0.015s

Admin@nidhogg MINGW64 /c/users/Admin/source
$ time ./entropius_gpt_128 48 35
Input number: 25132434249
Thread 1: Largest prime factor so far: 3
Thread 1: Largest prime factor so far: 13
Thread 1: Largest prime factor so far: 1283
Thread 1: Largest prime factor so far: 502277
Largest prime factor: 502277

real    0m24.611s
user    0m0.000s
sys     0m0.000s

Admin@nidhogg MINGW64 /c/users/Admin/source
$ time ./entropius_gpt_128 48 36
Input number: 34058405283
Thread 1: Largest prime factor so far: 3
Thread 1: Largest prime factor so far: 7
Thread 1: Largest prime factor so far: 1289
Thread 1: Largest prime factor so far: 1258207
Largest prime factor: 1258207

real    0m32.244s
user    0m0.000s
sys     0m0.000s


