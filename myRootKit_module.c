#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "myrootkit"
#define CLASS_NAME "rootkit"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux rootkit device driver");
MODULE_VERSION("1.0");

// Global variables
static int majorNumber;
static struct class *rootkitClass = NULL;
static struct device *rootkitDevice = NULL;
static char message[256] = {0};

// Function prototypes
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

// File operations structure
static struct file_operations fops = {
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .release = device_release,
};

// Module initialization function
static int __init rootkit_init(void) {
    // Print initialization message to the kernel log
    printk(KERN_INFO "Rootkit: Initializing...\n");

    // Register character device and obtain a major number
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "Rootkit failed to register a major number\n");
        return majorNumber;
    }
    // Print the major number to the kernel log
    printk(KERN_INFO "Rootkit registered with major number %d\n", majorNumber);

    // Create a device class
    rootkitClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(rootkitClass)) {
        // Clean up on error: unregister the character device
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(rootkitClass);
    }

    // Create a device instance
    rootkitDevice = device_create(rootkitClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(rootkitDevice)) {
        // Clean up on error: destroy the device class and unregister the character device
        class_destroy(rootkitClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create device\n");
        return PTR_ERR(rootkitDevice);
    }

    // Print success message to the kernel log
    printk(KERN_INFO "Rootkit device class created successfully\n");
    return 0; // Indicate successful initialization
}

// Module exit function
static void __exit rootkit_exit(void) {
    // Destroy the device instance
    device_destroy(rootkitClass, MKDEV(majorNumber, 0));
    // Unregister the device class
    class_unregister(rootkitClass);
    // Destroy the device class
    class_destroy(rootkitClass);
    // Unregister the character device
    unregister_chrdev(majorNumber, DEVICE_NAME);

    // Print goodbye message to the kernel log
    printk(KERN_INFO "Rootkit: Goodbye!\n");
}

// Device open function
static int device_open(struct inode *inodep, struct file *filep) {
    // Print device opened message to the kernel log
    printk(KERN_INFO "Rootkit: Device opened\n");
    return 0; // Indicate success
}

// Device read function
static ssize_t device_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    // Copy data from the message buffer to the user space
    int err_count = copy_to_user(buffer, message, len);
    if (err_count == 0) {
        // Print success message to the kernel log
        printk(KERN_INFO "Rootkit: Sent %zu characters to user\n", len);
        return 0; // Indicate success
    } else {
        // Print error message to the kernel log
        printk(KERN_INFO "Rootkit: Failed to send %d characters to user\n", err_count);
        return -EFAULT; // Return an error code
    }
}

// Device write function
static ssize_t device_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    // Limit the length of incoming data to the size of the message buffer
    if (len > sizeof(message)) {
        len = sizeof(message);
    }
    // Copy data from user space to the message buffer
    copy_from_user(message, buffer, len);
    // Print success message to the kernel log
    printk(KERN_INFO "Rootkit: Received %zu characters from user\n", len);
    return len; // Return the number of bytes written
}

// Device release function
static int device_release(struct inode *inodep, struct file *filep) {
    // Print device closed message to the kernel log
    printk(KERN_INFO "Rootkit: Device closed\n");
    return 0; // Indicate success
}

// Register the module initialization and exit functions
module_init(rootkit_init);
module_exit(rootkit_exit);
