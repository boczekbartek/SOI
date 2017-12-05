[![Build Status](https://travis-ci.com/boczekbartek/SOI.svg?token=6nq26MsXXRA1CVRzsEzJ&branch=master)](https://travis-ci.com/boczekbartek/SOI)

# SOI
Operating System on Warsaw University of Technology

# Installation
        mkdir -p build && cd build
        cmake ..
        make -j4
        
# Task (in Polish)
Bufor n-elementowy FIFO (n > 3). Dwóch konsumentów i dwóch producentów. 
Producent A produkuje jedną literę, Producent B produkuje jednocześnie dwie 
litery. Element jest usuwany z bufora po odczytaniu przez obu konsumentów. 
Dany element nie może być wielokrotnie czytany przez tego samego konsumenta. 
Ponadto, liczba elementów w buforze po usunięciu elementu przez dowolnego 
konsumenta nie może spaść poniżej 3.


