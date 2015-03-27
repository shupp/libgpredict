# libgpredict

This is an attempt to take the prediction code out of Gpredict and make it a standalone dynamic
library.

## dependencies
### mac os x

    brew install glib

### linux

    sudo apt-get install libglib2.0-dev

## install

    git clone https://github.com/cubehub/libgpredict.git
    cd libgpredict
    mkdir build
    cd build
    cmake ../
    make
    make install

## test

    cd test
    mkdir build
    cd build
    cmake ../
    make
    ./iss-test

It should print this:

```
Testing a single ISS prediction

TLE Data OK

Lat:       18.389813558938   (expected:  18.389606541458)
Lon:       -36.880015960342  (expected: -36.879846099166)
Alt:        409.7429431026   (expected: 409.7429029679)
Footprint:  4454.9076239034  (expected: 4454.9074167831)
Velocity:  7.6691931516      (expected: 7.66919316690)
```
