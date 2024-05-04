# Closest Pairs Using Divide&Conquer Algorithm
My divide and conquer algorithm implementation project for solving closest pairs problem. Coding is done with C++ language.

To compile the code:
>>> g++ main.cpp-o main-Wall-Werror

Test cases along with calico are provided. To run calico for testing:
>>>calico test.yaml |& tee calico_log.txt

Valgrind tool can be used for memory leak check. To use it:
>>>valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./main <map_name> |& tee-a valgrind_log.txt
