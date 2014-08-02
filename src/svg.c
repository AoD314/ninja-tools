
#include "svg.h"
#include <string.h>

p_rect create_rect(uint_t x, uint_t y, uint_t width, uint_t height) {
    p_rect r = (p_rect)malloc( 1 * sizeof(struct rect) );
    r->x = x;
    r->y = y;
    r->w = width;
    r->h = height;
    return r;
}

p_text create_text(uint_t x, uint_t y, const string txt) {
    p_text t = (p_text)malloc( 1 * sizeof(struct text) );
    t->x = x;
    t->y = y;
    size_t len = strlen(txt);
    t->t = (char *)malloc(len + 1);
    strcpy(t->t, txt);
    return t;
}

p_svg svg_open(const string name)
{
    FILE * f = fopen(name, "w");
    if (f == NULL)
        return NULL;

    p_svg s = (p_svg)malloc(1 * sizeof(struct svg));
    s->f = f;
    fprintf(f, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
    fprintf(f, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
    fprintf(f, "<svg width=\"256\" height=\"64\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");
    fprintf(f, "<g fill=\"white\" stroke=\"black\" stroke-width=\"1\" font-family=\"helvetica\" font-size=\"4\">\n");
    return s;
}

void svg_close(p_svg s)
{
    fprintf(s->f, "</g></svg>");
    fclose(s->f);
    free(s);
}


void svg_add_rect(p_svg s, p_rect r) {
    fprintf(s->f, "<rect x=\"%llu\" y=\"%llu\" width=\"%llu\" height=\"%llu\"/>\n", r->x, r->y, r->w, r->h);
}

void svg_add_text(p_svg s, p_text t) {
    fprintf(s->f, "<text x=\"%llu\" y=\"%llu\"  fill=\"red\">%s</text>\n", t->x, t->y, t->t);
}
