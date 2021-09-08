#ifndef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void get_all_clobs()
{
    EM_ASM(
        let element = document.querySelectorAll('.post');
        if (element) {
            element.forEach(function(e){e.remove()});
        });

    int i;
    for (i = num_clobs - 1; i >= 0; i--)
    {
        char *tmp;
        asprintf(&tmp, "<div class=\"container mb-3 post\"> \
                <div class=\"row\"> \
                    <div class=\"card col-md-12\"> \
                        <div class=\"card-body\"> \
                            <p class=\"card-text\">%s \
                        </div> \
                    </div> \
                </div> \
            </div>",
                 *(clobs + i));
        free(*(clobs + i));

        display_html(tmp);
    }
    free(clobs);
}

EMSCRIPTEN_KEEPALIVE
void set_clob(char *string)
{
    asprintf(&curr_clob, "%s", string);
    EM_ASM(
        let element = document.querySelector('#char_count');
        while (element.lastChild) {
            element.removeChild(element.lastChild);
        });
    char *tmp;
    int char_count = (int)strlen(string);
    if (char_count > 255)
    {
        asprintf(&tmp, "<p style=\"color:red\">Character Count: %lu</p>", strlen(string));
    }
    else
    {
        asprintf(&tmp, "<p>Character Count: %lu</p>", strlen(string));
    }
    EM_ASM_(
        {
            let element = document.querySelector('#char_count');
            element.innerHTML += UTF8ToString($0);
        },
        tmp);
    free(tmp);
    free(string);
}

EMSCRIPTEN_KEEPALIVE
void post_clob()
{
    while (!connected)
    {
        emscripten_thread_sleep(100);
    }

    EM_ASM(
        let element = document.querySelector('#char_count');
        while (element.lastChild)
        {
            element.removeChild(element.lastChild);
        } element.innerHTML += "<p>Character Count: 0</p>");
    emscripten_websocket_send_utf8_text(ws, curr_clob);
    free(curr_clob);
}
