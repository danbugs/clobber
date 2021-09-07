#include <stdio.h>
#include <emscripten.h>
#include "../dependencies/dcw/dcw.h"

html_t main_h =
#include "../frontend/main.cml"
    ;

html_t jumbotron_h =
#include "../frontend/jumbotron.cml"
    ;

html_t form_h =
#include "../frontend/form.cml"
    ;

html_t feed_h =
#include "../frontend/feed.cml"
    ;

extern void display_html(html_t raw_html);

EMSCRIPTEN_KEEPALIVE
html_t jumbotron()
{
    return jumbotron_h;
}

EMSCRIPTEN_KEEPALIVE
html_t form()
{
    return form_h;
}

EMSCRIPTEN_KEEPALIVE
html_t feed()
{
    return feed_h;
}

int main()
{
    display_html(main_h);
}