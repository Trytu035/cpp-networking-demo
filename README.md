
# Setup
In the top-level project directory there should be created a file named `config.txt`.
The file should contain cmake code setting up variable `Boost_DIR_Path` to the location of installed boost library.
Example `config.txt` content:
```
set(Boost_DIR_Path "Absolute_path.../../boost_1_89_0/lib64-msvc-14.3/cmake/Boost-1.89.0")
```