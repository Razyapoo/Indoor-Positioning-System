#include "link.h"

//#define SERIAL_DEBUG

struct MyLink *init_link()
{
#ifdef SERIAL_DEBUG
    Serial.println("init_link");
#endif
    struct MyLink *p = (struct MyLink *)malloc(sizeof(struct MyLink));
    p->next = NULL;
    p->tag_addr = 0;

    return p;
}

void add_link(struct MyLink *p, uint16_t addr)
{
#ifdef SERIAL_DEBUG
    Serial.println("add_link");
#endif
    struct MyLink *temp = p;
    //Find struct MyLink end
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    //Serial.println("add_link:find struct MyLink end");
    //Create a anchor
    struct MyLink *a = (struct MyLink *)malloc(sizeof(struct MyLink));
    a->tag_addr = addr;
    a->next = NULL;

    //Add anchor to end of struct MyLink
    temp->next = a;

    return;
}

bool find_link(struct MyLink *p, uint16_t addr)
{
#ifdef SERIAL_DEBUG
    Serial.println("find_link");
#endif
    // Serial.println("addr: " + (String)addr);
    // Serial.println(addr);
    if (addr == 0)
    {
        Serial.println("find_link:Input addr is 0");
        return false;
    }

    if (p->next == NULL)
    {
        Serial.println("find_link:Link is empty");
        return false;
    }

    struct MyLink *temp = p;
    //Find target struct MyLink or struct MyLink end
    while (temp->next != NULL)
    {
        temp = temp->next;
        if (temp->tag_addr == addr)
        {
            Serial.println("find_link:Find addr");
            return true;
        }
    }

    Serial.println("find_link:Can't find addr");
    return false;
}

// void fresh_link(struct MyLink *p, uint16_t addr, float range)
// {
// #ifdef SERIAL_DEBUG
//     Serial.println("fresh_link");
// #endif
//     struct MyLink *temp = find_link(p, addr);
//     if (temp != NULL)
//     {
//         temp->range = range;
//         return;
//     }
//     else
//     {
//         Serial.println("fresh_link:Fresh fail");
//         return;
//     }
// }

// void print_link(struct MyLink *p)
// {
// #ifdef SERIAL_DEBUG
//     Serial.println("print_link");
// #endif
//     struct MyLink *temp = p;

//     while (temp->next != NULL)
//     {
//         //Serial.println("Dev %d:%d m", temp->next->tag_addr, temp->next->range);
//         Serial.println(temp->next->tag_addr, HEX);
// //        Serial.println(temp->next->range[0]);/
//         Serial.println(temp->next->range);
//         temp = temp->next;
//     }

//     return;
// }

void delete_link(struct MyLink *p, uint16_t addr)
{
#ifdef SERIAL_DEBUG
    Serial.println("delete_link");
#endif
    if (addr == 0)
        return;

    struct MyLink *temp = p;
    while (temp->next != NULL)
    {
        if (temp->next->tag_addr == addr)
        {
            struct MyLink *del = temp->next;
            temp->next = del->next;
            free(del);
            return;
        }
        temp = temp->next;
    }
    return;
}

// void make_link_json(struct MyLink *p, String *s)
// {
// #ifdef SERIAL_DEBUG
//     Serial.println("make_link_json");
// #endif
//     s = String(p->tagId) + "{\"links\":[";
//     struct MyLink *temp = p;
//     Serial.println(*s);
    
//     while (temp->next != NULL)
//     {
//         #ifdef SERIAL_DEBUG
//           Serial.println("in_while");
//         #endif
//         temp = temp->next;
//         char link_json[50];
//         #ifdef SERIAL_DEBUG
//           Serial.println("before_sprintf");
//         #endif
//         sprintf(link_json, "{\"Tag ID\":\"%u\",\"Anchor ID\":\"%X\",\"R\":\"%.2f\"}", temp->tagId, temp->tag_addr, temp->range);
//         #ifdef SERIAL_DEBUG
//           Serial.println("after_sprintf");
//         #endif
//         s += link_json;
//         if (temp->next != NULL)
//         {
//             s += ",";
//         }
//     }
//     //Serial.println("after_while");
//     s += "]}";
//     Serial.println(*s);
// }
