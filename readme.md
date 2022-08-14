# Zap's C++ template

My personal C++ template with plain makefile.
I might not use the best practices but this is what works for me.
Any feedback is greatly appreciated.

## Usage and conventions

Change the program name in the `makefile`

Create `.cpp` and `.hpp` files in `src` folder. They will automatically be
detected and compiled.

Store template classes and functions in `.h` files, also in the `src` folder.

Build with `make debug` for debug mode with incremental compilation and
`make build` for optimized build.

To use libraries, create `lib` directory and add git submodules there.

## License

This template is licensed under the [MIT License](LICENSE).
