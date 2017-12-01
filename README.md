# Sm2Exe, a simple Smudge wrapper for generating PE executables.

### What does that mean?

**Sm2Exe** is a program which reads from an **SMK** file and outputs the bytecode
to the PE resources of a pre-built version of the **interpreter**.

To make a long story short, Sm2Exe is able to generate a independent PE file (`.exe`) from any Smudge program.

### Building
To build `sm2exe`, the procedure is the same as [Smudge](https://www.github.com/smudgelang/smudge).
To build a `Smudge Windows embedded` program just compile Smudge with option `-D_SM_WIN_EMBED`

### Usage
To build an independent executable you'll need:
- The **Smudge** interpreter (from [here](https://www.github.com/smudgelang/smudge))
- A **`Smudge Windows embedded`** build, compatible with the interpreter version (generally the same)
- An **`sm2exe`** build
- A Smudge **program** (`.sm` files)
- One or more icons **[OPTIONAL]**

First, **compile** your program into an **SMK** file with the following command (in our example `myapp.sm` is the main box):
```
smudge -c myapp.sm
```

This will generate the file **`myapp.smk`**, which we can pack into an executable:

```
./sm2exe -i DIST -I icon1.ico -I icon2.ico -o myapp.exe myapp.smk
```
Where `DIST` is:
- `latest-console.exe` for **32-bit console application** (default)
- `latest-window.exe` for **32-bit GUI application**
- `latest64-console.exe` for **64-bit console application**
- `latest64-window.exe` for **64-bit GUI application**
All the arguments except `myapp.smk` are optional. <br>

The executable created in this case will be named `myapp.exe`, and will contain the icons specified. It is completely independent from the interpreter and can be obviously executed without it. <br>
**`32-bit`** builds will need the dynamic libraries contained in folder **`libs32`** to run correctly, while **`64-bit`** those in the **`libs64`** one. You should take that into account when distributing your software.


### License

As the main project [Smudge](https://www.github.com/smudgelang/smudge), Sm2Exe is licensed under the Apache License 2.0: look at the `LICENSE` file to read more. <br>
Copyright 2017 Riccardo Musso
