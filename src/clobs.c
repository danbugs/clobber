#ifndef __EMSCRIPTEN__
#include <emscripten.h>
#endif

html_t clob = HTMLIFY("<div class=\"container mb-3 post\"> \
                <div class=\"row\"> \
                    <div class=\"card col-md-12\"> \
                        <div class=\"card-body\"> \
                            <p class=\"card-text\">%s \
                        </div> \
                    </div> \
                </div> \
            </div>");

html_t initial_char_count = HTMLIFY("<p>Character Count: 0</p>");

void free_all_clobs()
{
    for (int i = num_clobs - 1; i >= 0; i--)
    {
        free(*(clobs + i));
    }
    free(clobs);
}

void get_all_clobs()
{
    remove_html_with_class("post");
    display_html_loop(clob, clobs, num_clobs, true);
    free_all_clobs();
}

EMSCRIPTEN_KEEPALIVE
void set_clob(char *string)
{
    asprintf(&curr_clob, "%s", string);
    remove_html_children_of_id("char_count");
    char *tmp;
    int char_count = (int)strlen(string);
    if (char_count > 255)
    {
        asprintf(&tmp, HTMLIFY("<p style=\"color:red\">Character Count: %lu</p>"), strlen(string));
    }
    else
    {
        asprintf(&tmp, HTMLIFY("<p>Character Count: %lu</p>"), strlen(string));
    }
    insert_html_at_id(tmp, "char_count");
    free(tmp);
    free(string);
}

EMSCRIPTEN_KEEPALIVE
void post_clob()
{
    remove_html_children_of_id("char_count");
    insert_html_at_id(initial_char_count, "char_count");
    emscripten_websocket_send_utf8_text(ws, curr_clob);
    free(curr_clob);
}
