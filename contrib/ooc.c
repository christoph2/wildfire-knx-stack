#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

void *new(const void *_class,...);
void delete(void *self);
void *clone(const void *self);
int differ(const void *self,const void *b);
size_t sizeOf(const void *self);

typedef struct tagClass {
    size_t size;
    void *(*ctor)(void * self,va_list * app);
    void *(*dtor)(void * self);
    void *(*clone)(const void *self);
    int (*differ)(const void *self, const void* b);
} Class;


void *new(const void *_class,...)
{ 
    const Class *cls=_class;
    void * p=calloc(1,cls->size);
    
    assert(p);
    *(const Class **) p=cls;
    
    if (cls->ctor) {
        va_list ap;
    va_start(ap, _class);
    p = cls->ctor(p,&ap);
    va_end(ap);
    }
    
    return p;
}


void delete(void *self)
{ 
    const Class **cp=self;

    if (self && *cp && (*cp)->dtor) {
        self=(*cp)->dtor(self);
    }
    free(self);
}


int differ(const void *self,const void *b)
{ 
    const Class *const *cp=self;
    
    assert(self && *cp && (*cp)->differ);

    return (*cp)->differ(self, b);
}

size_t sizeOf(const void *self)
{ 
    const Class *const *cp=self;

    assert(self && *cp);

    return (*cp)->size;
}
