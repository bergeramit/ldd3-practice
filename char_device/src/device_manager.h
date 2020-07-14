#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

extern int char_driver__major;

int setup_cdev(struct cdev **my_cdev,
               struct file_operations *fops,
               dev_t char_device_identifier);

int setup_device_region(dev_t *region_identifier_out,
                        int first_minor,
                        int number_of_devices);

#endif /* DEVICE_MANAGER */