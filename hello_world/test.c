#include <stdio.h>
#include <stdarg.h>


void print_new(const char *fmt, ...) {
	va_list args;

	va_start(args, fmt);
	int first_arg = va_arg(args, int);
	printf("First argument: %d\n", first_arg);
	char second_arg = va_arg(args, int); 
	printf("second argument: %c\n", second_arg);

}

int main() {
	printf("concatinate" "this" "without commas!\n");
	print_new("test this nice print\n", 1, 'x');
}
