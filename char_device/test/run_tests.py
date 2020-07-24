import ctypes
import logging

BASIC_TESTS_SO_PATH = "./basic_tests.so"

def run_test(test, expected_result, *args):
    result = test(*args)
    try:
        assert result == expected_result
        logging.info("test: {} Ran Successfully".format(test.__name__))
    except AssersionError:
        logging.error("Unexpected Result!")

def test_device_io(device_file_path):
    logging.info("Running test_device_io using the .so: {}".format(BASIC_TESTS_SO_PATH))
    basic_tests = ctypes.CDLL(BASIC_TESTS_SO_PATH)
    basic_tests.test_io.argtypes = [ctypes.c_char_p]
    arg_file_path = ctypes.c_char_p(bytes(device_file_path, "UTF-8"))
    run_test(basic_tests.test_io, True, arg_file_path)

def main():
    logging.info("Begin Running Tests")
    test_device_io("a.txt")

if __name__ == "__main__":
    main()