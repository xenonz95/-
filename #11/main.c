#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define PATH "/dev/input/event4"

char out_code(int code)
{
    char c;
    if(code>1 && code<11)
        c = '1'+(code - 1);
    if(code == 11)
        c = '0';
    switch(code){
        case 16:c = 'q';break;
        case 17:c = 'w';break;
        case 18:c = 'e';break;
        case 19:c = 'r';break;
        case 20:c = 't';break;
        case 21:c = 'y';break;
        case 22:c = 'u';break;
        case 23:c = 'i';break;
        case 24:c = 'o';break;
        case 25:c = 'p';break;
        case 30:c = 'a';break;
        case 31:c = 's';break;
        case 32:c = 'd';break;
        case 33:c = 'f';break;
        case 34:c = 'g';break;
        case 35:c = 'h';break;
        case 36:c = 'j';break;
        case 37:c = 'k';break;
        case 38:c = 'l';break;
        case 44:c = 'z';break;
        case 45:c = 'x';break;
        case 46:c = 'c';break;
        case 47:c = 'v';break;
        case 48:c = 'b';break;
        case 49:c = 'n';break;
        case 50:c = 'm';break;
    default : printf ("|isn't a normal key");break;
    }
    return c;
}

int main()
{
    int file_key;
    struct input_event input_ev_one;
    file_key = open(PATH ,O_NONBLOCK);
    if(file_key <= 0)
    {
        printf("open keyboard(event4) filed\n");
        return -1;
    }
    while(1)
    {
        if(read(file_key,&input_ev_one,sizeof input_ev_one) == sizeof input_ev_one)
        {
            if(input_ev_one.type == EV_KEY)
            {
                if(input_ev_one.value == 0)
                {
                    printf("\nThis is key ");
                    printf("%c",out_code(input_ev_one.code));
                    putchar('\n');
                    if(input_ev_one.code == KEY_ESC)
                        break;
                }
            }
        }
    }
    close(file_key);
    return 0;
}