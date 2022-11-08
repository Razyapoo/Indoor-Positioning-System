#include <Arduino.h>

struct MyLink
{
    uint16_t tag_addr;
    struct MyLink *next;
};

struct MyLink *init_link();
void add_link(struct MyLink *p, uint16_t addr);
bool find_link(struct MyLink *p, uint16_t addr);
//void fresh_link(struct MyLink *p, uint16_t addr, float range);
//void print_link(struct MyLink *p);
void delete_link(struct MyLink *p, uint16_t addr);
//void make_link_json(struct MyLink *p,String *s);
