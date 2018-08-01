# ThreeWASM

Compile
```
em++ Three.cpp -o function.js -s EXPORTED_FUNCTIONS=[_calculate] -s EXTRA_EXPORTED_RUNTIME_METHODS[allocate,intArrayFromString,ALLOC_NORMAL,UTF32ToString,_free] -s WASM=1 -std=c++11 -s ALLOW_MEMORY_GROWTH=1 -O3
```
