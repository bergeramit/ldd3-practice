#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

const char test_message[] = "Hi There Man";

bool test_io(char *file_path) {
    char read_buf[100] = {0};
    FILE *fp = fopen(file_path, "wb");
    fflush(stdin);

    write(fileno(fp), "Hi There Man", sizeof(test_message));
    printf("Written: %s\n", test_message);
    fclose(fp);

    fp = fopen(file_path, "rb");
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

    if (memcmp(test_message, read_buf, sizeof(test_message)) == 0) {
        return true;
    }

    return false;
}