/* Jacob Hamrick
	jfhamri
	CPSC 102 Section 1
	April 3rd, 2015

	Updated for MP3 - list.cpp
*/

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

link_t::link_t(void *newentity)
{
   next = NULL;
   entity = newentity;
}

link_t::~link_t(void)
{
   fprintf(stderr, "in link destructor \n");
}

void link_t::set_next(link_t *new_next)
{
   next = new_next;
}

link_t * link_t::get_next()
{
   return next;
}

void * link_t::get_entity()
{
   return entity;
}


list_t::list_t()
{
   first = NULL;
   current = NULL;
   last = NULL;
}


void list_t::add(void *entity)
{
   /* Create a new link passing it the entity pointer */
   link_t *new_link;
   new_link = new link_t(entity);

   /* Now add the link to the list using generally the */
   /* same approach as the C version                   */
	
   if(first != NULL)
   {
      last->set_next(new_link);
      last = new_link;
      last->set_next(NULL);
      current = last;
   }

   else
   {
      first = new_link;
      last = new_link;
      current = last;
      new_link->set_next(NULL);
   }
}

void list_t::reset(void)
{
   current = first;
}

int list_t::not_end(void)
{
   int toreturn = 0;
   if(current != NULL)
      toreturn = 1;

   return toreturn;
}

void list_t::next_link(void)
{
   assert(current != NULL);
   current = current->get_next();
}

void *list_t::get_entity(void)
{
   assert(current != NULL);
   return current->get_entity();
}

list_t::~list_t()
{
   link_t *kill;
   fprintf(stderr, "in list destructor \n");

   /* Delete each link_t in the list */
	
   reset();
   while(current != NULL)
   {
      kill = current;
      current = current->get_next();
      delete kill;
   }
}


