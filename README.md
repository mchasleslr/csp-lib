# csp-lib

## Setup

### Debug

```bash
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Release

```bash
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Tests

```bash
make test
```

## Execution

### N-Queens

```bash
./solve-queens <number_of_queens>
```

### Sudoku

```bash
./solve-sudoku
```
