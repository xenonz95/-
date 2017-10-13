#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct link
{
    string s;
    int num;
    link * next;
    link()
    {
        num = 0;
        next = NULL;
    }
    void test(string ts)
    {
        link * p;
        p = this;
        int mark = 0;
        while(p->next != NULL)
        {
            if (ts == p->s)
            {
                ++(p->num);
                mark = 1;
            }
            p = p->next;
        }
        if(!mark)
        {
            this->add(ts);
        }
    }
    void add(string ins)
    {
        if(this->next == NULL)
        {
            this->next = new link;
            this->next->s = ins;
            this->next->next = NULL;
            return;
        }
        
        link * p,*temp;
        p = this;
        while(p->next->next != NULL)
        {
            cout <<"test\n";
            if(ins > p->s && ins <= p->next->s)
            {
                temp = p->next;
                p->next = new link;
                p->next->s = ins;
                p->next->next = temp;
                break;
            }
            else if(ins > p->s && ins > p->next->s)
            {
                p = p->next;
            }
            else
            {
                temp = p->next;
                p->next = new link;
                p->next = p;
                p->s = ins;
                p->num = 0;
                p->next->next = temp;
                break;
            }
        }
        if(this->next->next == NULL)
        {
            if(ins > p->s && ins <= p->next->s)
            {
                temp = p->next;
                p->next = new link;
                p->next->s = ins;
                p->next->next = NULL;
            }
            else if(ins > p->s && ins > p->next->s)
            {
                p = p->next;
            }
            else
            {
                temp = p->next;
                p->next = new link;
                p->next = p;
                p->s = ins;
                p->num = 0;
                p->next->next = NULL;
            }
        }
        return;
    }
    void print()
    {
        link * p;
        p = this;
        while(p->next != NULL)
        {
            cout << p->s << ' ' << p->num << endl;
            p = p->next;
        }
    }
};




char changeChar(char c)
{
    if(c >= '0' && c<= '9')
    return c;
    if(c == '-')
    return -1;
    if(c >= 'A' && c<= 'Z')
    {
        if(c == 'Q'|| c == 'Z')
            return -1;
        if(c > 'Q' )
            c-=1;
        return (c-'A')/3+2+'0';
    }
}

int main()
{
    ifstream input("input");
    ofstream output("output");
    string str,temp;
    stringstream ss,outss;
    char c;
    int num;
    input >> num;
    link head;
    while(input >> str)
    {
        outss.clear();
        //output << str <<endl;
        ss << str;
        while(ss >> c)
        {
            if(changeChar(c)>0)
                outss << changeChar(c);
        }
        ss.clear();
        outss >> str;
        output << str <<endl;
    }
    cout <<"tests\n";
    link a;
    a.test("12345");
    
    a.test("12345");
    a.test("72348");
    a.print();



    input.close();
    output.close();
    return 0;
}