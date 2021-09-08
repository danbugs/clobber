#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include <emscripten/websocket.h>
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

extern void display_html(html_t raw_html);

EMSCRIPTEN_WEBSOCKET_T ws = (EMSCRIPTEN_WEBSOCKET_T)NULL;
char **clobs;

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

char **split(char *string, char *separator)
{
    int counter = 1;
    char *tmp = string;

    while (*tmp)
    {
        if (*tmp == *separator)
        {
            counter++;
        }
        tmp++;
    }

    char **result = malloc(sizeof(char *) * counter);
    char *token = strtok(string, separator);

    int index = 0;
    while (token)
    {
        *(result + index++) = strdup(token);
        token = strtok(0, separator);
    }
    *(result + index) = 0;

    return result;
}

void display_clobs()
{
    EM_ASM(
        let element = document.querySelectorAll('.post');
        if (element) {
            element.forEach(function(e){e.remove()});
        });

    if (clobs)
    {
        int i;
        for (i = 0; *(clobs + i); i++)
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
}

EMSCRIPTEN_KEEPALIVE
void post_clob(char *string)
{
    emscripten_websocket_send_utf8_text(ws, string);
}

EM_BOOL wss_on_open(int eventType, const EmscriptenWebSocketOpenEvent *e, void *userData)
{
    printf("opened the connection!\n");
    return 0;
}

EM_BOOL wss_on_close(int eventType, const EmscriptenWebSocketCloseEvent *e, void *userData)
{
    printf("closed the connection!\n");
    return 0;
}

EM_BOOL wss_on_error(int eventType, const EmscriptenWebSocketErrorEvent *e, void *userData)
{
    printf("something went wrong!\n");
    return 0;
}

EM_BOOL wss_on_message(int eventType, const EmscriptenWebSocketMessageEvent *e, void *userData)
{

    clobs = split(e->data, "\n");
    display_clobs();
    return 0;
}

int main()
{
    display_html(main_h);

    if (!emscripten_websocket_is_supported())
    {
        printf("WebSockets are not supported, cannot continue!\n");
        exit(EXIT_FAILURE);
    }

    EmscriptenWebSocketCreateAttributes attr;
    emscripten_websocket_init_create_attributes(&attr);

    int isProd = EM_ASM_INT(
        { return window.location.hostname == "127.0.0.1" ? 1 : 0; });

    attr.url = isProd == 1 ? "ws://localhost:3000/" : "wss://clober-api.herokuapp.com";

    ws = emscripten_websocket_new(&attr);

    if (ws <= 0)
    {
        printf("WebSocket creation failed, error code %d!\n", (EMSCRIPTEN_RESULT)ws);
        exit(EXIT_FAILURE);
    }

    emscripten_websocket_set_onopen_callback(ws, (void *)42, wss_on_open);
    emscripten_websocket_set_onclose_callback(ws, (void *)43, wss_on_close);
    emscripten_websocket_set_onerror_callback(ws, (void *)44, wss_on_error);
    emscripten_websocket_set_onmessage_callback(ws, (void *)45, wss_on_message);
}