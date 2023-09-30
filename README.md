# Linux-Shell

A simple linux shell.

# Features

* You can execute every command you have on your system since everything is executed via ``execvp``  
* Input/Output Redirection
* Signal handling. If a child proccess is running it will receive the signal, othewise the terminal receives the signal.
* Multiple commands supported via ; (For example ls;ls)
* Supports history (type 'history' on the shell to view history)
* Support custom allias (in the shell session)  
```bash
createalias lll "ls -las"
lll
```
* Enviromental variables support
```bash
cd ${HOME}
```
  
* Auto completion using tab is not supported.
* Arrow keys not supported.

# Build

Only linux is supported   
CMake is as the build system  

```bash
mkdir build
cd build
cmake ..
make
./shell
```