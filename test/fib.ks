def fib(n) {
  if (n < 2) {
    return n;
  }
  return fib(n - 1) + fib(n - 2);
}

print(fib(1));
print(fib(2));
print(fib(3));
print(fib(4));
print(fib(5));
print(fib(6));
print(fib(7));
print(fib(8));
print(fib(9));
print(fib(10));
print(fib(20));
// print(fib(30));