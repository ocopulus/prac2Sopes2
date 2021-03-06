#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

EXPORT_SYMBOL(sys_write);
EXPORT_SYMBOL(sys_open);
EXPORT_SYMBOL(sys_close);
EXPORT_SYMBOL(sys_read);

int cantidad = 0;
int datos[50];

static void write_file(char *filename, char *data)
{
  struct file *file;
  loff_t pos = 0;
  int fd;

  mm_segment_t old_fs = get_fs();
  set_fs(KERNEL_DS);

  fd = sys_open(filename, O_WRONLY|O_CREAT, 0644);
  if (fd >= 0) {
    sys_write(fd, data, strlen(data));
    file = fget(fd);
    if (file) {
      vfs_write(file, data, strlen(data), &pos);
      fput(file);
    }
    sys_close(fd);
  }
  set_fs(old_fs);
}

static void read_file(char *filename)
{
  int fd;
  char buf[1];

  mm_segment_t old_fs = get_fs();
  set_fs(KERNEL_DS);

  fd = sys_open(filename, O_RDONLY, 0);
  if (fd >= 0) {
    printk(KERN_DEBUG);	
    while (sys_read(fd, buf, 1) == 1)
      printk("%c", buf[0]);
    printk("\n");
    sys_close(fd);
  }
  set_fs(old_fs);
  write_file("/statistics/salida.JSON", "{Funciona papu}");
}

static int __init init(void)
{
  read_file("/statistics/datos.txt");
  return 0;
}

static void __exit exit(void)
{ 
  printk(KERN_INFO "Adios Modulo\n");
}

MODULE_LICENSE("GPL");
module_init(init);
module_exit(exit);
