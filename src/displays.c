html_t jumbotron_h =
#include "../frontend/jumbotron.cml"
    ;
EMSCRIPTEN_KEEPALIVE
html_t jumbotron()
{
    return jumbotron_h;
}

html_t form_h =
#include "../frontend/form.cml"
    ;
EMSCRIPTEN_KEEPALIVE
html_t form()
{
    return form_h;
}