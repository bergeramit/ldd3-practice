#include <stdio.h>
#include <unistd.h>

const char test_message[] = "Hi There Man";

int main(int argc, char *argv[]) {
    char read_buf[100] = {0};
    FILE *fp = fopen(argv[1], "wb");
    fflush(stdin);

    write(fileno(fp), "Hi There Man", sizeof(test_message));
    printf("Written: %s\n", test_message);
    fclose(fp);

    fp = fopen(argv[1], "rb");
    /*
     * fread(read_buf, 3, 1, fp); --> will not read 3 bytes!
     * 
     * This fread is internal to libc and is using a buffer
     * so the calls will always be for 4096, in order to read
     * exact data we need to use the 'read' without middle buffers
     * the 'read' in #include <unistd.h> does just that
     */
    read(fileno(fp), read_buf, sizeof(test_message)); // --> will read 3 bytes

    printf("The read data is: %s\n", read_buf);
    fclose(fp);
}