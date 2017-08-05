#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/mman.h>



void main()
{
    int *DISPLAY_MEM_BASE_fb = 0;
    char *dev = "/dev/fb0";
    int fd;
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;
    char *buf;
    int i=0;
    if ((fd = open(dev, O_RDWR)) < 0)
    {
        printf("Unable to open framebuffer %s\n", dev);
        return;
    }
    if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
    {
        printf("Unable to get  FBIOGET_VSCREENINFO %s\n", dev);
        close(fd);
        return;
    }
    if (ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix) < 0)
    {
        printf("Unable to get  FBIOGET_FSCREENINFO %s\n", dev);
        close(fd);
        return;
    }
    printf("framebuffer info: bits=%d, w=%d, h=%d, mem_line=%d, mem_size=%d\n",
           fb_var.bits_per_pixel, fb_var.xres, fb_var.yres, fb_fix.line_length, fb_fix.smem_len);

    buf = (char *)mmap((void *)NULL, fb_fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ((int)buf == -1)
    {
        printf("Error: failed to map fb buffer to memory.\n");
        close(fd);
        return;
    }

    DISPLAY_MEM_BASE_fb = (int *)buf;
    for(i=0;i<(sizeof(DISPLAY_MEM_BASE_fb)/4);i++)
    {
        printf("%d\n",sizeof(DISPLAY_MEM_BASE_fb));
    }
    return;
}