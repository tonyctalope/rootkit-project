#include <linux/init.h>
#include <linux/module.h>

void hello(void) {
    pr_info("Hello from myModuleHello\n");
}

// Exporté pour être utilisé par d'autres modules
EXPORT_SYMBOL_GPL(hello);

static int __init hello_init(void) {
    pr_info("Hello, world!\n");
    return 0;
}

static void __exit hello_exit(void) {
    pr_info("Goodbye, cruel world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");